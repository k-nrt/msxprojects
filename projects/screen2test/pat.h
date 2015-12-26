#ifndef PAT_H
#define PAT_H
#include <msx-bios-wrapper.h>

extern u8 g_u8PatBuffer;
extern u16 g_pPatNameTable;

void PatClearBuffer();
void PatBlit( s8 x, s8 y, u8 w, u8 h, const u8 *pPattern );
void PatFill( s8 x, s8 y, s8 w, s8 h, const u8 pattern );
void PatFlip();


#endif //PAT_H
