#ifndef __XSP_H__
#define __XSP_H__

#include <msx-types.h>

extern void SpbInit(void);
extern void SpbSetSprite(u8 iPlane, s16 x, s16 y, u8 iPattern, u8 nColor);
extern void SpbUpdate(void);

extern void SpbSendSprite( u16 pSpriteAttribute );

#endif //__XSP_H__
