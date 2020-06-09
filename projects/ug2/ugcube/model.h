#ifndef MODEL_H
#define MODEL_H

#include <msx-types.h>
#include "vec_math.h"
#include "pers.h"

typedef struct tModel
{
	s8 m_xMin;
	s8 m_xMax;
	s8 m_yMin;
	s8 m_yMax;
    s8 m_zMin;
    s8 m_zMax;

	u8 m_nbVertices;
	u8 m_nbLines;

	s8x3 *m_pVertices;
	u16 *m_pIndices;
} SModel;

 #define LINE(i0,i1) (PersScreenPositionsAddress+(i0*8)), (PersScreenPositionsAddress+(i1*8))
#endif //MODEL_H
