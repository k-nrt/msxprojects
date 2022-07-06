#ifndef VDP_COMMAND_H
#define VDP_COMMAND_H
#include <msx-types.h>
#include "macros.h"

//! FORCLR(F3E9H, 1).
//! �����l	15.
//! ���e	�O�i�F�BCOLOR���Őݒ肳���.
SDCC_FIXED_ADDRESS(0xF3E9) extern u8 FORCLR;

//! BAKCLR(F3EAH, 1)
//! �����l	4
//! ���e	�w�i�F�BCOLOR���Őݒ肳���
SDCC_FIXED_ADDRESS(0xF3EA) extern u8 BAKCLR;

//! BDRCLR(F3EBH, 1).
//! �����l	7.
//! ���e	���ӐF�BCOLOR���Őݒ肳���.
SDCC_FIXED_ADDRESS(0xF3EB) extern u8 BDRCLR;

//! DFPAGE(FAF5H, 1)
//! ���e	�f�B�X�v���C�y�[�W�ԍ�
SDCC_FIXED_ADDRESS(0xFAF5) extern u8 DFPAGE;

//! ACPAGE(FAF6H, 1)
//! ���e	�A�N�e�B�u�y�[�W�ԍ�
SDCC_FIXED_ADDRESS(0xFAF6) extern u8 ACPAGE;

//! GRPACX(FCB7H, 2).
//! ���e	�O���t�B�b�N�A�L�������[�^(X���W).
SDCC_FIXED_ADDRESS(0xFCB7) extern u16 GRPACX;

//! GRPACY(FCB9H, 2)
//! ���e	�O���t�B�b�N�A�L�������[�^(Y���W).
SDCC_FIXED_ADDRESS(0xFCB9) extern u16 GRPACY;

extern void VDPSetDisplayPage(u8 nPage) SDCCCALL(0);

#define VDPSetActivePage(_page) ACPAGE=_page
#define VDPSetForegroundColor(_color) FORCLR=_color
#define VDPSetBackgroundColor(_color) BAKCLR=_color
#define VDPSetBorderColor(_color) BDRCLR=_color
#define VDPSetColor(_fg,_bg,_bd) FORCLR=_fg;BAKCLR=_bg;BDRCLR=_bd

void VDPWait() SDCCCALL(0);
void VDPWaitLine(u8 sx, u8 sy, u8 ex, u8 ey) SDCCCALL(0);
void VDPWaitLine2(u8 sx, u8 sy, u8 ex, u8 ey) SDCCCALL(0);

void VDPFill(u8 x, u8 y, u16 w, u16 h) SDCCCALL(0);

#define VDPSetPrintPosition(_x,_y) GRPACX=_x;GRPACY=_y
#define VDPPrint(_x,_y,_string) GRPACX=_x;GRPACY=_y;VDPPrintCore(_string)
extern void VDPPrintCore(const char *pszString);

#define VDPPrintU8D(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU8DCore(_v)
#define VDPPrintU16D(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU16DCore(_v)
extern void VDPPrintU8DCore(u8 value) SDCCCALL(0);
extern void VDPPrintU16DCore(u16 value) SDCCCALL(0);

#define VDPPrintU8X(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU8XCore(_v)
#define VDPPrintU16X(_x,_y,_v) GRPACX=_x;GRPACY=_y;VDPPrintU16XCore(_v)
extern void VDPPrintU8XCore(u8 value) SDCCCALL(0);
extern void VDPPrintU16XCore(u16 value) SDCCCALL(0);

extern void VDPPrintF(const char *pszFormat, ...);
#endif //VDP_COMMAND_H
