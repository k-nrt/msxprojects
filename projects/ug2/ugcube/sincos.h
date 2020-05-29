#ifndef SINCOS_H
#define SINCOS_H

#include <msx-types.h>

typedef s8 fp2_6;       //. fixed point 1:1:6
typedef s16 fp2_14;     //. fixed point 1:1:14

//. fixed point sincos  1:1:6
typedef u16 SinCosLp;
#define SinCos_GetSin(v) ((s8)v>>8)
#define SinCos_GetCos(v) ((s8)v)

//. signed vec2 1:7:0
typedef u16 s8XY;
#define s8XY_GetX(xy) (s8)(xy>>8)
#define s8XY_GetY(xy) (s8)(xy)

//. get sin/cos low precision
extern SinCosLp SinCos_GetSinCosLp(u8 rot);
extern fp2_6 SinCos_GetSinLp(u8 rot);
extern fp2_6 SinCos_GetCosLp(u8 rot);

//. get sin/cos high precision
extern fp2_14 SinCos_GetSinHp(u8 rot);
extern fp2_14 SinCos_GetCosHp(u8 rot);

//. rotate xy low precision
extern s8XY SinCos_RotateXYS8Lp(s8 x, s8 y, u8 rot);

#endif //SINCOS_H