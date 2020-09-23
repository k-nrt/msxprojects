#include <msx-types.h>
#include <msx-bios-wrapper.h>

#include "pers.h"
#include "clip.h"
#include "flipper.h"
#include "vdp_command.h"

#include "mtk_mesh.h"
#include "mtk_model.h"
#include "mtk_input.h"

#include "mtk_effect.h"
#include "mtk_player.h"
#include "mtk_enemy.h"
#include "mtk_shot.h"

#include "mesh.h"
#include "mtk_mesh_beam.inc"
#include "mtk_mesh_enemy1.inc"
#include "mtk_mesh_exp0.inc"
#include "mtk_mesh_exp1.inc"
#include "mtk_mesh_exp2.inc"
#include "mtk_mesh_exp3.inc"

static u16 s_vertices[64];

extern u8 g_u8Timer;

void WaitVSync()
{
	volatile u8 u8Timer = g_u8Timer;
	while(u8Timer == g_u8Timer);
}

static const SFlipperConfig s_mtkFlipperConfig =
{
	0x11,
	2, 13, 
	{0, 16, 256, 192},
	{
		{0,0,0,0},
		{0,0,0,0},
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,2),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,3),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,4),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,5),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,6),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,7),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,8),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,9),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,10),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,11),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,12),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,13),
		{0,0,0,0},
		{0,0,0,0},
	}
};


SMtkModel g_modelEnemy;
SMtkModel g_modelExp0;
SMtkModel g_modelExp1;
SMtkModel g_modelExp2;
SMtkModel g_modelExp3;
SMtkModel g_modelShot;

void MtkInit()
{
    FlipperInit(&s_mtkFlipperConfig,0x0001,0x0657);
	FlipperPrint(0,0,0xff, "initialize mtk ...");

	MtkInputInit();
	MtkPlayerInit();
	MtkShotInit();
	MtkEnemyInit();
	MtkEffectInit();

	PersSetVertexBuffer(1,0x0000);

	MtkModelCreate(&g_modelEnemy, &g_meshEnemy1, 0, 128, 0);
	MtkModelCreate(&g_modelShot, &g_meshBeam, 0, 0, 0);

	MtkModelCreate(&g_modelExp0, &g_meshExp0, 0, 0, 0);
	MtkModelCreate(&g_modelExp1, &g_meshExp1, 0, 0, 0);
	MtkModelCreate(&g_modelExp2, &g_meshExp2, 0, 0, 0);
	MtkModelCreate(&g_modelExp3, &g_meshExp3, 0, 0, 0);

	FlipperPrint(0,0,0xff, "                  ");
}


void Mtk_Main(const char* pszTitle)
{
	u8 i;

	MtkInit();
	FlipperPrint(0,0,0xff,pszTitle);

	for(;;)
	{
        FlipperClear();

		MtkInputScan();
		MtkPlayerUpdate();
		MtkShotUpdate();
		MtkEnemyUpdate();

		{
			SMtkShot *pShot = g_mtkShots;
			for (i = 0; i < MTK_SHOT_MAX; i++, pShot++)
			{
				if (pShot->m_status == kMtkShotStatus_Move)
				{
					u8 j;
					SMtkEnemy *pEnemy = g_mtkEnemies;

					for (j = 0; j < MTK_ENEMY_MAX; j++, pEnemy++)
					{
						if (pEnemy->m_status != kMtkEnemyStatus_Move)
						{
							continue;
						}

						if
						(
							pEnemy->m_position.x - 32 < pShot->m_position.x &&
							pEnemy->m_position.y - 32 < pShot->m_position.y &&
							pEnemy->m_position.z - 32 < pShot->m_position.z &&
							pShot->m_position.x < pEnemy->m_position.x + 32 &&
							pShot->m_position.y < pEnemy->m_position.y + 32 &&
							pShot->m_position.z < pEnemy->m_position.z + 32
						)
						{
							pEnemy->m_status = kMtkEnemyStatus_Idle;
							pShot->m_status = kMtkShotStatus_Hit;
							MtkEffectSpawn(kMtkEffectType_Explosion, &pEnemy->m_position, &pEnemy->m_velocity);
							break;
						}
					}
				}
			}
		}

		MtkEffectUpdate();

        FlipperApplyForegroundColor();
		VDPWait();

		{
			SMtkEnemy *pEnemy = g_mtkEnemies;
			for (i = 0; i < MTK_ENEMY_MAX; i++, pEnemy++)
			{
				if (pEnemy->m_status != kMtkEnemyStatus_Idle)
				{
					MtkModelDrawNoClip(&g_modelEnemy, &pEnemy->m_position);
				}
			}
		}

		{
			SMtkShot *pShot = g_mtkShots;
			for (i = 0; i < MTK_SHOT_MAX; i++, pShot++)
			{
				if ( pShot->m_status != kMtkShotStatus_Idle)
				{
					MtkModelDrawNoClip(&g_modelShot, &pShot->m_position);
				}
			}
		}

		{
			const SMtkModel *pModels[] = 
			{
				&g_modelExp0,
				&g_modelExp1,
				&g_modelExp2,
				&g_modelExp3,
				NULL, NULL, NULL, NULL
			};
			const SMtkEffect *pEffect = g_mtkEfects;

			for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
			{
				if (pEffect->m_type == kMtkEffectType_None)
				{
					continue;
				}
				else
				{
					u8 index = pEffect->m_timer - 1;
					MtkModelDrawNoClip(pModels[index], &pEffect->m_position);
				}
			}
		}

		WaitVSync();
        FlipperFlip();
	}

    //FlipperTerm();
}
