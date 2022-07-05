#ifndef SINCOS_H
#define SINCOS_H

#include <msx-types.h>

s16 SinCos_GetSin(u8 rot) SDCCCALL(0);
s16 SinCos_GetCos(u8 rot);

s16 SinCos_MulS8x256(s8 s8Value, s16 s16Sin);

#define SinCos_RotateXS8x256(_x,_y,_sin,_cos) (SinCos_MulS8x256(_x,_cos)-SinCos_MulS8x256(_y,_sin))
#define SinCos_RotateYS8x256(_x,_y,_sin,_cos) (SinCos_MulS8x256(_x,_sin)+SinCos_MulS8x256(_y,_cos))

#endif //SINCOS_H