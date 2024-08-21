#include "pers.h"
#include "mtk_model.h"

#pragma codeseg CODE2

s16x3 g_meshPosition;
s8 g_meshZNear;

SMtkMesh* MtkModelGetLod(const SMtkModel *pModel, const s16x3 *pPosition)
{
	static u8 lod;
	static SMtkMesh *pMesh;

	s16x3Copy(g_meshPosition,(*pPosition));
	g_meshZNear = g_persContext.m_s8ClipNear;
	lod = 0;
	pMesh = pModel->m_meshes;

	while (90 < g_meshPosition.z)
	{
		g_meshPosition.x >>= 1;
		g_meshPosition.y >>= 1;
		g_meshPosition.z >>= 1;
		g_meshZNear >>= 1;
		lod++;
		pMesh++;

		if (pMesh == NULL ||
			g_meshZNear <= 0 ||
			MTK_MODEL_LOD_MAX <= lod)
		{
			return NULL;
		}
	}

	if (g_meshPosition.x < -127 || 127 < g_meshPosition.x ||
		g_meshPosition.y < -127 || 127 < g_meshPosition.y ||
		g_meshPosition.z < -127 || 127 < g_meshPosition.z)
	{
		return NULL;
	}

	return pMesh;
}

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
	SMtkMesh *pMesh = MtkModelGetLod(pModel, pPosition);
	if (!pMesh)
	{
		return;
	}

	PersSetPosition(g_meshPosition.x, g_meshPosition.y, g_meshPosition.z);
	MtkMeshDrawNoClip(pMesh);
}

void MtkModelDrawBBoxClip(const SMtkModel *pModel, const s16x3 *pPosition)
{
	SMtkMesh *pMesh = MtkModelGetLod(pModel, pPosition);
	if (!pMesh)
	{
		return;
	}

	PersSetPosition(g_meshPosition.x, g_meshPosition.y, g_meshPosition.z);
	if (!PersClipBBoxVram(pMesh->m_bboxVram, g_meshZNear))
	{
		return;
	}

	MtkMeshDrawNoClip(pMesh);
}
