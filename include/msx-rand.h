//-----------------------------------------------------------------------------
//. ‚ç‚ñ‚·‚¤.
//-----------------------------------------------------------------------------
#ifndef __MSX_RAND_H__
#define __MSX_RAND_H__

#include <msx-types.h>

extern void msxRandInit( u8 nSeed8, u16 nSeed16 );

extern u8 msxRandGet8(void);
extern u16 msxRandGet16(void);

#endif //__MSX_RAND_H__
