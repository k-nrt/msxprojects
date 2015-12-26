//-----------------------------------------------------------------------------
//. ‚·‚Õ‚ç‚¢‚Æ.
//-----------------------------------------------------------------------------
#ifndef __MSX_SPRITE_H__
#define __MSX_SPRITE_H__

#include <msx-types.h>

enum ESpriteMode
{
	kSpriteMode_8x8x1,
	kSpriteMode_8x8x2,
	kSpriteMode_16x16x1,
	kSpriteMode_16x16x2,
};

extern void msxSpriteSetMode( enum ESpriteMode eMode );
extern void msxSpriteSetPattern( u8 nPattern, u8 *pPattern );
extern void msxSpriteSetPlane( u8 nPlane, u8 x, u8 y, u8 nPattern, u8 nColor );

#endif //__MSX_SPRITE_H__
