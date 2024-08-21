//. MSX-DOS VDP BIOS wrapper.
#pragma once
#include <msx-types.h>

extern void vdpEnableScreen(u8 enable); //! DISSCR/ENASCR.
extern void vdpWriteRegister(u8 register, u8 data); //! WRTVDP.
extern u8 vdpReadRegister(u8 register);
extern u8 vdpReadStatusRegister0(void); //! RDVDP.

extern void vdpSetActivePage(u8 page); //! 0xFAF6
extern void vdpSetDisplayPage(u8 page); //! 0xFAF5
//extern void vdpSetPage(); //! SETPAG.

extern u8 vdpReadVram(u16 address); //! NRDVRM.
extern void vdpWriteVram(u16 address, u8 data); //! NWRVRM.
extern void vdpFillVram(u16 address, u16 size, u8 data); //! BIGFIL.
extern void vdpCopyToVram(const void* ram, u16 vram, u16 size); //! LDIRVM.
extern void vdpCopyFromVram(u16 vram, void* ram, u16 size); //! LDIRMV.

extern void vdpSetScreenMode(u8 screenMode); //! CHGMOD.
extern void vdpSetScreenColor(u8 screenMode, u8 foregroundColor, u8 backgroundColor, u8 borderColor); //! CHGCLR.

extern void vdpClearSprite(void); //! CLRSPR.

enum vdpSpriteMode
{
	kVdpSpriteMode_8x8       = 0x00,
	kVdpSpriteMode_8x8_Mag   = 0x01,
	kVdpSpriteMode_16x16     = 0x02,
	kVdpSpriteMode_16x16_Mag = 0x03,
};
extern void vdpSetSpriteMode(enum vdpSpriteMode spriteMode);

enum vdpLogicalOperationCode
{
	kVdpLogicalOperationCode_IMP = 0x00,
	kVdpLogicalOperationCode_AND = 0x01,
	kVdpLogicalOperationCode_OR  = 0x02,
	kVdpLogicalOperationCode_EOR = 0x03,
	kVdpLogicalOperationCode_NOT = 0x04,
	kVdpLogicalOperationCode_TIMP = 0x08,
	kVdpLogicalOperationCode_TAND = 0x09,
	kVdpLogicalOperationCode_TOR  = 0x0a,
	kVdpLogicalOperationCode_TEOR = 0x0b,
	kVdpLogicalOperationCode_TNOT = 0x0c,
};

extern void vdpSetLogicalOperation(enum vdpLogicalOperationCode code); //! 0xFB02.

extern void vdpSetForegroundColor(u8 color); //! 0xF3E9.
extern void vdpSetBackgroundColor(u8 color); //! 0xF3EA.

extern void vdpSetGraphicAccumlator(u16 x, u16 y); //! 0xFCB7 / 0xFCB9.

extern void vdpPrintChar(char c); //! GRPPRT.
extern void vdpPrint(const char *str);
extern void vdpPrintHexU8(u8 hex);
extern void vdpPrintHexU16(u16 hex);
extern void vdpPrintHexU32(u32 hex);

extern void vdpSetPalette(u8 offset, u8 size, u8* colors);

extern void vdpSetScreenModePalette(u8 u8Screen); //! SUBROM CHGMDP.
