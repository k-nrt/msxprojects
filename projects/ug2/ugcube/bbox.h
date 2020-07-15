#ifndef BBOX_H
#define BBOX_H

#include <msx-types.h>
#include "vec_math.h"

typedef struct tBBox
{
	s8 m_s8MinX;
	s8 m_s8MaxX;
	s8 m_s8MinY;
	s8 m_s8MaxY;
    s8 m_s8MinZ;
    s8 m_s8MaxZ;
} SBBox;

extern void BBoxCreate(SBBox *pBBox, const s8x3 *pVertices, u8 nbVertices);

#endif //BBOX_H
