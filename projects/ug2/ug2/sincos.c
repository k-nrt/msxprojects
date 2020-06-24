#include "sincos.h"
#include "macros.h"
#include "math.h"
//const s16 g_s16SinMul0[64][16];
//const s16 g_s16SinMul1[64][16];
//const s16 g_s16SinMul2[64][16];
//const s16 g_s16SinMul3[64][16];



SDCC_FIXED_ADDRESS(0x7f00) const s16 g_s16SinTablePlus[] =
{
	(s16)0x0000,(s16)0x0007,(s16)0x000D,(s16)0x0013,(s16)0x001A,(s16)0x0020,(s16)0x0026,(s16)0x002C,
	(s16)0x0032,(s16)0x0039,(s16)0x003F,(s16)0x0045,(s16)0x004B,(s16)0x0051,(s16)0x0057,(s16)0x005D,
	(s16)0x0062,(s16)0x0068,(s16)0x006E,(s16)0x0074,(s16)0x0079,(s16)0x007F,(s16)0x0084,(s16)0x0089,
	(s16)0x008F,(s16)0x0094,(s16)0x0099,(s16)0x009E,(s16)0x00A3,(s16)0x00A8,(s16)0x00AC,(s16)0x00B1,
	(s16)0x00B6,(s16)0x00BA,(s16)0x00BE,(s16)0x00C2,(s16)0x00C6,(s16)0x00CA,(s16)0x00CE,(s16)0x00D2,
	(s16)0x00D5,(s16)0x00D9,(s16)0x00DC,(s16)0x00DF,(s16)0x00E2,(s16)0x00E5,(s16)0x00E8,(s16)0x00EB,
	(s16)0x00ED,(s16)0x00EF,(s16)0x00F2,(s16)0x00F4,(s16)0x00F5,(s16)0x00F7,(s16)0x00F9,(s16)0x00FA,
	(s16)0x00FC,(s16)0x00FD,(s16)0x00FE,(s16)0x00FF,(s16)0x00FF,(s16)0x0100,(s16)0x0100,(s16)0x0100,
};

SDCC_FIXED_ADDRESS(0x7f80) const s  16 g_s16SinTableMinus[] =
{
	(s16)0x0000,(s16)0xFFF9,(s16)0xFFF3,(s16)0xFFED,(s16)0xFFE6,(s16)0xFFE0,(s16)0xFFDA,(s16)0xFFD4,
	(s16)0xFFCE,(s16)0xFFC7,(s16)0xFFC1,(s16)0xFFBB,(s16)0xFFB5,(s16)0xFFAF,(s16)0xFFA9,(s16)0xFFA3,
	(s16)0xFF9E,(s16)0xFF98,(s16)0xFF92,(s16)0xFF8C,(s16)0xFF87,(s16)0xFF81,(s16)0xFF7C,(s16)0xFF77,
	(s16)0xFF71,(s16)0xFF6C,(s16)0xFF67,(s16)0xFF62,(s16)0xFF5D,(s16)0xFF58,(s16)0xFF54,(s16)0xFF4F,
	(s16)0xFF4A,(s16)0xFF46,(s16)0xFF42,(s16)0xFF3E,(s16)0xFF3A,(s16)0xFF36,(s16)0xFF32,(s16)0xFF2E,
	(s16)0xFF2B,(s16)0xFF27,(s16)0xFF24,(s16)0xFF21,(s16)0xFF1E,(s16)0xFF1B,(s16)0xFF18,(s16)0xFF15,
	(s16)0xFF13,(s16)0xFF11,(s16)0xFF0E,(s16)0xFF0C,(s16)0xFF0B,(s16)0xFF09,(s16)0xFF07,(s16)0xFF06,
	(s16)0xFF04,(s16)0xFF03,(s16)0xFF02,(s16)0xFF01,(s16)0xFF01,(s16)0xFF00,(s16)0xFF00,(s16)0xFF00,
};

#if 0
s16 SinCos_GetSin(u8 rot)
{
	if (rot < 128)
	{
		if (rot < 64)
		{
			return g_s16SinTablePlus[rot];
		}
		else
		{
			return g_s16SinTablePlus[127 - rot];
		}
	}
	else
	{
		if (rot < 192)
		{
			return g_s16SinTableMinus[rot - 128];
		}
		else
		{
			return g_s16SinTableMinus[255 - rot];
		}
	}

	return 0;
}
#endif

s16 SinCos_GetCos(u8 u8Rot)
{
	register u8 rot = u8Rot;
	if (rot < 128)
	{
		if (rot < 64)
		{
			return g_s16SinTablePlus[63 - rot];
		}
		else
		{
			return g_s16SinTableMinus[rot - 64];
		}
	}
	else
	{
		if (rot < 192)
		{
			return g_s16SinTableMinus[191 - rot];
		}
		else
		{
			return g_s16SinTablePlus[rot - 192];
		}
	}
}

s16 SinCos_MulS8x256(s8 s8Value, s16 s16Sin)
{
	register s8 value = s8Value;
	register s16 sin = s16Sin;

	if (sin == 0x0000)
	{
		return 0;
	}

	if (sin & 0x8000)
	{
		sin = -sin;
		if (sin == 0x0100)
		{
			return ((s16)-value) << 8;
		}

		if (value & 0x80)
		{
			return (s16)Math_MulU8xU8_U16((u8)-value, (u8)sin);
		}
		else
		{
			return -(s16)Math_MulU8xU8_U16((u8)value, (u8)sin);
		}
	}
	else
	{
		if (sin == 0x0100)
		{
			return ((s16)value) << 8;
		}

		if (value & 0x80)
		{
			return -(s16)Math_MulU8xU8_U16((u8)-value, (u8)sin);
		}
		else
		{
			return (s16)Math_MulU8xU8_U16((u8)value, (u8)sin);
		}
	}
}
