#include <msx-rand.h>
#include "mtk_star.h"

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

	s8x3Set(g_mtkStar.m_v3Velocity, 0, 0, 0);
	s8x3Set(g_mtkStar.m_v3AnglerVelocity, 0, 0, 0);
	g_mtkStar.m_zNear = zNear;
	g_mtkStar.m_zFar = zFar;
	g_mtkStar.m_zRange = zFar - zNear;
}

void MtkStarUpdate(void)
{
	static u8 i;
	static s8x3 *pStar;
	pStar = g_mtkStars;
	for(i = 0; i < MTK_STAR_MAX; i++, pStar++)
	{
		static s8 x, y, z;
		x = pStar->x;
		y = pStar->y;
		z = pStar->z;

		if (g_mtkStar.m_v3AnglerVelocity.x)
		{
			s8XY xy = SinCos_RotateXYS8Lp(y, z, -g_mtkStar.m_v3AnglerVelocity.x);
			y = s8XY_GetX(xy);
			z = s8XY_GetY(xy);
		}

		if (g_mtkStar.m_v3AnglerVelocity.y)
		{
			s8XY xy = SinCos_RotateXYS8Lp(z, x, -g_mtkStar.m_v3AnglerVelocity.y);
			z = s8XY_GetX(xy);
			x = s8XY_GetY(xy);
		}

		if (g_mtkStar.m_v3AnglerVelocity.z)
		{
			s8XY xy = SinCos_RotateXYS8Lp(x, y, -g_mtkStar.m_v3AnglerVelocity.z);
			x = s8XY_GetX(xy);
			y = s8XY_GetY(xy);
		}

		x -= g_mtkStar.m_v3Velocity.x;
		y -= g_mtkStar.m_v3Velocity.y;
		z -= g_mtkStar.m_v3Velocity.z;

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
