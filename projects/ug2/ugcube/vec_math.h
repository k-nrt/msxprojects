#ifndef VEC_MATH_H
#define VEC_MATH_H

#include <msx-types.h>

typedef struct
{
	s16 x, y, z;
} s16x3;

typedef struct
{
	s8 x, y, z;
} s8x3;

#define Vec3Set(_v,_x,_y,_z) _v.x=_x;_v.y=_y;_v.z=_z
#define Vec3Copy(_v,_r) _v.x=_r.x;_v.y=_r.y;_v.z=_r.z
#define Vec3Op(_r,_a,_op,_b) _r.x=_a.x _op _b.x;_r.y=_a.y _op _b.y;_r.z=_a.z _op _b.z

void S8x3_RotateX(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot);
void S8x3_RotateY(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot);
void S8x3_RotateZ(s8x3 *pResult, const s8x3 *pSrc, u8 u8Rot);

void S8x3_RotateXYZ(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);
void S8x3_RotateXZY(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);

void S8x3_RotateYZX(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);
void S8x3_RotateYXZ(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);

void S8x3_RotateZXY(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);
void S8x3_RotateZYX(s8x3 *pResult, const s8x3 *pSrc, const s8x3 *pRot);

enum ERotationOrder
{
	kRotationOrder_XYZ,
	kRotationOrder_XZY,

	kRotationOrder_YZX,
	kRotationOrder_YXZ,

	kRotationOrder_ZXY,
	kRotationOrder_ZYX,
};

void S8x3_Rotate(s8x3 *pResult, const s8x3 *pSrc, enum ERotationOrder eRotationOrder, const s8x3 *pRot);



#endif //VEC_MATH_H

