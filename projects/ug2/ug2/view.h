#ifndef VIEW_H
#define VIEW_H

#include "vec_math.h"

typedef struct tagView
{
	s16x3 m_v3Position;
	u8 m_u8RotationZ;

	s16x3 m_v3TransformResult;
	s16 m_s16Sin;
	s16 m_s16Cos;
} SView;

extern SView g_view;

#define VX g_view.m_v3TransformResult.x
#define VY g_view.m_v3TransformResult.y
#define VZ g_view.m_v3TransformResult.z
#define VSIN g_view.m_s16Sin
#define VCOS g_view.m_s16Cos

void ViewInitialize();
void ViewUpdate(s16 s16X, s16 s16Y, u8 u8Rot);

void ViewTransform(const s16x3 *pPosition);

#endif //VIEW_H
