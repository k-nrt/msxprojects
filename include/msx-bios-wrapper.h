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
extern u8 msxBiosReadSlot( u8 iSlot, u16 nAddress ) SDCCCALL(0);

//. WRSLT.
extern void msxBiosWriteSlot( u8 iSlot, u16 nAddress, u8 nParam ) SDCCCALL(0);

//. ENASLT.
extern void msxBiosEnableSlot( u8 iSlot ) SDCCCALL(0);

//. DISSCR.
extern void msxBiosDisableScreen(void);

//. ENASCR.
extern void msxBiosEnableScreen(void);

//. WRTVDP.
extern void msxBiosWriteVDP( u8 nRegister, u8 nParam ) SDCCCALL(0);

//. RDVRM.
extern u8 msxBiosReadVRAM( u16 nAddress ) SDCCCALL(0);

//. WRTVRM.
extern void msxBiosWriteVRAM( u16 nAddress, u8 nParam ) SDCCCALL(0);

//. SETRD
extern void msxBiosSetRead( u16 nAddress ) SDCCCALL(0);

//. SETWRT
extern void msxBiosSetWrite( u16 nAddress ) SDCCCALL(0);

//. FILVRM.
extern void msxBiosFillVRAM( u16 nAddress, u16 nSize, u8 nParam ) SDCCCALL(0);

//. LDIRMV.
extern void msxBiosCopyVRAMtoRAM( u16 nSrc, u16 nDst, u16 nSize ) SDCCCALL(0);

//. LDIRVM.
extern void msxBiosCopyRAMtoVRAM( u16 nSrc, u16 nDst, u16 nSize ) SDCCCALL(0);

//. CHGMOD
extern void msxBiosChangeScreenMode( u8 nScreenMode ) SDCCCALL(0);

//. CHGCLR.
extern void msxBiosChangeColor( u8 nMode, u8 nForgroundColor, u8 nBackgroundColor, u8 nBorderColor ) SDCCCALL(0);

//. CLRSPR.
extern void msxBiosClearSprite( u8 nScreenMode ) SDCCCALL(0);

//. INITXT.
extern void msxBiosInitializeTextMode(void);

//. INIT32.
extern void msxBiosInitializeText32Mode(void);

//. INIGRP.
extern void msxBiosInitializeGraphicMode(void);

//. INIMLT.
extern void msxBiosInitializeMultiColorMode(void);

//. CALPAT.
extern u16 msxBiosGetSpriteGeneratorTableAddress( u8 iSprite ) SDCCCALL(0);

//. CALATR.
extern u16 msxBiosGetSpriteAttributeTableAddress( u8 iSprite ) SDCCCALL(0);

//. GRPPRT.
extern void msxBiosGraphicPrint( u8 nChar ) SDCCCALL(0);

//. GICINI.
extern void msxBiosInitializePSG(void);

//. WRTPSG.
extern void msxBiosWritePSG( u8 nRegister, u8 nParam ) SDCCCALL(0);

//. RDPSG.
extern u8 msxBiosReadPSG( u8 nRegister ) SDCCCALL(0);

//. GTSTCK.
extern u8 msxBiosGetStick( u8 nPad ) SDCCCALL(0);

//. GTTRIG.
extern u8 msxBiosGetTrigger( u8 nTrigger ) SDCCCALL(0);

//. CHGSND.
extern void msxBiosChangeSound( u8 nParam ) SDCCCALL(0);

//. RSLREG.
extern u8 msxBiosReadSlotRegister(void);

//. WSLREG.
extern void msxBiosWriteSlotRegister( u8 nParam ) SDCCCALL(0);

//. RDVDP.
extern u8 msxBiosReadVDPStatus(void);

//. SNSMAT.
extern u8 msxBiosReadKeyMatrix( u8 iLine ) SDCCCALL(0);

//. GRPPRT.
extern void msx2BiosGraphicPrint(u8 u8Code) SDCCCALL(0);

//. CHGMDP.
extern void msx2BiosChangeModePalette(u8 u8Screen) SDCCCALL(0);

//. SETPAG.
extern void msx2BiosSetPage( u8 u8Display, u8 u8Active ) SDCCCALL(0);

//. NRDVRM.
extern u8 msx2BiosReadVRAM( u16 u16Address ) SDCCCALL(0);

//. NWRVRM.
extern void msx2BiosWriteVRAM( u16 u16Address, u8 u8Value ) SDCCCALL(0);

//. SUBROM WRTVDP.
extern void msx2BiosWriteVDP(u8 nRegister, u8 nParam) SDCCCALL(0);

//. SUBROM RDVDP.
extern u8 msx2BiosReadVDPStatus(u8 nRegister) SDCCCALL(0);

#endif //__msxBios_BIOS_WRAPPER_H__

