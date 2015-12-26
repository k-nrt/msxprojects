#include "explosion.h"
#include "model_data.h"
#include "view.h"

SExplosion g_explosion[EXPLOSION_MAX];

UgxBuffer g_bufferExplosionIndices[4];
u8 g_nbExplosionIndices[4];
UgxBuffer g_bufferExplosionVertices[4];
u8 g_nbExplosionVertices[4];

const u8 g_nExplosionColors[4] = { 0x66, 0x88, 0x99, 0xbb };

void ExplosionInitialize()
{
	register u8 i;
	register SExplosion *pExplosion;
	const SUgxModel* pExpModels[] =
	{
		&g_modelExp3,
		&g_modelExp2,
		&g_modelExp1,
		&g_modelExp0,
	};

	for (i = 0,pExplosion = g_explosion; i < EXPLOSION_MAX; i++,pExplosion++)
	{
		Vec3Set(pExplosion->m_v3Position, -100 + i * 60, 0, 200);
		Vec3Set(pExplosion->m_v3Velocity, 0, 0, 0);
		pExplosion->m_liveCount = (i + 1) * 2;
	}

	for (i = 0; i < 4; i++)
	{
		g_bufferExplosionIndices[i] = UgxCreateIndexBuffer(pExpModels[i]->pIndices, pExpModels[i]->nbLines * 2);
		g_nbExplosionIndices[i] = pExpModels[i]->nbLines;
		g_bufferExplosionVertices[i] = UgxCreateVertexBuffer(pExpModels[i]->pVertices, pExpModels[i]->nbVertices, kRotationOrder_XYZ, 0, 0, 0, 0x200);
		g_nbExplosionVertices[i] = pExpModels[i]->nbVertices;
	}

}

void ExplosionUpdateAndRender()
{
	register u8 i;
	register SExplosion *pExplosion;

	for (i = 0, pExplosion = g_explosion; i < EXPLOSION_MAX; i++, pExplosion++)
	{
		if (pExplosion->m_liveCount < 4)
		{
			register u8 index = pExplosion->m_liveCount;
			ViewTransform(&pExplosion->m_v3Position);

			UgxColor(g_nExplosionColors[index]);
			UgxSetModelPosition(VX, VY, VZ);
			UgxDrawModel(//VX, VY, VZ,
				g_bufferExplosionVertices[index],
				g_nbExplosionVertices[index],
				g_bufferExplosionIndices[index],
				g_nbExplosionIndices[index]);
		}

		if (pExplosion->m_liveCount == 0)
		{
			pExplosion->m_liveCount = 8;
		}
		else
		{
			pExplosion->m_liveCount--;
		}
	}
}

void ExplosionRender(const s16x3 *pPosition, u8 liveCount)
{
	if (liveCount < 4)
	{
		register u8 index = liveCount;
		ViewTransform(pPosition);

		UgxColor(g_nExplosionColors[index]);
		UgxSetModelPosition(VX, VY, VZ);
		UgxDrawModel(//VX, VY, VZ,
			g_bufferExplosionVertices[index],
			g_nbExplosionVertices[index],
			g_bufferExplosionIndices[index],
			g_nbExplosionIndices[index]);
	}
}
