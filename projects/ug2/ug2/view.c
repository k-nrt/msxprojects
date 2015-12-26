#include "view.h"
#include "sincos.h"
#include "math.h"

#pragma codeseg CODE2

SView g_view;

void ViewInitialize()
{
	g_view.m_u8RotationZ = 0;
	Vec3Set(g_view.m_v3Position, 0, 0, 0);
	Vec3Set(g_view.m_v3TransformResult, 0, 0, 0);
	g_view.m_s16Cos = 1;
	g_view.m_s16Sin = 0;
}

void ViewUpdate(s16 s16X, s16 s16Y, u8 u8Rot)
{
	g_view.m_u8RotationZ = u8Rot;
	g_view.m_v3Position.x = s16X;
	g_view.m_v3Position.y = s16Y;

	VSIN = SinCos_GetSin((u8)g_view.m_u8RotationZ);
	VCOS = SinCos_GetCos((u8)g_view.m_u8RotationZ);
}

void ViewTransform(const s16x3 *pPosition)
{
	static s16 wx, wy, wz;
	wx = -g_view.m_v3Position.x + pPosition->x;
	wy = -g_view.m_v3Position.y + pPosition->y;
	wz = -g_view.m_v3Position.z + pPosition->z;

	VX = (Math_MulS16xS16_S32(wx, g_view.m_s16Cos) >> 8) - (Math_MulS16xS16_S32(wy, g_view.m_s16Sin) >> 8);
	VY = (Math_MulS16xS16_S32(wx, g_view.m_s16Sin) >> 8) + (Math_MulS16xS16_S32(wy, g_view.m_s16Cos) >> 8);
	VZ = wz;
}

