;------------------------------------------------------------------------------
; MSX BIOS Wrapper.
;------------------------------------------------------------------------------
	.area	_CODE
	.globl _msxBiosReadSlot
	.globl _msxBiosWriteSlot
	.globl _msxBiosEnableSlot
	.globl _msxBiosDisableScreen
	.globl _msxBiosEnableScreen
	.globl _msxBiosWriteVDP
	.globl _msxBiosReadVRAM
	.globl _msxBiosWriteVRAM
	.globl _msxBiosSetRead
	.globl _msxBiosSetWrite
	.globl _msxBiosFillVRAM
	.globl _msxBiosCopyVRAMtoRAM
	.globl _msxBiosCopyRAMtoVRAM
	.globl _msxBiosChangeScreenMode
	.globl _msxBiosChangeColor
	.globl _msxBiosClearSprite
	.globl _msxBiosInitializeTextMode
	.globl _msxBiosInitializeText32Mode
	.globl _msxBiosInitializeGraphicMode
	.globl _msxBiosInitializeMultiColorMode
	.globl _msxBiosGetSpriteGeneratorTableAddress
	.globl _msxBiosGetSpriteAttributeTableAddress
	.globl _msxBiosGraphicPrint
	.globl _msxBiosInitializePSG
	.globl _msxBiosWritePSG
	.globl _msxBiosReadPSG
	.globl _msxBiosGetStick
	.globl _msxBiosGetTrigger
	.globl _msxBiosChangeSound
	.globl _msxBiosReadSlotRegister
	.globl _msxBiosWriteSlotRegister
	.globl _msxBiosReadVDPStatus
	.globl _msxBiosReadKeyMatrix
	
	.globl _msx2BiosGraphicPrint
	.globl _msx2BiosChangeModePalette

	.globl _msx2BiosSetPage
	.globl _msx2BiosReadVRAM
	.globl _msx2BiosWriteVRAM

	.globl _msx2BiosWriteVDP
	.globl _msx2BiosReadVDPStatus

;------------------------------------------------------------------------------
; BIOS Entry
;------------------------------------------------------------------------------
	CHKRAM = 0x0000
	RDSLT  = 0x000c
	WRSLT  = 0x0014
	ENASLT = 0x0024

	DISSCR = 0x0041
	ENASCR = 0x0044
	WRTVDP = 0x0047
	RDVRM  = 0x004a
	WRTVRM = 0x004d
	SETRD  = 0x0050
	SETWRT = 0x0053
	FILVRM = 0x0056
	LDIRMV = 0x0059
	LDIRVM = 0x005c
	CHGMOD = 0x005f
	CHGCLR = 0x0062
	CLRSPR = 0x0069
	INITXT = 0x006c
	INIT32 = 0x006f
	INIGRP = 0x0072
	INIMLT = 0x0075
	SETTXT = 0x0078
	SETT32 = 0x007b
	SETGRP = 0x007e
	SETMLT = 0x0081
	CALPAT = 0x0084
	CALATR = 0x0087
	GSPSIZ = 0x008a
	GRPPRT = 0x008d

	GICINI = 0x0090
	WRTPSG = 0x0093
	RDPSG  = 0x0096
	
	CHSNS  = 0x009c
	CHGET  = 0x009f
	CHPUT  = 0x00a2
	BEEP   = 0x00c0
	CLS    = 0x00c3
	POSIT  = 0x00c6
	FNKSB  = 0x00c9
	ERAFNK = 0x00cc
	DSPFNK = 0x00cf
	TOTEXT = 0x00d2

	GTSTCK = 0x00D5
	GTTRIG = 0x00d8

	CHGCAP = 0x0132
	CHGSND = 0x0135
	RSLREG = 0x0138
	WSLREG = 0x013b
	RDVDP  = 0x013e
	SNSMAT = 0x0141
	KILBUF = 0x0156
	
	SUBROM = 0x015c
	EXTROM = 0x015f
	
	BIGFIL = 0x016b
	NSETRD = 0x016E
	NSTWRT = 0x0171
	NRDVRM = 0x0174
	NWRVRM = 0x0177

; SUB-ROM
	SUB_GRPPRT = 0x0089
	SUB_NVBXLN = 0x00C9
	SUB_NVBXFL = 0x00CD
	SUB_CHGMOD = 0x00D1
	SUB_INITXT = 0x00D5
	SUB_INIT32 = 0x00D9
	SUB_INIGRP = 0x00DD
	SUB_INIMLT = 0x00E1
	SUB_SETTXT = 0x00E5
	SUB_SETT32 = 0x00E9
	SUB_SETGRP = 0x00ED
	SUB_SETMLT = 0x00F1
	SUB_CLRSPR = 0x00F5
	SUB_CALPAT = 0x00F9
	SUB_CALATR = 0x00FD
	SUB_GSPSIZ = 0x0101
	SUB_GETPAT = 0x0105
	SUB_WRTVRM = 0x0109
	SUB_RDVRM = 0x010D
	SUB_CHGCLR = 0x0111
	SUB_CLSSUB = 0x0115
	SUB_DSPFNK = 0x011D
	SUB_WRTVDP = 0x012D
	SUB_VDPSTA = 0x0131
	SUB_SETPAG = 0x013D
	SUB_INIPLT = 0x0141
	SUB_RSTPLT = 0x0145
	SUB_GETPLT = 0x0149
	SUB_SETPLT = 0x014D
	SUB_BEEP = 0x017D
	SUB_PROMPT = 0x0181
	SUB_NEWPAD = 0x01AD
	SUB_CHGMDP = 0x01B5
	SUB_KNJPRT = 0x01BD
	SUB_REDCLK = 0x01F5
	SUB_WRTCLK = 0x01F9
	
;------------------------------------------------------------------------------
; WORK.
;------------------------------------------------------------------------------
	DPPAGE = 0xFAF5
	ACPAGE = 0xFAF6
	
;------------------------------------------------------------------------------
; RDSLT.
; extern u8 msxBiosReadSlot( u8 iSlot, u16 nAddress );
;------------------------------------------------------------------------------
_msxBiosReadSlot:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; iSlot
	inc		hl
	ld		e,(hl)
	inc		hl
	ld		d,(hl)	; nAddress
	ex		de,hl
	call	RDSLT
	ld		l, a	; result
	ret

;------------------------------------------------------------------------------
; WRSLT.
; extern void msxBiosWriteSlot( u8 iSlot, u16 nAddress, u8 nParam );
;------------------------------------------------------------------------------
_msxBiosWriteSlot:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; iSlot
	inc		hl
	ld		e,(hl)
	inc		hl
	ld		d,(hl)	; nAddress
	inc		hl
	ld		l,(hl)	; nParam
	ex		de,hl
	jp		WRSLT

;------------------------------------------------------------------------------
; ENASLT.
; extern void msxBiosEnableSlot( u8 iSlot );
;------------------------------------------------------------------------------
_msxBiosEnableSlot:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; iSlot
	jp		ENASLT

;------------------------------------------------------------------------------
; DISSCR.
; extern void msxBiosDisableScreen(void);
;------------------------------------------------------------------------------
_msxBiosDisableScreen:
	jp		DISSCR

;------------------------------------------------------------------------------
; ENASCR.
; extern void msxBiosEnableScreen(void);
;------------------------------------------------------------------------------
_msxBiosEnableScreen:
	jp		ENASCR

;------------------------------------------------------------------------------
; WRTVDP.
; extern void msxBiosWriteVDP( u8 nRegister, u8 nParam );
;------------------------------------------------------------------------------
_msxBiosWriteVDP:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		c,(hl)	; nRegister
	inc		hl
	ld		b,(hl)	; nParam
	jp		WRTVDP

;------------------------------------------------------------------------------
; RDVRM.
; extern u8 msxBiosReadVRAM( u16 nAddress );
;------------------------------------------------------------------------------
_msxBiosReadVRAM:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		e,(hl)	; nRegister
	inc		hl
	ld		d,(hl)	; nParam
	ex		de,hl
	call	RDVRM
	ld		l,a		; result
	ret

;------------------------------------------------------------------------------
; WRTVRM.
; extern void msxBiosWriteVRAM( u16 nAddress, u8 nParam );
;------------------------------------------------------------------------------
_msxBiosWriteVRAM:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)
	inc		hl
	ld		a,(hl)
	ex		de,hl
	jp		WRTVRM
	
;------------------------------------------------------------------------------
; SETRD
; extern void msxBiosSetRead( u16 nAddress );
;------------------------------------------------------------------------------
_msxBiosSetRead:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)
	ex		de,hl
	jp		SETRD

;------------------------------------------------------------------------------
; SETWRT
; extern void msxBiosSetWrite( u16 nAddress );
;------------------------------------------------------------------------------
_msxBiosSetWrite:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)
	ex		de,hl
	jp		SETWRT

;------------------------------------------------------------------------------
; FILVRM.
; extern void msxBiosFillVRAM( u16 nAddress, u16 nSize, u8 nParam );
;------------------------------------------------------------------------------
_msxBiosFillVRAM:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)		; nAddress
	inc		hl
	ld		c,(hl)
	inc		hl
	ld		b,(hl)		; nSize
	inc		hl
	ld		a,(hl)		; nParam
	ex		de,hl
	jp		FILVRM

;------------------------------------------------------------------------------
; LDIRMV.
; extern void msxBiosCopyVRAMtoRAM( u16 nSrc, u16 nDst, u16 nSize );
;------------------------------------------------------------------------------
_msxBiosCopyVRAMtoRAM:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)		; nSrc
	inc		hl
	ld		c,(hl)
	inc		hl
	ld		b,(hl)		; nDst
	inc		hl
	ld		a,(hl)
	inc		hl
	ld		h,(hl)		; nSizes
	; bc -> hl
	; ha -> bc
	ld		l,c
	ld		c,a
	ld		a,h
	ld		h,b
	ld		b,a
	ex		de,hl
	push	ix
	call	LDIRMV
	pop		ix
	ret
	

;------------------------------------------------------------------------------
; LDIRVM.
; extern void msxBiosCopyRAMtoVRAM( u16 nSrc, u16 nDst, u16 nSize );
;------------------------------------------------------------------------------
_msxBiosCopyRAMtoVRAM:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)
	inc		hl
	ld		d,(hl)		; nSrc
	inc		hl
	ld		c,(hl)
	inc		hl
	ld		b,(hl)		; nDst
	inc		hl
	ld		a,(hl)
	inc		hl
	ld		h,(hl)		; nSizes
	; bc -> hl
	; ha -> bc
	ld		l,c
	ld		c,a
	ld		a,h
	ld		h,b
	ld		b,a
	ex		de,hl
	push	ix
	call	LDIRVM
	pop		ix
	ret
	

;------------------------------------------------------------------------------
; CHGMOD
; extern void msxBiosChangeScreenMode( u8 nScreenMode );
;------------------------------------------------------------------------------
_msxBiosChangeScreenMode:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nScreenMode
	push	ix
	call	CHGMOD
	pop		ix
	ret

;------------------------------------------------------------------------------
; CHGCLR.
; extern void msxBiosChangeColor( u8 nMode, u8 nForgroundColor, u8 nBackgroundColor, u8 nBorderColor );
;------------------------------------------------------------------------------
_msxBiosChangeColor:
	FORCLR = 0xf3e9
	BAKCLR = 0xf3ea
	BDRCLR = 0xf3eb

	ld		hl,#2
	add		hl,sp
	ld		b,(hl)		; nMode
	inc		hl
	ld		a,(hl)		; nForgroundColor
	inc		hl
	ld		(FORCLR),a
	ld		a,(hl)		; nBackgroundColor
	inc		hl
	ld		(BAKCLR),a
	ld		a,(hl)		; nBorderColor
	inc		hl
	ld		(BDRCLR),a
	ld		a,b
	push	ix
	call	CHGCLR
	pop		ix
	ret
	
;------------------------------------------------------------------------------
; CLRSPR.
; extern void msxBiosClearSprite( u8 nScreenMode );
;------------------------------------------------------------------------------
_msxBiosClearSprite:
	SCRMOD = 0xfcaf

	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nScreenMode
	ld		(SCRMOD),a
	push	ix
	call	CLRSPR
	pop		ix
	ret

;------------------------------------------------------------------------------
; INITXT.
; extern void msxBiosInitializeTextMode(void);
;------------------------------------------------------------------------------
_msxBiosInitializeTextMode:
	push	ix
	call	INITXT
	pop		ix
	ret


;------------------------------------------------------------------------------
; INIT32.
; extern void msxBiosInitializeText32Mode(void);
;------------------------------------------------------------------------------
_msxBiosInitializeText32Mode:
	push	ix
	call	INIT32
	pop		ix
	ret

;------------------------------------------------------------------------------
; INIGRP.
; extern void msxBiosInitializeGraphicMode(void);
;------------------------------------------------------------------------------
_msxBiosInitializeGraphicMode:
	push	ix
	call	INIGRP
	pop		ix
	ret

;------------------------------------------------------------------------------
; INIMLT.
; extern void msxBiosInitializeMultiColorMode(void);
;------------------------------------------------------------------------------
_msxBiosInitializeMultiColorMode:
	push	ix
	call	INIMLT
	pop		ix
	ret

;------------------------------------------------------------------------------
; CALPAT.
; extern u16 msxBiosGetSpriteGeneratorTableAddress( u8 iSprite );
;------------------------------------------------------------------------------
_msxBiosGetSpriteGeneratorTableAddress:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; iSprite
	jp		CALPAT		; result
	
;------------------------------------------------------------------------------
; CALATR.
; extern u16 msxBiosGetSpriteAttributeTableAddress( u8 iSprite );
;------------------------------------------------------------------------------
_msxBiosGetSpriteAttributeTableAddress:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; iSprite
	jp		CALATR		; result

;------------------------------------------------------------------------------
; GRPPRT.
; extern void msxBiosGraphicPrint( u8 nChar );
;------------------------------------------------------------------------------
_msxBiosGraphicPrint:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nChar
	jp		GRPPRT		; result

;------------------------------------------------------------------------------
; GICINI.
; extern void msxBiosInitializePSG(void);
;------------------------------------------------------------------------------
_msxBiosInitializePSG:
	push	ix
	call	GICINI
	pop		ix
	ret

;------------------------------------------------------------------------------
; WRTPSG.
; extern void msxBiosWritePSG( u8 nRegister, u8 nParam );
;------------------------------------------------------------------------------
_msxBiosWritePSG:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nRegister
	inc		hl
	ld		e,(hl)		; nParam
	jp		WRTPSG

;------------------------------------------------------------------------------
; RDPSG.
; extern u8 msxBiosReadPSG( u8 nRegister );
;------------------------------------------------------------------------------
_msxBiosReadPSG:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nRegister
	call	RDPSG
	ld		l,a			; result
	ret


;------------------------------------------------------------------------------
; extern u8 msxBiosGetStick( u8 nPad );
;------------------------------------------------------------------------------
_msxBiosGetStick:
	ld		hl,#2
	add		hl,sp
	
	ld		a,(hl)
	push	ix
	call	GTSTCK
	pop		ix
	ld		l,a
	ret

;------------------------------------------------------------------------------
; GTTRIG.
; extern u8 msxBiosGetTrigger( u8 nTrigger );
;------------------------------------------------------------------------------
_msxBiosGetTrigger:
	ld		hl,#2
	add		hl,sp
	
	ld		a,(hl)
	call	GTTRIG
	ld		l,a
	ret

;------------------------------------------------------------------------------
; CHGSND.
; extern void msxBiosChangeSound( u8 nParam );
;------------------------------------------------------------------------------
_msxBiosChangeSound:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nParam
	jp		CHGSND

;------------------------------------------------------------------------------
; RSLREG.
; extern u8 msxBiosReadSlotRegister(void);
;------------------------------------------------------------------------------
_msxBiosReadSlotRegister:
	call	RSLREG
	ld		l,a
	ret

;------------------------------------------------------------------------------
; WSLREG.
; extern void msxBiosWriteSlotRegister( u8 nParam );
;------------------------------------------------------------------------------
_msxBiosWriteSlotRegister:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; nParam
	jp		WSLREG

;------------------------------------------------------------------------------
; RDVDP.
; extern u8 msxBiosReadVDP(void);
;------------------------------------------------------------------------------
_msxBiosReadVDPStatus:
	di
	call	RDVDP
	ei
	ld		l,a
	ret

;------------------------------------------------------------------------------
; SNSMAT.
; extern u8 msxBiosReadKeyMatrix( u8 iLine );
;------------------------------------------------------------------------------
_msxBiosReadKeyMatrix:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; iLine
	call	SNSMAT
	ld		l,a
	ret

;------------------------------------------------------------------------------
; GRPPRT.
; extern void msx2BiosGraphicPrint( u8 u8Code );
;------------------------------------------------------------------------------
_msx2BiosGraphicPrint:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; u8Code

	push	ix
	ld		ix,#SUB_GRPPRT
	call	EXTROM
	pop		ix
	ret

;------------------------------------------------------------------------------
; CHGMDP.
; extern void msx2BiosChangeModePalette( u8 u8Screen );
;------------------------------------------------------------------------------
_msx2BiosChangeModePalette:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; u8Screen

	push	ix
	ld		ix,#SUB_CHGMDP
	call	EXTROM
	pop		ix
	ret

;------------------------------------------------------------------------------
; SETPAG.
; extern void msx2BiosSetPage( u8 u8Display, u8 u8Active );
;------------------------------------------------------------------------------
_msx2BiosSetPage:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; u8Display
	ld		(#DPPAGE),a
	inc		hl
	ld		a,(hl)		; u8Active
	ld		(#ACPAGE),a

	push	ix
	ld		ix,#SUB_SETPAG
	call	EXTROM
	pop		ix
	ret

;------------------------------------------------------------------------------
; NRDVRM.
; extern u8 msx2BiosReadVRAM( u16 u16Address );
;------------------------------------------------------------------------------
_msx2BiosReadVRAM:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)		; u16Address L
	inc		hl
	ld		d,(hl)		; u16Address H
	ex		de,hl
	call	NRDVRM
	ld		l,a
	ret

;------------------------------------------------------------------------------
; NWRVRM.
; extern void msx2BiosWriteVRAM( u16 u16Address, u8 u8Value );
;------------------------------------------------------------------------------
_msx2BiosWriteVRAM:
	ld		hl,#2
	add		hl,sp
	ld		e,(hl)		; u16Address L
	inc		hl
	ld		d,(hl)		; u16Address H
	inc		hl
	ld		a,(hl)
	ex		de,hl
	jp		NWRVRM

;------------------------------------------------------------------------------
; WRTVDP.
; extern void msx2BiosWriteVDP( u8 u8Register, u8 u8Value );
;------------------------------------------------------------------------------
_msx2BiosWriteVDP:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; u8Register
	ld		c,a
	inc		hl
	ld		a,(hl)		; u8Value
	ld		b,a

	push	ix
	ld		ix,#SUB_WRTVDP
	call	EXTROM
	pop		ix
	ret

;------------------------------------------------------------------------------
; VDPSTA.
; extern u8 msx2BiosReadVDPStatus( u8 u8Register );
;------------------------------------------------------------------------------
_msx2BiosReadVDPStatus:
	ld		hl,#2
	add		hl,sp
	ld		a,(hl)		; u8Register

	push	ix
	ld		ix,#SUB_VDPSTA
	call	EXTROM
	pop		ix
	ld		l,a
	ret
;------------------------------------------------------------------------------












