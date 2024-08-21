#ifndef MTK_STAR_H
#define MTK_STAR_H

#include <msx-types.h>
#include "vec_math.h"

typedef struct tMtkStar
{
	s8x3 m_v3Velocity;
	s8x3 m_v3AnglerVelocity;
	s8   m_zNear;
	s8   m_zFar;
	s8   m_zRange;
} MtkStar;
extern MtkStar g_mtkStar;

extern void MtkStarInit(s8 zNear, s8 zFar);

#define MtkStarSetVelocity(v3Velocity) s8x3Copy(g_mtkStar.m_v3Velocity, v3Velocity)
#define MtkStarSetAnglerVelocity(v3AnglerVelocity) s8x3Copy(g_mtkStar.m_v3AnglerVelocity, v3AnglerVelocity)
extern void MtkStarUpdate(void);

extern void MtkStarRender(void);

#endif //MTK_STAR_H
