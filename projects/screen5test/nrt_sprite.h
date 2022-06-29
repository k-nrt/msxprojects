#ifndef NRT_SPRITE_H
#define NRT_SPRITE_H

#include <msx-types.h>

struct NrtSpriteAttribute
{
	u8 y, x, pattern, color;
};

struct NrtSpriteColor
{
	u8 colors[16];
};

#define NRT_SPRITE_MAX (32)

extern struct NrtSpriteAttribute g_nrtSprites[NRT_SPRITE_MAX];
extern struct NrtSpriteColor     g_nrtSpriteColors[NRT_SPRITE_MAX];

extern void NrtSpriteInitialize();

extern void NrtSpriteSetPattern(u8 i, u8* pPatterns);

extern void NrtSpriteSetAttribute( u8 i, s16 x, u8 y, u8 pattern);
extern void NrtSpriteSetColor(u8 i, u8 color);
extern void NrtSpriteSetColors(u8 i, const u8* colors);

extern void NrtSpriteUpdate();

#endif //NRT_SPRITE_H
