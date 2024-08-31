#include <msx-rand.h>
#include "mtk_star.h"
#include "mtk_world.h"

#define MTK_STAR_MAX (32)
s8x3 g_mtkStars[MTK_STAR_MAX];

MtkStar g_mtkStar;

void MtkStarInit(s8 zNear, s8 zFar)
{
	u8 i;
	s8x3 *pStar = g_mtkStars;
	for(i = 0; i < MTK_STAR_MAX; i++, pStar++)
	{
		s16 x = (msxRandGet16() >> 3);
		s16 y = (msxRandGet8());
		s16 z = (msxRandGet16() >> 7) & 0x07f;
		pStar->x = (s8) x;
		pStar->y = (s8) y;
		pStar->z = (s8) z;
	}

	g_mtkStar.m_zNear = zNear;
	g_mtkStar.m_zFar = zFar;
	g_mtkStar.m_zRange = zFar - zNear;
}

void MtkStarUpdate(void)
{
	static u8 i;
	static s8x3 *pStar;
	static s8x3 velocity;
	static s8 x, y, z;
	
	pStar = g_mtkStars;
	s8x3Copy(velocity, g_mtkWorld.m_velocity);

	for(i = 0; i < MTK_STAR_MAX; i++, pStar++)
	{
		x = pStar->x;
		y = pStar->y;
		z = pStar->z;

		if (g_mtkWorld.m_angularVelocity.x)
		{
			s8XY xy = SinCos_RotateXYS8Lp(y, z, -g_mtkWorld.m_angularVelocity.x);
			y = s8XY_GetX(xy);
			z = s8XY_GetY(xy);
		}

		if (g_mtkWorld.m_angularVelocity.y)
		{
			s8XY xy = SinCos_RotateXYS8Lp(z, x, -g_mtkWorld.m_angularVelocity.y);
			z = s8XY_GetX(xy);
			x = s8XY_GetY(xy);
		}

#if 0
		if (g_mtkWorld.m_angularVelocity.z)
		{
			s8XY xy = SinCos_RotateXYS8Lp(x, y, -g_mtkWorld.m_angularVelocity.z);
			x = s8XY_GetX(xy);
			y = s8XY_GetY(xy);
		}
#endif
		x -= velocity.x;
		y -= velocity.y;
		z -= velocity.z;

		if (z < g_mtkStar.m_zNear)
		{
			z += g_mtkStar.m_zRange;
		}
		else if (g_mtkStar.m_zFar < z)
		{
			z -= g_mtkStar.m_zRange;
		}

		pStar->x = x;
		pStar->y = y;
		pStar->z = z;
	}
}
