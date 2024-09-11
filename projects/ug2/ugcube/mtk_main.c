#include <msx-types.h>
#include "bios_wrapper.h"
#include <msx-rand.h>

#include "pers.h"
#include "clip.h"
#include "flipper.h"
#include "vdp_command.h"

#include "mtk_mesh.h"
#include "mtk_model.h"
#include "mtk_input.h"
#include "mtk_world.h"

#include "mtk_effect.h"
#include "mtk_player.h"
#include "mtk_enemy.h"
#include "mtk_enemy_shot.h"
#include "mtk_shot.h"
#include "mtk_star.h"
#include "mtk_far_bg.h"

#include "mesh.h"
#include "mtk_mesh_beam.inc"
#include "mtk_mesh_planet.inc"

#pragma codeseg CODE2

static u16 s_vertices[64];

extern u8 g_u8Timer;

void WaitVSync(void)
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


SMtkModel g_modelShot;

SMtkMesh g_mtkMeshPlanet;

void MtkOnProgress(u16 address)
{
	VDPPrintU16X(256 - 32, 8, address);
}

const s8 kMtk_NearZ = 32;
const s16 kMtk_ScreenZ = 160;
const s16 kMtk_ScreenCenterX = 128;
const s16 kMtk_ScreenCenterY = 16 + 96;
const s16 kMtk_ViewportLeft = 0;
const s16 kMtk_ViewportRight = 256 - 1;
const s16 kMtk_ViewportTop = 16;
const s16 kMtk_ViewportBottom = 16 + 96 + 96 - 1;


void MtkInit(void)
{
	msxRandInit(0x53, 0x94a1);

	//. Flipper.
    FlipperInit(&s_mtkFlipperConfig, 0x0001, 0x0657);
	FlipperPrint(0, 0, 0xff, "initializing mtk ...");

    VDPSetActivePage(g_flipper.m_u8DisplayPage);
	VDPSetForegroundColor(0xff);

	//. Perspective Transform Engine.
	VDPPrint(0, 8, "create transform table ... ");
	PersInit
	(
		kMtk_NearZ, kMtk_ScreenZ,
		kMtk_ScreenCenterX, kMtk_ScreenCenterY,
		kMtk_ViewportLeft, kMtk_ViewportRight, kMtk_ViewportTop, kMtk_ViewportBottom,
		MtkOnProgress
	);
	Clip_SetRect(kMtk_ViewportLeft, kMtk_ViewportRight, kMtk_ViewportTop, kMtk_ViewportBottom);

	//. Subsystems.
	MtkInputInit();
	MtkPlayerInit();
	MtkShotInit();
	MtkEnemyInit();
	MtkEnemyShotInit();
	MtkEffectInit();
	MtkStarInit(8, 128 - 16);
	MtkFarBgInit();
	MtkWorldInit();

	//. Models.
	VDPPrint(0, 16, "create models ... ");

	PersSetVertexBuffer(1,0x0000);

	MtkEnemyCreateModels();
	MtkEnemyShotCreateModels();
	MtkEffectCreateModels();

	MtkModelCreate(&g_modelShot, &g_meshBeam, 0, 0, 0);

	MtkMeshCreate(&g_mtkMeshPlanet, &g_meshPlanet, 0, 0, 0, 0);

	MtkFarBgSetModel(0, &g_mtkMeshPlanet);
	MtkFarBgSetPosition(0, 0, 0, 96, 96);

	//. Clear screen.
	VDPSetForegroundColor(0x00);
	VDPWait();
	VDPFill(0, 0, 256, 212);
	VDPWait();

	//. Restore back buffer.
	VDPSetActivePage(g_flipper.m_u8ActivePage);
	VDPFill(0, 0, 256, 212);
	VDPWait();
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
		MtkEnemyShotUpdate();
		MtkStarUpdate();
		MtkFarBgUpdate();

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
						if (!pEnemy->m_shield)
						{
							continue;
						}

						if
						(
							pEnemy->m_position.x - 32 < pShot->m_position.x &&
							pEnemy->m_position.y - 32 < pShot->m_position.y &&
							pEnemy->m_position.z - 48 < pShot->m_position.z &&
							pShot->m_position.x < pEnemy->m_position.x + 32 &&
							pShot->m_position.y < pEnemy->m_position.y + 32 &&
							pShot->m_position.z < pEnemy->m_position.z + 48
						)
						{
							pShot->m_status = kMtkShotStatus_Hit;
							if (MtkEnemyAddDamage(1, pEnemy) <= 0)
							{
								MtkEffectSpawn(kMtkEffectType_Explosion, &pEnemy->m_position, &pEnemy->m_velocity);
							}
							break;
						}
					}
				}
			}
		}

		MtkEffectUpdate();

		FlipperApplyForegroundColor();
		VDPWait();

		MtkFarBgRender();
		MtkEnemyRender();
		MtkEnemyShotRender();

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

		MtkEffectRender();

		{
			LOGOPR = 0;
			VDPWait();
			VDPPSet(128, 16+96);
			MtkStarRender();
		}
		WaitVSync();
		FlipperFlip();
	}

	//FlipperTerm();
}
