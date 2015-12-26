#include "math.h"

s32 Math_MulS16xS16_S32(s16 x, s16 y)
{
	if (x & 0x8000)
	{
		if (y & 0x8000)
		{
			return (s32)Math_MulU16xU16_U32((u16)-x, (u16)-y);
		}
		else
		{
			return -(s32)Math_MulU16xU16_U32((u16)-x, (u16)y);
		}
	}
	else
	{
		if (y & 0x8000)
		{
			return -(s32)Math_MulU16xU16_U32((u16)x, (u16)-y);
		}
		else
		{
			return (s32)Math_MulU16xU16_U32((u16)x, (u16)y);
		}
	}
}
