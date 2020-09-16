#include <msx-types.h>
#include <msx-bios-wrapper.h>

#include "pers.h"
#include "clip.h"
#include "flipper.h"
#include "vdp_command.h"
#include "mesh.h"
#include "e1.inc"

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

#define MTK_MODEL_LOD_MAX (3)
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
		u8 trigger = msxBiosGetTrigger(2);
		//. shift.
		trigger |= msxBiosReadKeyMatrix(6) & 0x01;
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
typedef struct tMtkPlayer
{
	s16x3 m_velocity;
} SMtkPlayer;
extern SMtkPlayer g_mtkPlayer;

extern void MtkPlayerInit();
extern void MtkPlayerUpdate();

typedef struct tMtkEnemy
{
	s16x3 m_position;
} SMtkEnemy;
#define MTK_ENEMY_MAX (3)

extern SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

extern void MtkEnemyInit();
extern void MtkEnemyUpdate();

SMtkPlayer g_mtkPlayer;
void MtkPlayerInit()
{
	s16x3Set(g_mtkPlayer.m_velocity,0,0,0);
}

void MtkPlayerUpdate()
{
	s16 vx = g_mtkPlayer.m_velocity.x;
	s16 vy = g_mtkPlayer.m_velocity.y;

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

	g_mtkPlayer.m_velocity.x = vx;
	g_mtkPlayer.m_velocity.y = vy;
}

SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];
void MtkEnemyInit()
{
	u8 i;
	for (i = 0; i < MTK_ENEMY_MAX; i++)
	{
		s16 x = i;
		s16 z = i;
		x = -48 + x*48; 
		z = 160 + 40*z;
		s16x3Set(g_mtkEnemies[i].m_position,x,0,z);
	}
}

void MtkEnemyUpdate()
{
	u8 i;
	SMtkEnemy *pEnemy;
	for (i = 0, pEnemy = g_mtkEnemies; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		s16x3Op(pEnemy->m_position, pEnemy->m_position, +, g_mtkPlayer.m_velocity);
	}
}

SMtkModel g_modelEnemy;

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
		vertices[i] = PersRegisterVertices(g_meshe1.m_pVertices, g_meshe1.m_nbVertices, 0,rx,rz,1);
	}
	rz += 8;

	MtkInputInit();
	MtkPlayerInit();
	MtkEnemyInit();

	MtkModelCreate(&g_modelEnemy, &g_meshe1, 0, 128, 0);

	for(;;)
	{
        FlipperClear();

		MtkInputScan();
		MtkPlayerUpdate();
		MtkEnemyUpdate();

        FlipperApplyForegroundColor();
		VDPWait();

		for(i=0;i<MTK_ENEMY_MAX;i++)
		{
			SMtkEnemy *pEnemy = &g_mtkEnemies[i];

			MtkModelDrawNoClip(&g_modelEnemy, &pEnemy->m_position);	
		}

		//PersSetPosition(0,0,80);
		//PersTransformNoClipVram(vertices[u8Frame], g_meshe1.m_nbVertices);
		//PersDrawLines(g_meshe1.m_pIndices,g_meshe1.m_nbLines);

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
