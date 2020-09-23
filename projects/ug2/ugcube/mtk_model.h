#ifndef MTK_MODEL_H
#define MTK_MODEL_H

#include <msx-types.h>
#include "mesh.h"
#include "mtk_mesh.h"

#define MTK_MODEL_LOD_MAX (4)

typedef struct tMtkModel
{
	SMtkMesh m_meshes[MTK_MODEL_LOD_MAX];
} SMtkModel;

extern void MtkModelCreate(SMtkModel *pModel, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz); 
extern void MtkModelDrawNoClip(const SMtkModel *pModel, const s16x3 *pPosition);

#endif //MTK_MODEL_H
