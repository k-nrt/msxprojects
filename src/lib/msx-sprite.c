//-----------------------------------------------------------------------------
//. ���Ղ炢��.
//-----------------------------------------------------------------------------
#include "msx-bios-wrapper.h"
#include "msx-sprite.h"

//-----------------------------------------------------------------------------
//. �X�v���C�g���[�h�̂ݐݒ�.
//-----------------------------------------------------------------------------
void msxSpriteSetMode( enum ESpriteMode eMode )
{
	msxBiosWriteVDP( 1, ( MSX_VDPREG(1) & 0xfc ) | (u8) eMode );
}

//-----------------------------------------------------------------------------
//. �p�^�[���]��.
//-----------------------------------------------------------------------------
void msxSpriteSetPattern( u8 nPattern, u8 *pPattern )
{
	msxBiosCopyRAMtoVRAM( (u16) pPattern, 0x3800 + ((u16)nPattern)*32, 32 );
}

//-----------------------------------------------------------------------------
//. �v���[���ݒ�.
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
