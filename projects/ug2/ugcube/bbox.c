#include "macros.h"
#include "bbox.h"

void BBoxCreate(SBBox *pBBox, const s8x3 *pVertices, u8 nbVertices)
{
    u8 i;
    const s8x3 *pVertex = pVertices;
    s8 minX,maxX,minY,maxY,minZ,maxZ;

    minX = maxX = pVertex->x;
    minY = maxY = pVertex->y;
    minZ = maxZ = pVertex->z;
    pVertex++;

    for (i = 1; i < nbVertices; i++)
    {
        minX = MIN(minX,pVertex->x);
        minY = MIN(minY,pVertex->y);
        minZ = MIN(minZ,pVertex->z);
        maxX = MAX(maxX,pVertex->x);
        maxY = MAX(maxY,pVertex->y);
        maxZ = MAX(maxZ,pVertex->z);
        pVertex++;
    }

    pBBox->m_s8MinX = minX;
    pBBox->m_s8MaxX = maxX;
    pBBox->m_s8MinY = minY;
    pBBox->m_s8MaxY = maxY;
    pBBox->m_s8MinZ = minZ;
    pBBox->m_s8MaxZ = maxZ;
}


