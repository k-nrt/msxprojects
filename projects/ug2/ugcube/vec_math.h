#ifndef VEC_MATH_H
#define VEC_MATH_H

#include <msx-types.h>
#include "sincos.h"

typedef struct
{
	s16 x, y, z;
} s16x3;

#define s16x3Set(_v,_x,_y,_z) _v.x=_x;_v.y=_y;_v.z=_z
#define s16x3Copy(_v,_r) _v.x=_r.x;_v.y=_r.y;_v.z=_r.z
#define s16x3Op(_r,_a,_op,_b) _r.x=_a.x _op _b.x;_r.y=_a.y _op _b.y;_r.z=_a.z _op _b.z

typedef struct
{
	s8 x, y, z;
} s8x3;

#define s8x3Set(_v,_x,_y,_z) _v.x=_x;_v.y=_y;_v.z=_z
#define s8x3Copy(_v,_r) _v.x=_r.x;_v.y=_r.y;_v.z=_r.z
#define s8x3Op(_r,_a,_op,_b) _r.x=_a.x _op _b.x;_r.y=_a.y _op _b.y;_r.z=_a.z _op _b.z

#define s8x3_RotateX(d,s,rot) {s8XY xy=SinCos_RotateXYS8Lp(s.y,s.z,rot); d.y=s8XY_GetX(xy); d.z=s8XY_GetY(xy);}
#define s8x3_RotateY(d,s,rot) {s8XY xy=SinCos_RotateXYS8Lp(s.z,s.x,rot); d.z=s8XY_GetX(xy); d.x=s8XY_GetY(xy);}
#define s8x3_RotateZ(d,s,rot) {s8XY xy=SinCos_RotateXYS8Lp(s.x,s.y,rot); d.x=s8XY_GetX(xy); d.y=s8XY_GetY(xy);}

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

