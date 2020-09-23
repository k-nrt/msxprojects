#include "pers.h"
#include "mtk_model.h"

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
