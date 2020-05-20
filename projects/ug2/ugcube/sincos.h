#ifndef SINCOS_H
#define SINCOS_H

#include <msx-types.h>

typedef s8 fp2_6;
typedef s16 fp2_14;
typedef u16 SinCosLp;

extern SinCosLp SinCos_GetSinCosLp(u8 rot);
extern fp2_6 SinCos_GetSinLp(u8 rot);
extern fp2_6 SinCos_GetCosLp(u8 rot);

extern fp2_14 SinCos_GetSinHp(u8 rot);
extern fp2_14 SinCos_GetCosHp(u8 rot);


extern s16 SinCos_MulS16Hp(s16 x, fp2_14 y);

extern s8 SinCos_RotateXS8Lp(s8 x, s8 y, SinCosLp sincos);
extern s8 SinCos_RotateYS8Lp(s8 x, s8 y, SinCosLp sincos);

#define SinCos_RotateXS8x256(_x,_y,_sin,_cos) (SinCos_MulS8x256(_x,_cos)-SinCos_MulS8x256(_y,_sin))
#define SinCos_RotateYS8x256(_x,_y,_sin,_cos) (SinCos_MulS8x256(_x,_sin)+SinCos_MulS8x256(_y,_cos))

#endif //SINCOS_H