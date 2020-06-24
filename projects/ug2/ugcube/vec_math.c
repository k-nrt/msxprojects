#include "sincos.h"
#include "vec_math.h"

static s8x3 s_s8x3RotateX;
static s8x3 s_s8x3RotateY;
static s8x3 s_s8x3RotateZ;

#define S8x3_RotateXInternal(_x,_y,_z,_rot) \
{ \
	register s16 sin = SinCos_GetSin(_rot); \
	register s16 cos = SinCos_GetCos(_rot); \
	s_s8x3RotateX.x = _x; \
	s_s8x3RotateX.y = SinCos_RotateXS8x256(_y,_z, sin, cos) >> 8; \
	s_s8x3RotateX.z = SinCos_RotateYS8x256(_y,_z, sin, cos) >> 8; \
}

#define S8x3_RotateYInternal(_x,_y,_z,_rot) \
{ \
	register s16 sin = SinCos_GetSin(_rot); \
	register s16 cos = SinCos_GetCos(_rot); \
	s_s8x3RotateY.x = SinCos_RotateYS8x256(_z,_x, sin, cos) >> 8; \
	s_s8x3RotateY.y = _y; \
	s_s8x3RotateY.z = SinCos_RotateXS8x256(_z,_x, sin, cos) >> 8; \
}

#define S8x3_RotateZInternal(_x,_y,_z,_rot) \
{ \
	register s16 sin = SinCos_GetSin(_rot); \
	register s16 cos = SinCos_GetCos(_rot); \
	s_s8x3RotateZ.x = SinCos_RotateXS8x256(_x,_y, sin, cos) >> 8; \
	s_s8x3RotateZ.y = SinCos_RotateYS8x256(_x,_y, sin, cos) >> 8; \
	s_s8x3RotateZ.z = _z; \
}

void S8x3_RotateX(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot)
{
	S8x3_RotateXInternal(pSrc->x, pSrc->y, pSrc->z, u8Rot);
	Vec3Copy((*pResult),s_s8x3RotateX);
}

void S8x3_RotateY(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot)
{
	S8x3_RotateYInternal(pSrc->x, pSrc->y, pSrc->z, u8Rot);
	Vec3Copy((*pResult),s_s8x3RotateY);
}

void S8x3_RotateZ(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot)
{
	S8x3_RotateZInternal(pSrc->x, pSrc->y, pSrc->z, u8Rot);
	Vec3Copy((*pResult),s_s8x3RotateZ);
}

void S8x3_RotateXYZ(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateXInternal(pSrc->x,pSrc->y,pSrc->z, pRot->x);
	S8x3_RotateYInternal(s_s8x3RotateX.x,s_s8x3RotateX.y,s_s8x3RotateX.z, pRot->y);
	S8x3_RotateZInternal(s_s8x3RotateY.x,s_s8x3RotateY.y,s_s8x3RotateY.z, pRot->z);
	Vec3Copy((*pResult), s_s8x3RotateZ);
}

void S8x3_RotateXZY(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateXInternal(pSrc->x, pSrc->y, pSrc->z, pRot->x);
	S8x3_RotateZInternal(s_s8x3RotateX.x, s_s8x3RotateX.y, s_s8x3RotateX.z, pRot->z);
	S8x3_RotateYInternal(s_s8x3RotateZ.x, s_s8x3RotateZ.y, s_s8x3RotateZ.z, pRot->y);
	Vec3Copy((*pResult), s_s8x3RotateY);
}

void S8x3_RotateYZX(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateYInternal(pSrc->x, pSrc->y, pSrc->z, pRot->y);
	S8x3_RotateZInternal(s_s8x3RotateY.x, s_s8x3RotateY.y, s_s8x3RotateY.z, pRot->z);
	S8x3_RotateXInternal(s_s8x3RotateZ.x, s_s8x3RotateZ.y, s_s8x3RotateZ.z, pRot->x);
	Vec3Copy((*pResult), s_s8x3RotateX);
}

void S8x3_RotateYXZ(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateYInternal(pSrc->x, pSrc->y, pSrc->z, pRot->y);
	S8x3_RotateXInternal(s_s8x3RotateY.x, s_s8x3RotateY.y, s_s8x3RotateY.z, pRot->x);
	S8x3_RotateZInternal(s_s8x3RotateX.x, s_s8x3RotateX.y, s_s8x3RotateX.z, pRot->z);
	Vec3Copy((*pResult), s_s8x3RotateZ);
}

void S8x3_RotateZXY(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateZInternal(pSrc->x, pSrc->y, pSrc->z, pRot->z);
	S8x3_RotateXInternal(s_s8x3RotateZ.x, s_s8x3RotateZ.y, s_s8x3RotateZ.z, pRot->x);
	S8x3_RotateYInternal(s_s8x3RotateX.x, s_s8x3RotateX.y, s_s8x3RotateX.z, pRot->y);
	Vec3Copy((*pResult), s_s8x3RotateY);
}

void S8x3_RotateZYX(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot)
{
	S8x3_RotateZInternal(pSrc->x, pSrc->y, pSrc->z, pRot->z);
	S8x3_RotateYInternal(s_s8x3RotateZ.x, s_s8x3RotateZ.y, s_s8x3RotateZ.z, pRot->y);
	S8x3_RotateXInternal(s_s8x3RotateY.x, s_s8x3RotateY.y, s_s8x3RotateY.z, pRot->x);
	Vec3Copy((*pResult), s_s8x3RotateX);
}

void S8x3_Rotate(s8x3 *pResult, const s8x3 *pSrc, enum ERotationOrder eRotationOrder, const s8x3 *pRot)
{
	switch (eRotationOrder)
	{
	case kRotationOrder_XYZ:
		S8x3_RotateXYZ(pResult, pSrc, pRot);
		break;
	case kRotationOrder_XZY:
		S8x3_RotateXZY(pResult, pSrc, pRot);
		break;
	case kRotationOrder_YZX:
		S8x3_RotateYZX(pResult, pSrc, pRot);
		break;
	case kRotationOrder_YXZ:
		S8x3_RotateYXZ(pResult, pSrc, pRot);
		break;
	case kRotationOrder_ZXY:
		S8x3_RotateZXY(pResult, pSrc, pRot);
		break;
	case kRotationOrder_ZYX:
		S8x3_RotateZYX(pResult, pSrc, pRot);
		break;
	default:
		break;
	}
}
