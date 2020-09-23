#include <msx-types.h>
#include <msx-bios-wrapper.h>

#include "pers.h"
#include "clip.h"
#include "flipper.h"
#include "vdp_command.h"

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

typedef struct tMtkMesh
{
	u16 m_verticesVram;
	u8 m_nbVertices;
	u16 *m_pIndices;
	u8 m_nbLines;
} SMtkMesh;

extern void MtkMeshCreate(SMtkMesh *pMesh, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz, u8 shift);
extern void MtkMeshDrawNoClip(const SMtkMesh *pMesh);

void MtkMeshCreate(SMtkMesh *pMesh, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz, u8 shift)
{
	pMesh->m_verticesVram = PersRegisterVertices(pSrcMesh->m_pVertices, pSrcMesh->m_nbVertices, rx, ry, rz, shift);
	pMesh->m_nbVertices = pSrcMesh->m_nbVertices;
	pMesh->m_pIndices = pSrcMesh->m_pIndices;
	pMesh->m_nbLines = pSrcMesh->m_nbLines;
}

void MtkMeshDrawNoClip(const SMtkMesh *pMesh)
{
	PersTransformNoClipVram(pMesh->m_verticesVram, pMesh->m_nbVertices);
	PersDrawLines(pMesh->m_pIndices, pMesh->m_nbLines);
}

#define MTK_MODEL_LOD_MAX (4)
typedef struct tMtkModel
{
	SMtkMesh m_meshes[MTK_MODEL_LOD_MAX];
} SMtkModel;
extern void MtkModelCreate(SMtkModel *pModel, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz); 

void MtkModelCreate(SMtkModel *pModel, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz)
{
	u8 i;
	SMtkMesh *pMesh;
	for (i = 0, pMesh = pModel->m_meshes; i < MTK_MODEL_LOD_MAX; i++, pMesh++)
	{
		MtkMeshCreate(pMesh, pSrcMesh, rx, ry, rz, i);
	}
}

void MtkModelDrawNoClip(const SMtkModel *pModel, const s16x3 *pPosition)
{
	s16x3 v3Temp;
	u8 lod = 0;
	SMtkMesh *pMesh = pModel->m_meshes;

	s16x3Copy(v3Temp,(*pPosition));
	
	while (90 < v3Temp.z)
	{
		v3Temp.x >>= 1;
		v3Temp.y >>= 1;
		v3Temp.z >>= 1;
		lod++;
		pMesh++;

		if (MTK_MODEL_LOD_MAX <= lod)
		{
			return;
		}
	}

	PersSetPosition(v3Temp.x,v3Temp.y,v3Temp.z);
	MtkMeshDrawNoClip(pMesh);
}

typedef struct tMtkInput
{
	u8 m_down;
	u8 m_up;
	u8 m_push;
	u8 m_release;
} SMtkInput;

enum EMtkInput
{
	kMtkInput_Up     = 0x01,
	kMtkInput_Down   = 0x02,
	kMtkInput_Left   = 0x04,
	kMtkInput_Right  = 0x08,
	kMtkInput_Fire   = 0x10,
	kMtkInput_Thrust = 0x20,
};

extern SMtkInput g_mtkInput;
extern void MtkInputInit();
extern void MtkInputScan();
#define MtkInputIsDown(input)    (g_mtkInput.m_down    & (input))
#define MtkInputIsUp(input)      (g_mtkInput.m_up      & (input))
#define MtkInputIsPush(input)    (g_mtkInput.m_push    & (input))
#define MtkInputIsRelease(input) (g_mtkInput.m_release & (input))

SMtkInput g_mtkInput;
void MtkInputInit()
{
	g_mtkInput.m_down = 0;
	g_mtkInput.m_up = 0xff;
	g_mtkInput.m_push = 0;
	g_mtkInput.m_release = 0;
}

void MtkInputScan()
{
	u8 down = 0;
	u8 i;

	for (i = 0; i < 2; i++)
	{
		u8 stick = msxBiosGetStick(i);

		if (!stick)
		{
			continue;
		}

		if (stick < 3 || 7 < stick)
		{
			down |= kMtkInput_Up;
		}
		else if (3 < stick && stick < 7)
		{
			down |= kMtkInput_Down;
		}

		if (1 < stick && stick < 5)
		{
			down |= kMtkInput_Right;
		}
		else if (5 < stick)
		{
			down |= kMtkInput_Left;
		}
	}

	{
		u8 trigger = msxBiosGetTrigger(0);
		trigger |= msxBiosGetTrigger(1);
		if (trigger)
		{
			down |= kMtkInput_Fire;
		}
	}

	{
		u8 trigger = msxBiosGetTrigger(3);
		//. shift.
		trigger |= ~msxBiosReadKeyMatrix(6) & 0x01;
		if (trigger)
		{
			down |= kMtkInput_Thrust;
		}
	}

	{
		u8 up = ~down;
		//. down edge.
		g_mtkInput.m_push = (g_mtkInput.m_down ^ down) & down;

		//. up edge.
		g_mtkInput.m_release = (g_mtkInput.m_up ^ up) & up;

		g_mtkInput.m_down = down;
		g_mtkInput.m_up = up;
	}
}

typedef struct tMtkEffect
{
	s16x3 m_position;
	s16x3 m_velocity;
	u8 m_type;
	u8 m_timer;
} SMtkEffect;

enum EMtkEffectType
{
	kMtkEffectType_None,
	kMtkEffectType_Explosion,
};

#define MTK_EFFECT_MAX (3)
extern SMtkEffect g_mtkEfects[MTK_EFFECT_MAX];
extern void MtkEffectInit();
extern u8 MtkEffectSpawn(enum EMtkEffectType type, const s16x3 *pPosition, const s16x3 *pVelocity);
extern void MtkEffectUpdate();

typedef struct tMtkPlayer
{
	s16x3 m_velocity;
} SMtkPlayer;
extern SMtkPlayer g_mtkPlayer;

extern void MtkPlayerInit();
extern void MtkPlayerUpdate();

typedef struct tMtkEnemy
{
	u8 m_status;
	s16x3 m_position;
	s16x3 m_velocity;
} SMtkEnemy;
#define MTK_ENEMY_MAX (3)

enum EMtkEnemyStatus
{
	kMtkEnemyStatus_Idle,
	kMtkEnemyStatus_Move,
};

extern SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

extern void MtkEnemyInit();
extern void MtkEnemyUpdate();

enum EMtkShotStatus
{
	kMtkShotStatus_Idle,
	kMtkShotStatus_Move,
	kMtkShotStatus_Hit,
};

typedef struct tMtkShot
{
	u8    m_status;
	s16x3 m_position;
	s16x3 m_velocity;
} SMtkShot;
#define MTK_SHOT_MAX (4)
extern SMtkShot g_mtkShots[MTK_SHOT_MAX];

extern void MtkShotInit();
extern void MtkShotUpdate();

SMtkEffect g_mtkEfects[MTK_EFFECT_MAX];
void MtkEffectInit()
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		s16x3Set(pEffect->m_position, 0, 0, 0);
		s16x3Set(pEffect->m_velocity, 0, 0, 0);
		pEffect->m_type = kMtkEffectType_None;
		pEffect->m_timer = 0;
	}
}

u8 MtkEffectSpawn(enum EMtkEffectType type, const s16x3 *pPosition, const s16x3 *pVelocity)
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	UNUSED(type);
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		if (pEffect->m_type != kMtkEffectType_None)
		{
			continue;
		}
		s16x3Copy(pEffect->m_position, (*pPosition));
		s16x3Copy(pEffect->m_velocity, (*pVelocity));
		pEffect->m_type = kMtkEffectType_Explosion;
		pEffect->m_timer = 0;
		return TRUE;
	}

	return FALSE;
}

void MtkEffectUpdate()
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		if (pEffect->m_type == kMtkEffectType_None)
		{
			continue;
		}

		s16x3Op(pEffect->m_position, pEffect->m_position, +, pEffect->m_velocity);
		pEffect->m_timer++;
		if (4 < pEffect->m_timer)
		{
			pEffect->m_type = kMtkEffectType_None;
		}
	}
}

SMtkPlayer g_mtkPlayer;
void MtkPlayerInit()
{
	s16x3Set(g_mtkPlayer.m_velocity,0,0,0);
}

void MtkPlayerUpdate()
{
	s16 vx = g_mtkPlayer.m_velocity.x;
	s16 vy = g_mtkPlayer.m_velocity.y;
	s16 vz = g_mtkPlayer.m_velocity.z;
	
	if (MtkInputIsDown(kMtkInput_Right))
	{
		if (vx < 8)
		{
			vx += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Left))
	{
		if (-8 < vx)
		{
			vx -= 1;
		}
	}
	else if (vx < 0)
	{
		vx += 1;
	}
	else if (0 < vx)
	{
		vx -= 1;
	}

	if (MtkInputIsDown(kMtkInput_Down))
	{
		if (vy < 8)
		{
			vy += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Up))
	{
		if (-8 < vy)
		{
			vy -= 1;
		}
	}
	else if (vy < 0)
	{
		vy += 1;
	}
	else if (0 < vy)
	{
		vy -= 1;
	}

	if (MtkInputIsDown(kMtkInput_Thrust))
	{
		if (vz < 8)
		{
			vz += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Fire))
	{
		if (-8 < vz)
		{
			vz -= 1;
		}
	}
	else if (vz < 0)
	{
		vz += 1;
	}
	else if (0 < vz)
	{
		vz -= 1;
	}

	g_mtkPlayer.m_velocity.x = vx;
	g_mtkPlayer.m_velocity.y = vy;
	g_mtkPlayer.m_velocity.z = vz;
}

SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];
void MtkEnemyInit()
{
	u8 i;
	SMtkEnemy *pEnemy = g_mtkEnemies;
	for (i = 0; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		s16 x = i;
		s16 z = i;
		x = -48 + x*48; 
		z = 160 + 40*z;
		s16x3Set(pEnemy->m_position, x, 0, z);
		s16x3Set(pEnemy->m_velocity, 0, 0, 0);
		pEnemy->m_status = kMtkEnemyStatus_Move;
	}
}

void MtkEnemyUpdate()
{
	u8 i;
	SMtkEnemy *pEnemy;
	for (i = 0, pEnemy = g_mtkEnemies; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		if (pEnemy->m_status == kMtkEnemyStatus_Move)
		{
			s16x3Op(pEnemy->m_position, pEnemy->m_position, -, g_mtkPlayer.m_velocity);
		}
	}
}

SMtkShot g_mtkShots[MTK_SHOT_MAX];
void MtkShotInit()
{
	u8 i;
	SMtkShot *pShot = g_mtkShots;
	for(i=0; i < MTK_SHOT_MAX; i++, pShot++)
	{
		pShot->m_status = kMtkShotStatus_Idle;
		s16x3Set(pShot->m_position, 0, 0, 0);
		s16x3Set(pShot->m_velocity, 0, 0, 0);

	}
}

void MtkShotUpdate()
{
	u8 i;
	SMtkShot *pShot = g_mtkShots;
	u8 spawn = MtkInputIsPush(kMtkInput_Fire) ? 1 : 0;
	for(i=0; i < MTK_SHOT_MAX; i++, pShot++)
	{
		if (pShot->m_status == kMtkShotStatus_Move)
		{
			s16x3 velocity;
			s16x3Op(velocity,pShot->m_velocity,-,g_mtkPlayer.m_velocity);
			s16x3Op(pShot->m_position, pShot->m_position, +, velocity);

			if (720 < pShot->m_position.z)
			{
				pShot->m_status = kMtkShotStatus_Idle;
			}
		}
		else if (pShot->m_status == kMtkShotStatus_Hit)
		{
			s16x3 velocity;
			s16x3Op(velocity,pShot->m_velocity,-,g_mtkPlayer.m_velocity);
			s16x3Op(pShot->m_position, pShot->m_position, +, velocity);

			pShot->m_status = kMtkShotStatus_Idle;
		}
		else if (spawn)
		{
			spawn = 0;
			pShot->m_status = kMtkShotStatus_Move;
			s16x3Set(pShot->m_position,  16, 8, 65);
			pShot->m_velocity.x = g_mtkPlayer.m_velocity.x;
			pShot->m_velocity.y = g_mtkPlayer.m_velocity.y;
			pShot->m_velocity.z = 64;
		}
	}
}


SMtkModel g_modelEnemy;
SMtkModel g_modelExp0;
SMtkModel g_modelExp1;
SMtkModel g_modelExp2;
SMtkModel g_modelExp3;
SMtkModel g_modelShot;

void Mtk_Main(const char* pszTitle)
{
	static u8 rz = 0;
	u8 i;
	u16* vertices = s_vertices;
	u8 u8TrigPrev = 0;
	u8 u8Frame = 0;
	
    FlipperInit(&s_mtkFlipperConfig,0x0001,0x0657);
	FlipperPrint(0,0,0xff,pszTitle);

	PersSetVertexBuffer(1,0x0000);

	for (i = 0; i < 64; i++)
	{
		u8 rx = i << 2;
		vertices[i] = PersRegisterVertices(g_meshEnemy1.m_pVertices, g_meshEnemy1.m_nbVertices, 0,rx,rz,1);
	}
	rz += 8;

	MtkInputInit();
	MtkPlayerInit();
	MtkShotInit();
	MtkEnemyInit();
	MtkEnemyInit();

	MtkModelCreate(&g_modelEnemy, &g_meshEnemy1, 0, 128, 0);
	MtkModelCreate(&g_modelShot, &g_meshBeam, 0, 0, 0);

	MtkModelCreate(&g_modelExp0, &g_meshExp0, 0, 0, 0);
	MtkModelCreate(&g_modelExp1, &g_meshExp1, 0, 0, 0);
	MtkModelCreate(&g_modelExp2, &g_meshExp2, 0, 0, 0);
	MtkModelCreate(&g_modelExp3, &g_meshExp3, 0, 0, 0);

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

		u8Frame++;
		if (64 <= u8Frame)
		{
			u8Frame = 0;
		}

		WaitVSync();
        FlipperFlip();
	}

    //FlipperTerm();
}
