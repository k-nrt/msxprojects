#include "enemy.h"
#include "pers.h"
#include "model_data.h"
#include "macros.h"
#include "player.h"
#include "view.h"
#include "explosion.h"

SEnemy g_enemies[ENEMY_MAX];

#define ENEMY_ROTATIONZ_MAX (VIEW_ROTATION_Z_MAX/2)

UgxBuffer g_bufferEmemy0Indices;
UgxBuffer g_bufferEnemy0Vertices[VIEW_ROTATION_Z_MAX + 1];

UgxBuffer g_bufferEmemy1Indices;
UgxBuffer g_bufferEnemy1Vertices[VIEW_ROTATION_Z_MAX + 1];

#define ENEMY_ROCK_ROTATION_MAX (32)

UgxBuffer g_bufferEmemy2Indices;
UgxBuffer g_bufferEnemy2Vertices[ENEMY_ROCK_ROTATION_MAX];


struct
{
	u8 m_liveEnemies;
} g_enemyContext;

void EnemyInitialize()
{
	register u8 i;
	register s8 r;
	register SEnemy *pEnemy;
	for (i = 0,pEnemy = g_enemies; i < ENEMY_MAX; i++,pEnemy++)
	{
		pEnemy->m_eType = kEnemyType_None;
		pEnemy->m_eStatus = kEnemyStatus_Idle;
		pEnemy->m_liveCount = 0;
		pEnemy->m_rotation = 0;
		Vec3Set(pEnemy->m_v3Position, 0, 0, 0);
	}

	g_enemyContext.m_liveEnemies = 0;

	g_bufferEmemy0Indices = UgxCreateIndexBuffer(g_modelEnemy0.pIndices, g_modelEnemy0.nbLines * 2);
	g_bufferEmemy1Indices = UgxCreateIndexBuffer(g_modelEnemy1.pIndices, g_modelEnemy1.nbLines * 2);
	for (i = 0, r = -VIEW_ROTATION_Z_MAX; i < (VIEW_ROTATION_Z_MAX + 1); i++, r += VIEW_ROTATION_Z_MAX / ENEMY_ROTATIONZ_MAX)
	{
		g_bufferEnemy0Vertices[i] = UgxCreateVertexBuffer(g_modelEnemy0.pVertices, g_modelEnemy0.nbVertices, kRotationOrder_XYZ, 0, 0, r, 0x100);
		g_bufferEnemy1Vertices[i] = UgxCreateVertexBuffer(g_modelEnemy1.pVertices, g_modelEnemy1.nbVertices, kRotationOrder_XYZ, 0, 0, r, 0x100);
	}

	g_bufferEmemy2Indices = UgxCreateIndexBuffer(g_modelEnemy2.pIndices, g_modelEnemy2.nbLines * 2);
	for (i = 0, r = 0 ; i < ENEMY_ROCK_ROTATION_MAX ; i++, r -= 0x100 / ENEMY_ROCK_ROTATION_MAX)
	{
		g_bufferEnemy2Vertices[i] = UgxCreateVertexBuffer(g_modelEnemy2.pVertices, g_modelEnemy2.nbVertices, kRotationOrder_XYZ, 40, r, 60, 0x200);
	}

}

void EnemyBeginExplosion(SEnemy *pEnemy)
{
	pEnemy->m_eStatus = kEnemyStatus_Explosion;
	pEnemy->m_liveCount = 5;
}

static void EnemyUpdateAndRender_Explosion(SEnemy *pEnemy)
{
	if ( 0 < pEnemy->m_liveCount)
	{
		pEnemy->m_liveCount--;
		ExplosionRender(&pEnemy->m_v3Position, pEnemy->m_liveCount);
	}
	else
	{
		pEnemy->m_eStatus = kEnemyStatus_Idle;
	}
}

static void EnemyUpdateAndRender_Type0(SEnemy *pEnemy)
{
	register u8 modelRotY;

	pEnemy->m_v3Position.z -= g_player.m_v3Velocity.z >> 2;

	if (pEnemy->m_v3Position.z < 0)
	{
		pEnemy->m_eStatus = kEnemyStatus_Idle;
	}

	ViewTransform(&pEnemy->m_v3Position);

	modelRotY = (((s8)g_view.m_u8RotationZ) >> 1) + 4;

	UgxColor(0x33);
	UgxSetModelPosition(VX, VY, VZ);
	UgxDrawModel
	(
//		VX, VY, VZ,
		g_bufferEnemy0Vertices[modelRotY],
		g_modelEnemy0.nbVertices,
		g_bufferEmemy0Indices,
		g_modelEnemy0.nbLines
	);
}

static void EnemyUpdateAndRender_Type1(SEnemy *pEnemy)
{
	register u8 modelRotY;

	pEnemy->m_v3Position.z -= g_player.m_v3Velocity.z >> 2;

	if (pEnemy->m_v3Position.z < 0)
	{
		pEnemy->m_eStatus = kEnemyStatus_Idle;
	}

	ViewTransform(&pEnemy->m_v3Position);

	modelRotY = (((s8)g_view.m_u8RotationZ) >> 1) + 4;

	UgxColor(0x33);
	UgxSetModelPosition(VX, VY, VZ);
	UgxDrawModel
	(
//		VX, VY, VZ,
		g_bufferEnemy1Vertices[modelRotY],
		g_modelEnemy1.nbVertices,
		g_bufferEmemy1Indices,
		g_modelEnemy1.nbLines
	);
}

static void EnemyUpdateAndRender_Type2(SEnemy *pEnemy)
{
	register u8 modelRotY;

	pEnemy->m_v3Position.z -= g_player.m_v3Velocity.z >> 2;

	if (pEnemy->m_v3Position.z < 0)
	{
		pEnemy->m_eStatus = kEnemyStatus_Idle;
	}

	pEnemy->m_rotation++;
	if (ENEMY_ROCK_ROTATION_MAX <= pEnemy->m_rotation)
	{
		pEnemy->m_rotation = 0;
	}

	ViewTransform(&pEnemy->m_v3Position);

	UgxColor(0x66);
	UgxSetModelPosition(VX, VY, VZ);
	UgxDrawModel
	(
		//		VX, VY, VZ,
		g_bufferEnemy2Vertices[pEnemy->m_rotation],
		g_modelEnemy2.nbVertices,
		g_bufferEmemy2Indices,
		g_modelEnemy2.nbLines
	);
}

void EnemyUpdateAndRender()
{
	register u8 i;
	register SEnemy *pEnemy;
	g_enemyContext.m_liveEnemies = 0;

	for (i = 0, pEnemy = g_enemies; i < ENEMY_MAX; i++, pEnemy++)
	{
		if (pEnemy->m_eStatus == kEnemyStatus_Idle)
		{
			continue;
		}

		if (pEnemy->m_eStatus == kEnemyStatus_Explosion)
		{
			if (pEnemy->m_liveCount < 5)
			{
				EnemyUpdateAndRender_Explosion(pEnemy);
				continue;
			}
			else
			{
				pEnemy->m_liveCount--;
			}
		}

		switch (pEnemy->m_eType)
		{
		case kEnemyType_None:
			break;

		case kEnemyType_Type0:
			EnemyUpdateAndRender_Type0(pEnemy);
			break;

		case kEnemyType_Type1:
			EnemyUpdateAndRender_Type1(pEnemy);
			break;

		case kEnemyType_Type2:
			EnemyUpdateAndRender_Type2(pEnemy);
			break;
		}

		g_enemyContext.m_liveEnemies++;
	}

	if (g_enemyContext.m_liveEnemies <= 0)
	{
		register u8 x, y;
		pEnemy = g_enemies;
		for (y = 0; y < 2;y++)
		{
			register s16 yy = -64 + ((s16)y) * 128;
			for (x = 0; x < 4; x++)
			{
				register s16 xx = -192 + ((s16)x) * 128;

				pEnemy->m_eType = kEnemyType_Type2;
				pEnemy->m_eStatus = kEnemyStatus_Active;
				pEnemy->m_liveCount = 0;
				pEnemy->m_rotation = x + y;
				Vec3Set(pEnemy->m_v3Position, xx, yy, 768);

				pEnemy++;
			}
		}
	}
}
