#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>
#include "nrt_sprite.h"

#include "hmmm.h"

void WriteString(u16 x, u16 y, char *pString);
void DrawBg(u16 pos);
void BgUpdate(u16 pos);

#define VDP8(_r) *((u8*)(0xF3DF+_r))
#define VDP24(_r) *((u8*)(0xFFE7+_r-8))
#define VDP(_r) ((_r<8)?VDP8(_r):VDP24(_r))


u8 ReadVDP(u8 r)
{
	if (r < 8)
	{
		volatile u8 *pR0 = ((u8*)0xF3DF) + (u16)r;
		return *pR0;
	}
	else if (r < 24)
	{
		volatile u8 *pR8 = ((u8*)0xFFE7) + (u16)(r-8);
		return *pR8;
	}
	else
	{
		return 0;
	}
}

const u8 g_u8SpritePattern[] =
{
	0x03, 0x03, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F, 0xAB, 0xA5, 0x8B, 0x40, 0x40, 0x20, 0x15, 0x0E, 0x03, 0xC0, 0xC0, 0xF0, 0xF8, 0xFC, 0xF8, 0xFC, 0xC5, 0xA3, 0xC5, 0x0A, 0x16, 0xAC, 0x58, 0xB0, 0xC0,
	0x00, 0x0E, 0x1D, 0x3E, 0x7F, 0x7E, 0xDD, 0x7C, 0x5B, 0x7C, 0x3F, 0x3F, 0x1F, 0x0A, 0x01, 0x00, 0x00, 0x30, 0x08, 0x84, 0x42, 0x86, 0x03, 0x3A, 0xDC, 0x3A, 0xF4, 0xE8, 0x50, 0xA0, 0x40, 0x00,
};


const struct NrtSpriteColor g_spriteColors[] =
{
	{ { 0x01, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x0E, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 } },
	{ { 0x60, 0x61, 0x61, 0x61, 0x61, 0x61, 0x61, 0x6E, 0x68, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x6E, 0x60 } },
};



void WriteVDP(u8 u8Register, u8 u8Value)
{
	if (u8Register < 8)
	{
		VDP8(u8Register) = u8Value;
	}
	else
	{
		VDP24(u8Register) = u8Value;
	}
	__asm
		RDVDP = 0x0006
		WRVDP = 0x0007

		ld		hl, #3
		add		hl, sp

		ld		a, (WRVDP)
		ld		c, a
		inc		c			//. c: = PORT's address
		ld      a, (hl)  //. u8Value.
		out(c), a

		ld		a, #0x80
		dec		hl
		or		a, (hl)
		out(c), a			// R#0 : = u8Value

		__endasm;
}

u8 ReadVDPStatus0()
{
	__asm
	ld		a, (WRVDP)
		inc		a
		ld		d, a

		ld		a, (RDVDP)
		inc		a
		ld		e, a

		ld      h, #0
		ld		l, #0x8f
		ld		b, #0

		// Read S2
		// R15 = 2
		ld		c, d
		out (c), h
		out (c), l

		ld		c, e
		nop
		in      a, (c)

		// Crear R15
		// R15 = 0
		ld		c, d
		out (c), b
		out (c), l

		ld l, a

		__endasm;
}


u8 ReadVDPStatus1()
{
	__asm
		ld		a, (WRVDP)
		inc		a
		ld		d, a

		ld		a, (RDVDP)
		inc		a
		ld		e, a

		ld      h, #1
		ld		l, #0x8f
		ld		b, #0

		// Read S2
		// R15 = 2
		ld		c, d
		out		(c), h
		out		(c), l

		ld		c, e
		nop
		in      a, (c)

		// Crear R15
		// R15 = 0
		ld		c, d
		out		(c),b
		out		(c),l

		ld l,a

	__endasm;
}



unsigned short g_hTIMISave = 0;
void *g_pHTIMI = (void*)(0xfd9f+1);
void OnTimer();
void HSYNC()
{
	WriteVDP(1, VDP(1) & (~0x40));

	WriteVDP(19, 0);
	WriteVDP(0, VDP(0) & (~0x10));
	WriteVDP(1, VDP(1) & (~0x01));

	//*((unsigned short*)0xfda0) = g_hTIMISave;
	//g_pHTIMI = OnTimer;
}




volatile u16 g_u16Timer = 0;
void OnTimer()
{
	__asm
		di
	__endasm;

	//if (VDP(19)==64)
		if (ReadVDPStatus1() & 1)
	{
		HSYNC();
	}
	else
	{
		WriteVDP(1, VDP(1) | 0x40);
		//	msxBiosWriteVDP(8, ReadVDP(8) | 0x02);
		//	NrtSpriteUpdate();
		//	msxBiosWriteVDP(8, ReadVDP(8) & (~0x02));

		//g_hTIMISave = *((unsigned short*)0xfda0);
		//*((unsigned short*)0xfda0) = (unsigned short)HSYNC;

		ReadVDPStatus0();

		//. HSYNC.
		WriteVDP(19, 64);
		WriteVDP(0, VDP(0) | 0x10);
		//msxBiosWriteVDP(0, ReadVDP(0) | 0x10);

		
		WriteVDP(1, VDP(1) | (0x01));

		//NrtSpriteUpdate();
		g_u16Timer++;
	}
	__asm
		ei
	__endasm;
}


void WaitVSYNC()
{
	volatile u16 u16Timer = g_u16Timer;
	for (; g_u16Timer == u16Timer;);
}

void Copy(void *pDst, const void *pSrc, u16 size)
{
	UNUSED(pDst);
	UNUSED(pSrc);
	UNUSED(size);
	//. ‚±‚±‚É
	//. push ix
	//. add ix,sp
	//. ‚ª“ü‚é.
	__asm
		ld		e, 4(ix)
		ld		d, 5(ix)
		ld		l, 6(ix)
		ld		h, 7(ix)
		ld		c, 8(ix)
		ld		b, 9(ix)

		ldir
	__endasm;
		/*
	u8 *pD = (u8*)pDst;
	u8 *pS = (u8*)pSrc;
	u16 i;
	for (i = 0; i < size; i++, pS++, pD++)
	{
		*pD = *pS;
	}
	*/
}

typedef struct
{
	s16 x;
	s16 y;
	s8 vx, vy;
	u8 pad0, pad1;
} SBall;

SBall g_balls[16];

void BallInit()
{
	u8 i;
	for (i = 0; i < 16; i++)
	{
		g_balls[i].x = msxRandGet8();
		g_balls[i].y = msxRandGet8();

		switch (msxRandGet8()&0x3)
		{
		case 0:
			g_balls[i].vx = 1;
			g_balls[i].vy = 1;
			break;

		case 1:
			g_balls[i].vx = -1;
			g_balls[i].vy = 1;
			break;

		case 2:
			g_balls[i].vx = 1;
			g_balls[i].vy = -1;
			break;

		case 3:
			g_balls[i].vx = -1;
			g_balls[i].vy = -1;
			break;

		default:
			break;
		}

		NrtSpriteSetAttribute(i<<1, g_balls[i].x, g_balls[i].y, 0);
		NrtSpriteSetColors(i<<1, g_spriteColors[0].colors );

		NrtSpriteSetAttribute((i << 1)+1, g_balls[i].x, g_balls[i].y, 4);
		NrtSpriteSetColors((i << 1)+1, g_spriteColors[1].colors);
		//g_balls[i].vx = (msxRandGet8()&0x1)-0;
		//g_balls[i].vy = (msxRandGet8()&0x1)-0;
	}
}

void BallUpdate()
{
	u8 i;
	const s16 left = -16;
	const s16 right = 256;
	for (i = 0; i < 16; i++)
	{
		SBall *pBall = &g_balls[i];

		pBall->x += pBall->vx;
		if (pBall->x < left )
		{
			pBall->x = left;
			pBall->vx = -pBall->vx;
		}
		else if (pBall->x > right )
		{
			pBall->x = right;
			pBall->vx = -pBall->vx;
		}

		pBall->y += pBall->vy;
		if (pBall->y < -8 )
		{
			pBall->y = -8;
			pBall->vy = -pBall->vy;
		}
		else if (pBall->y > (212-8))
		{
			pBall->y = (212 - 8);
			pBall->vy = -pBall->vy;
		}

//		g_sprites[i].x = pBall->x;
//		g_sprites[i].y = pBall->y;

		NrtSpriteSetAttribute(i<<1, pBall->x, pBall->y, 0);
		NrtSpriteSetAttribute((i<<1)+1, pBall->x, pBall->y, 4);
	}
}



void main(void)
{
	u8 y;
	u8 nColor;
	UNUSED(nColor);

	msx2BiosChangeModePalette(5);

#if 0
	for (y = 0; y < 24; y++)
	{
		WriteString(0, ((u16)y) << 3, "Hello World!!");
	}
#else
	WriteString(0, 0, "Hello World!!");
	for (y = 0; y < 24; y++)
	{
		HMMM(0, 0, 13 * 8, 8, 0, ((u16)y + 1) * 8);
		WaitCommand();
	}

	for (y = 0; y < 255; y++)
	{
		volatile u16 *pGRPACX = (u16*)0xFCB7;
		volatile u16 *pGRPACY = (u16*)0xFCB9;

		*pGRPACX = (y & 0x1f) << 3;
		*pGRPACY = (y & 0xe0) >> 2;

		msx2BiosGraphicPrint(y);
	}

	DrawBg(0);

#endif

	msxBiosWriteVDP(1, (ReadVDP(1) & 0xf6) | 0x02 );
	msxBiosWriteVDP(8, (ReadVDP(8) & 0xfd));

	msxTimerSetCallBack(OnTimer);

	msxRandInit(0x45, 0xdead);

	NrtSpriteInitialize();

	BallInit();
	

	msx2BiosWriteVRAM(0x0, 0xff);

	//msxBiosCopyRAMtoVRAM((u16)g_u8SpritePattern, 0x7800, 32);

	NrtSpriteSetPattern(0, g_u8SpritePattern);
	NrtSpriteSetPattern(1, &g_u8SpritePattern[32] );

#if 0
	for (y = 0; y < 32; y++)
	{
		NrtSpriteSetColors(y, g_spriteColor[0].colors);
		NrtSpriteSetColors(y, g_spriteColor[0].colors);
		//Copy( g_spriteColors[y].colors, g_spriteColor.colors, 16);
		
		/*
		s8 i;
		for (i = 0; i < 16; i++)
		{
			g_spriteColors[y].colors[i] = g_spriteColor.colors[i];
		}
		*/
	}
#endif

	for (;;)
	{
		static u16 pos = 0;
		u8 s = 0;
		msxBiosChangeColor(1, 15, 4, 0x8);
		s = msxBiosGetStick(0);
		if (s == 3 && pos < 96)
		{
			pos++;
//			DrawBg(pos);
		}
		else if (s == 7 && pos > 0)
		{
			pos--;
//			DrawBg(pos);
		}
		/*
		for( nColor = 0 ; nColor < 16 ; nColor++ )
		{
			msxBiosChangeColor( 1, 15, 4, nColor );
		}
		*/

		BgUpdate(pos);

		
		BallUpdate();

		msxBiosChangeColor(1, 15, 4, 0x7);

		WaitVSYNC();
		msxBiosChangeColor(1, 15, 4, 0xf);
		NrtSpriteUpdate();

		//msxBiosWriteVDP(8, ReadVDP(8) | 0x02);
//		NrtSpriteUpdate();
		//msxBiosWriteVDP(8, ReadVDP(8) & (~0x02));
	}
}

void WriteString( u16 x, u16 y, char *pString )
{
	volatile u16 *pGRPACX = (u16*)0xFCB7;
	volatile u16 *pGRPACY = (u16*)0xFCB9;

	*pGRPACX = x;
	*pGRPACY = y;

	for (; *pString != 0; pString++)
	{
		msx2BiosGraphicPrint(*pString);
	}
}

const char g_map[] =

	"@@@@@00000@@@@@00000@@@@@0000012-----|||||-----|||||-----|||||12aaaaabbbbbaaaaabbbbbaaaaabbbbb12aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"       ########    ##############3                           o           !!!####                                                "
	"         #######      ############                           o             !!#        |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"                             ########         O       o      o              !         |                                         "
	"                                             ooo     ooo     o       !!##             |                                         "
	"      ##########                              o       o      o      !!!!##        ||  |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"  [++][++][++][++][++][++]                   o o     o o     o      !!!!!###     ||   |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"[++][++][++][++][++][++][++]{}{}{}{}{}{}{}{}{}()()()()()()()()()()()(){}{}{}{}{}{}{}{}{()()()()(aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"@@@@@00000@@@@@00000@@@@@0000012-----|||||-----|||||-----|||||12aaaaabbbbbaaaaabbbbbaaaaabbbbb12aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"       ########    ##############3                           o           !!!####                                                "
	"         #######      ############                           o             !!#        |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"                             ########         O       o      o              !         |                                         "
	"                                             ooo     ooo     o       !!##             |                                         "
	"      ##########                              o       o      o      !!!!##        ||  |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"  [++][++][++][++][++][++]                   o o     o o     o      !!!!!###     ||   |         aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
	"[++][++][++][++][++][++][++]{}{}{}{}{}{}{}{}{}()()()()()()()()()()()(){}{}{}{}{}{}{}{}{()()()()(aaaaabbbbbaaaaabbbbbaaaaabbbbb12"
;

void DrawBg( u16 pos )
{
	u8 x, y;
	const char *p = g_map;
	const char *pPrev = g_map;
	static u16 prev = 0;

	p += pos;
	pPrev += prev;

	for (y = 0; y < 16; y++,p+=128-32,pPrev+=128-32)
	{
		for (x = 0; x < 32; x++,p++,pPrev++)
		{
			u8 c = *p;
			u8 cp = *pPrev;
			if (c != cp)
			{
				u8 u = (c & 0x1f) << 3;
				u8 v = (c & 0xe0) >> 2;

				HMMM(u, v, 8, 8, x << 3, 64 + (y << 3));
			}
		}
	}

	prev = pos;
	
}

void BgUpdate( u16 newpos )
{
	u8 x, y;
	static const char *p = g_map;
	static const char *pPrev = g_map;
	static u16 pos = 0;
	static u16 prev = 0;
	static u8 phase = 0;
	UNUSED(newpos);

	if (phase == 0)
	{
		//pos = newpos;
		p = &g_map[0] + pos;
		pPrev = &g_map[0] + prev;
	}

	for (y = phase; y < phase+1; y++, p += 128 - 32, pPrev += 128 - 32)
	{
		for (x = 0; x < 32; x++, p++, pPrev++)
		{
			u8 c = *p;
			u8 cp = *pPrev;
			if (c != cp)
			{
				u8 u = (c & 0x1f) << 3;
				u8 v = (c & 0xe0) >> 2;

				WaitCommand();
				HMMM(u, v, 8, 8, x << 3, 64 + (y << 3));
			}
		}
	}

	phase += 1;
	if (phase >= 16)
	{
		prev = pos;
		phase = 0;
		pos++;
		if (pos >= 96)
		{
			pos = 0;
		}
	}
}
