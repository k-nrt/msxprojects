#include <msx-bios-wrapper.h>
#include "nrt_sprite.h"

struct NrtSpriteAttribute g_nrtSprites[NRT_SPRITE_MAX];
struct NrtSpriteColor     g_nrtSpriteColors[NRT_SPRITE_MAX];

#define COLOR_EC_BIT (0x80)

static u8 s_frame = 0;

void NrtSpriteInitialize()
{
	u8 i,j;
	struct NrtSpriteAttribute *pAttribute = g_nrtSprites;
	struct NrtSpriteColor *pColor = g_nrtSpriteColors;
	for (i = 0; i < NRT_SPRITE_MAX; i++, pAttribute++, pColor++ )
	{
		pAttribute->y = 217;
		pAttribute->x = 0;
		pAttribute->pattern = 0;
		pAttribute->color = 0;

		for (j = 0; j < 16; j++)
		{
			pColor->colors[j] = 0;
			pColor->colors[j] = 0;
			pColor->colors[j] = 0;
			pColor->colors[j] = 0;
		}
	}
}

void NrtSpriteSetPattern(u8 iSprite, u8* pPattern)
{
	msxBiosCopyRAMtoVRAM((u16)pPattern, 0x7800 + (((u16)iSprite) << 5), 32);
}

void NrtSpriteSetAttribute(u8 iSprite, s16 x, u8 y, u8 pattern)
{
	struct NrtSpriteAttribute *pAttribute = &g_nrtSprites[iSprite];
	struct NrtSpriteColor *pColor = &g_nrtSpriteColors[iSprite];

	if (x < -31 || ( 211 < y && y < 224 )  )
	{
		pAttribute->y = 217;
	}
	else if (x < 0)
	{
		pAttribute->y = y;
		pAttribute->x = ((u8)(x+32));
		pAttribute->pattern = pattern;

		if ( !(pAttribute->color & COLOR_EC_BIT) )
		{
			u8 i;
			pAttribute->color |= COLOR_EC_BIT;
			for (i = 0; i < 16; i++)
			{
				//. set EC bit.
				pColor->colors[i] |= COLOR_EC_BIT;
			}
		}
	}
	else if (x < 256)
	{
		pAttribute->y = y;
		pAttribute->x = ((u8)x);
		pAttribute->pattern = pattern;

		if (pAttribute->color & COLOR_EC_BIT)
		{
			u8 i;
			pAttribute->color &= ~COLOR_EC_BIT;
			for (i = 0; i < 16; i++)
			{
				//. reset EC bit.
				pColor->colors[i] &= ~COLOR_EC_BIT;
			}
		}
	}
	else
	{
		pAttribute->y = 217;
	}
}

void NrtSpriteSetColor(u8 iSprite, u8 color)
{
	u8 i;
	struct NrtSpriteColor *pColor = &g_nrtSpriteColors[iSprite];
	u8 u8ColorBits = (g_nrtSprites[iSprite].color & COLOR_EC_BIT) | color;

	for (i = 0; i < 16; i++)
	{
		pColor->colors[i] = u8ColorBits;
	}
}

void NrtSpriteSetColors(u8 iSprite, const u8* colors)
{
	u8 i;
	struct NrtSpriteColor *pColor = &g_nrtSpriteColors[iSprite];
	u8 u8ColorBits = g_nrtSprites[iSprite].color & COLOR_EC_BIT;

	for (i = 0; i < 16; i++)
	{
		pColor->colors[i] = u8ColorBits | colors[i];
	}
}

static void SendSprite0123();
static void SendSprite3210();
static void SendSprite2301();
static void SendSprite1032();

void NrtSpriteUpdate()
{
	/*
	const void(*s_pSendSprite[])() =
	{
		SendSprite0123,
		SendSprite3210,
		SendSprite2301,
		SendSprite1032
	};
	s_pSendSprite[s_frame & 0x3]();
	*/

	switch (s_frame & 0x3)
	{
	case 0:
		SendSprite0123();
		break;
	case 1:
		SendSprite3210();
		break;
	case 2:
		SendSprite2301();
		break;
	case 3:
		SendSprite1032();
		break;
	}
	s_frame++;
}

void SendSprite0123()
{
	msxBiosCopyRAMtoVRAM((u16)g_nrtSpriteColors, 0x7400, 512);
	msxBiosCopyRAMtoVRAM((u16)g_nrtSprites, 0x7600, 128);

	msxBiosWriteVDP(5, 0x77ff >> 7);
	msxBiosWriteVDP(11, 0x7600 >> 15);
}

void SendSprite3210()
{
	__asm
		push	ix
		ld	de, #0x6400
		ld	hl, #_g_nrtSpriteColors + 24 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6480
		ld	hl, #_g_nrtSpriteColors + 16 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6500
		ld	hl, #_g_nrtSpriteColors + 8 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6580
		ld	hl, #_g_nrtSpriteColors + 0 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6600
		ld	hl, #_g_nrtSprites + 24 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6620
		ld	hl, #_g_nrtSprites + 16 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6640
		ld	hl, #_g_nrtSprites + 8 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6660
		ld	hl, #_g_nrtSprites + 0 * 4
		ld	bc, #32
		call #0x005c
		pop		ix
	__endasm;

	msxBiosWriteVDP(5, 0x67ff >> 7);
	msxBiosWriteVDP(11, 0x6600 >> 15);
}

void SendSprite2301()
{
	__asm
	push	ix
		ld	de, #0x7400
		ld	hl, #_g_nrtSpriteColors + 16 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x7480
		ld	hl, #_g_nrtSpriteColors + 24 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x7500
		ld	hl, #_g_nrtSpriteColors + 0 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x7580
		ld	hl, #_g_nrtSpriteColors + 8 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x7600
		ld	hl, #_g_nrtSprites + 16 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x7620
		ld	hl, #_g_nrtSprites + 24 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x7640
		ld	hl, #_g_nrtSprites + 0 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x7660
		ld	hl, #_g_nrtSprites + 8 * 4
		ld	bc, #32
		call #0x005c
		pop		ix
	__endasm;

	msxBiosWriteVDP(5, 0x77ff >> 7);
	msxBiosWriteVDP(11, 0x7600 >> 15);
}

void SendSprite1032()
{
	__asm
	push	ix
		ld	de, #0x6400
		ld	hl, #_g_nrtSpriteColors + 8 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6480
		ld	hl, #_g_nrtSpriteColors + 0 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6500
		ld	hl, #_g_nrtSpriteColors + 24 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6580
		ld	hl, #_g_nrtSpriteColors + 16 * 16
		ld	bc, #128
		call #0x005c

		ld	de, #0x6600
		ld	hl, #_g_nrtSprites + 8 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6620
		ld	hl, #_g_nrtSprites + 0 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6640
		ld	hl, #_g_nrtSprites + 24 * 4
		ld	bc, #32
		call #0x005c

		ld	de, #0x6660
		ld	hl, #_g_nrtSprites + 16 * 4
		ld	bc, #32
		call #0x005c
		pop		ix
	__endasm;

	msxBiosWriteVDP(5, 0x67ff >> 7);
	msxBiosWriteVDP(11, 0x6600 >> 15);
}

