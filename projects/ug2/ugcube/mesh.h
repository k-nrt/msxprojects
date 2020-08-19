#ifndef MESH_H
#define MESH_H

#include <msx-types.h>
#include "vec_math.h"
#include "pers.h"

typedef struct tMesh
{
	u8 m_nbVertices;
	u8 m_nbLines;

	s8x3 *m_pVertices;
	u16 *m_pIndices;
} SMesh;

 #define LINE(i0,i1) (PersScreenPositionsAddress+(i0*8)), (PersScreenPositionsAddress+(i1*8))
#endif //MESH_H
