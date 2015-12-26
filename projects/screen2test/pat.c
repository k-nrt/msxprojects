#include "pat.h"

u8 g_u8PatBuffer = 0;
u16 g_pPatNameTable = 0x1800;

enum 
{
	kLeft = 0,
	kRight = 32,
	kTop   = 0,
	kBottom = 16,
};

void PatClearBuffer()
{
	msxBiosFillVRAM( g_pPatNameTable, 256, 0x1 );
	msxBiosFillVRAM( g_pPatNameTable + 256, 32, 0x2 );
}

void PatBlit( s8 x, s8 y, u8 w, u8 h, const u8 *pPattern )
{
	u8 i;
	static u16 pDst;
	static u16 pSrc;
	u8 stride = w;


	if( ((y+h) <= kTop) || (kBottom <= y) || ((x+w) <= kLeft ) || (kRight <= x) )
	{
		return;
	}

	if( y < kTop )
	{
		//. 上はみ出.
		h = h + y - kTop;
		pDst = g_pPatNameTable + x + (kTop)*32;
		pSrc = (u16) pPattern + (kTop - y)*((u16)stride);
	}

	else if( kBottom < (y+h) )
	{
		//. 下はみ出.
		h = kBottom - y;
		pDst = g_pPatNameTable + y*32 + x;
		pSrc = (u16) pPattern;
	}
	else
	{
		//. はみ出てない.
		pDst = g_pPatNameTable + y*32 + x;
		pSrc = (u16) pPattern;
	}

	if( x < kLeft )
	{
		//. 左はみ出.
		w = w + x - kLeft;
		pDst += kLeft-x;
		pSrc += kLeft-x;
	}
	else if( kRight < (x+w) )
	{
		//. 右はみ出.
		w = kRight - x;
	}

	for( i = 0 ; i < h ; i++ )
	{
		msxBiosCopyRAMtoVRAM( pSrc, pDst, w );
		pSrc += stride;
		pDst += 32;
	}
}

void PatFill( s8 x, s8 y, s8 w, s8 h, const u8 pattern )
{
	s8 i;
	static u16 pDst;

	if( ((y+h) <= kTop) || (kBottom <= y) || ((x+w) <= kLeft ) || (kRight <= x) )
	{
		return;
	}

	if( y < kTop )
	{
		h = h + y - kTop;
		y = kTop;
	}

	if( kBottom <= (y+h) )
	{
		h = kBottom - y;
	}

	if( x < kLeft )
	{
		w = w + x - kLeft;
		x = kLeft;
	}

	if( kRight <= (x+w) )
	{
		w = kRight - x;
	}

	pDst = g_pPatNameTable + y*32 + x;
	for( i = 0 ; i < h ; i++ )
	{
		msxBiosFillVRAM( pDst, w, pattern );
		pDst += 32;
	}
}


void PatFlip()
{
	if( g_u8PatBuffer == 0 )
	{
		msxBiosWriteVDP( 2, 0x18 >> 2 );
		g_pPatNameTable = 0x1c00;
		g_u8PatBuffer = 1;
	}
	else
	{
		msxBiosWriteVDP( 2, 0x1c >> 2 );
		g_pPatNameTable = 0x1800;
		g_u8PatBuffer = 0;
	}
}
