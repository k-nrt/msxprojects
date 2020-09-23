#include "pers.h"
#include "mtk_mesh.h"

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
