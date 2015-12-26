#include "key_matrix.h"
#include "macros.h"

//! OLDKEY(FBDAH, 11)
//! 内容	キーマトリクスの状態(旧)

SDCC_FIXED_ADDRESS(0xFBDA) u8 OLDKEY[11];

//! NEWKEY(FBE5H, 11)
//! 内容	キーマトリクスの状態(新)
SDCC_FIXED_ADDRESS(0xFBE5) u8 NEWKEY[11];

u8 KeyMatrix_Press(s16 km)
{
	u8 column = km >> 8;
	u8 bit = km & 0xff;
	return (~NEWKEY[column]) & bit;
}
