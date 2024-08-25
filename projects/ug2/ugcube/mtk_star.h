#ifndef MTK_STAR_H
#define MTK_STAR_H

#include <msx-types.h>
#include "vec_math.h"

typedef struct tMtkStar
{
	s8x3 m_velocity;
	s8x3 m_angularVelocity;
	s8   m_zNear;
	s8   m_zFar;
	s8   m_zRange;
} MtkStar;
extern MtkStar g_mtkStar;

extern void MtkStarInit(s8 zNear, s8 zFar);

#define MtkStarSetVelocity(velocity) s8x3Copy(g_mtkStar.m_velocity, velocity)
#define MtkStarSetAnglerVelocity(angularVelocity) s8x3Copy(g_mtkStar.m_angularVelocity, angularVelocity)
extern void MtkStarUpdate(void);

extern void MtkStarRender(void);

#endif //MTK_STAR_H
