#ifndef MTK_MESH_H
#define MTK_MESH_H

#include <msx-types.h>
#include "mesh.h"

typedef struct tMtkMesh
{
	u16 m_verticesVram;
	u8 m_nbVertices;
	u16 *m_pIndices;
	u8 m_nbLines;
} SMtkMesh;

extern void MtkMeshCreate(SMtkMesh *pMesh, const SMesh *pSrcMesh, u8 rx, u8 ry, u8 rz, u8 shift);
extern void MtkMeshDrawNoClip(const SMtkMesh *pMesh);

#endif // MTK_MESH_H
