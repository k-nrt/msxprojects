//-----------------------------------------------------------------------------
//. BIOS Wrapper.
//-----------------------------------------------------------------------------
#ifndef __MSX_BIOS_WRAPPER_H__
#define __MSX_BIOS_WRAPPER_H__

#include <msx-types.h>

//-----------------------------------------------------------------------------
//. Macros.
//-----------------------------------------------------------------------------
//. VDP Register Save Area.
#define MSX_VDPREG( _r ) *(((u8*)0xF3DF)+_r)


//-----------------------------------------------------------------------------
//. BIOS.
//-----------------------------------------------------------------------------
//. RDSLT.
extern u8 msxBiosReadSlot( u8 iSlot, u16 nAddress );

//. WRSLT.
extern void msxBiosWriteSlot( u8 iSlot, u16 nAddress, u8 nParam );

//. ENASLT.
extern void msxBiosEnableSlot( u8 iSlot );

//. DISSCR.
extern void msxBiosDisableScreen(void);

//. ENASCR.
extern void msxBiosEnableScreen(void);

//. WRTVDP.
extern void msxBiosWriteVDP( u8 nRegister, u8 nParam );

//. RDVRM.
extern u8 msxBiosReadVRAM( u16 nAddress );

//. WRTVRM.
extern void msxBiosWriteVRAM( u16 nAddress, u8 nParam );

//. SETRD
extern void msxBiosSetRead( u16 nAddress );

//. SETWRT
extern void msxBiosSetWrite( u16 nAddress );

//. FILVRM.
extern void msxBiosFillVRAM( u16 nAddress, u16 nSize, u8 nParam );

//. LDIRMV.
extern void msxBiosCopyVRAMtoRAM( u16 nSrc, u16 nDst, u16 nSize );

//. LDIRVM.
extern void msxBiosCopyRAMtoVRAM( u16 nSrc, u16 nDst, u16 nSize );

//. CHGMOD
extern void msxBiosChangeScreenMode( u8 nScreenMode );

//. CHGCLR.
extern void msxBiosChangeColor( u8 nMode, u8 nForgroundColor, u8 nBackgroundColor, u8 nBorderColor );

//. CLRSPR.
extern void msxBiosClearSprite( u8 nScreenMode );

//. INITXT.
extern void msxBiosInitializeTextMode(void);

//. INIT32.
extern void msxBiosInitializeText32Mode(void);

//. INIGRP.
extern void msxBiosInitializeGraphicMode(void);

//. INIMLT.
extern void msxBiosInitializeMultiColorMode(void);

//. CALPAT.
extern u16 msxBiosGetSpriteGeneratorTableAddress( u8 iSprite );

//. CALATR.
extern u16 msxBiosGetSpriteAttributeTableAddress( u8 iSprite );

//. GRPPRT.
extern void msxBiosGraphicPrint( u8 nChar );

//. GICINI.
extern void msxBiosInitializePSG(void);

//. WRTPSG.
extern void msxBiosWritePSG( u8 nRegister, u8 nParam );

//. RDPSG.
extern u8 msxBiosReadPSG( u8 nRegister );

//. GTSTCK.
extern u8 msxBiosGetStick( u8 nPad );

//. GTTRIG.
extern u8 msxBiosGetTrigger( u8 nTrigger );

//. CHGSND.
extern void msxBiosChangeSound( u8 nParam );

//. RSLREG.
extern u8 msxBiosReadSlotRegister(void);

//. WSLREG.
extern void msxBiosWriteSlotRegister( u8 nParam );

//. RDVDP.
extern u8 msxBiosReadVDPStatus(void);

//. SNSMAT.
extern u8 msxBiosReadKeyMatrix( u8 iLine );

//. GRPPRT.
extern void msx2BiosGraphicPrint(u8 u8Code);

//. CHGMDP.
extern void msx2BiosChangeModePalette(u8 u8Screen);

//. SETPAG.
extern void msx2BiosSetPage( u8 u8Display, u8 u8Active );

//. NRDVRM.
extern u8 msx2BiosReadVRAM( u16 u16Address );

//. NWRVRM.
extern void msx2BiosWriteVRAM( u16 u16Address, u8 u8Value );

//. SUBROM WRTVDP.
extern void msx2BiosWriteVDP(u8 nRegister, u8 nParam);

//. SUBROM RDVDP.
extern u8 msx2BiosReadVDPStatus(u8 nRegister);

#endif //__msxBios_BIOS_WRAPPER_H__

