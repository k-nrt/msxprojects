#ifndef VDP_COMMAND_H
#define VDP_COMMAND_H
#include <msx-types.h>
#include "macros.h"

//! FORCLR(F3E9H, 1).
//! 初期値	15.
//! 内容	前景色。COLOR文で設定される.
SDCC_FIXED_ADDRESS(0xF3E9) extern u8 FORCLR;

//! BAKCLR(F3EAH, 1)
//! 初期値	4
//! 内容	背景色。COLOR文で設定される
SDCC_FIXED_ADDRESS(0xF3EA) extern u8 BAKCLR;

//! BDRCLR(F3EBH, 1).
//! 初期値	7.
//! 内容	周辺色。COLOR文で設定される.
SDCC_FIXED_ADDRESS(0xF3EB) extern u8 BDRCLR;

//! DFPAGE(FAF5H, 1)
//! 内容	ディスプレイページ番号
SDCC_FIXED_ADDRESS(0xFAF5) extern u8 DFPAGE;

//! ACPAGE(FAF6H, 1)
//! 内容	アクティブページ番号
SDCC_FIXED_ADDRESS(0xFAF6) extern u8 ACPAGE;

//! GRPACX(FCB7H, 2).
//! 内容	グラフィックアキュムレータ(X座標).
SDCC_FIXED_ADDRESS(0xFCB7) extern u16 GRPACX;

//! GRPACY(FCB9H, 2)
//! 内容	グラフィックアキュムレータ(Y座標).
SDCC_FIXED_ADDRESS(0xFCB9) extern u16 GRPACY;

extern void VDPSetDisplayPage(u8 nPage);

#define VDPSetActivePage(_page) ACPAGE=_page
#define VDPSetForegroundColor(_color) FORCLR=_color
#define VDPSetBackgroundColor(_color) BAKCLR=_color
#define VDPSetBorderColor(_color) BDRCLR=_color
#define VDPSetColor(_fg,_bg,_bd) FORCLR=_fg;BAKCLR=_bg;BDRCLR=_bd

void VDPWait();
void VDPWaitLine(u8 sx, u8 sy, u8 ex, u8 ey);
void VDPWaitLine2(u8 sx, u8 sy, u8 ex, u8 ey);

void VDPFill(u8 x, u8 y, u16 w, u16 h);

#define VDPSetPrintPosition(_x,_y) GRPACX=_x;GRPACY=_y
#define VDPPrint(_x,_y,_string) GRPACX=_x;GRPACY=_y;VDPPrintCore(_string)
extern void VDPPrintCore(const char *pszString);

#define VDPPrintU8D(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU8DCore(_v)
#define VDPPrintU16D(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU16DCore(_v)
extern void VDPPrintU8DCore(u8 value);
extern void VDPPrintU16DCore(u16 value);

#define VDPPrintU8X(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU8XCore(_v)
#define VDPPrintU16X(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU16XCore(_v)
extern void VDPPrintU8XCore(u8 value);
extern void VDPPrintU16XCore(u16 value);

extern void VDPPrintF(const char *pszFormat, ...);
#endif //VDP_COMMAND_H
