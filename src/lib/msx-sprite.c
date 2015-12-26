//-----------------------------------------------------------------------------
//. すぷらいと.
//-----------------------------------------------------------------------------
#include "msx-bios-wrapper.h"
#include "msx-sprite.h"

//-----------------------------------------------------------------------------
//. スプライトモードのみ設定.
//-----------------------------------------------------------------------------
void msxSpriteSetMode( enum ESpriteMode eMode )
{
	msxBiosWriteVDP( 1, ( MSX_VDPREG(1) & 0xfc ) | (u8) eMode );
}

//-----------------------------------------------------------------------------
//. パターン転送.
//-----------------------------------------------------------------------------
void msxSpriteSetPattern( u8 nPattern, u8 *pPattern )
{
	msxBiosCopyRAMtoVRAM( (u16) pPattern, 0x3800 + ((u16)nPattern)*32, 32 );
}

//-----------------------------------------------------------------------------
//. プレーン設定.
//-----------------------------------------------------------------------------
void msxSpriteSetPlane( u8 nPlane, u8 x, u8 y, u8 nPattern, u8 nColor )
{
	u16 nVRAM = 0x1b00 + ((u16)nPlane)*4;
	msxBiosWriteVRAM( nVRAM, y );
	nVRAM++;
	msxBiosWriteVRAM( nVRAM, x );
	nVRAM++;
	msxBiosWriteVRAM( nVRAM, nPattern );
	nVRAM++;
	msxBiosWriteVRAM( nVRAM, nColor );
}
