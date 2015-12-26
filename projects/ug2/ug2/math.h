#ifndef MATH_H
#define MATH_H

#include <msx-types.h>

extern u8 Math_MulU8xU8_U8(u8 x, u8 y);

extern u16 Math_MulU8xU8_U16(u8 x, u8 y);
extern s16 Math_MulS8xS8_S16(u8 x, u8 y);

extern u32 Math_MulU8xU16_U32(u8 x, u16 y);

extern s32 Math_MulS8xS16_S32(u8 x, u16 y);

extern u32 Math_MulU16xU16_U32(u16 x, u16 y);

extern s32 Math_MulS16xS16_S32(s16 x, s16 y);

#endif //MATH_H
