;------------------------------------------------------------------------------
; BIOS.
;------------------------------------------------------------------------------
	.include	"msx-bios-entry.s"

;------------------------------------------------------------------------------
; System work area
;------------------------------------------------------------------------------
	.include	"msx-system-work.s"

;------------------------------------------------------------------------------
; Macro.
;------------------------------------------------------------------------------
	.include	"msx-dos-call-macro.s"

;------------------------------------------------------------------------------
; void vdpEnableScreen(u8 enable)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpEnableScreen

_vdpEnableScreen:
	push	ix
	or	a
	jr	z, _vdpEnableScreen_Disable

_vdpEnableScreen_Enable:
	ld	ix, #ENASCR
	jr	_vdpEnableScreen_CALSLT

_vdpEnableScreen_Disable:
	ld	ix, #DISSCR

_vdpEnableScreen_CALSLT:
	push	iy
	ld	iy, (#EXPTBL - #0x0001)
	call	CALSLT
	pop	iy
	pop	ix
	ret

;------------------------------------------------------------------------------
; void vdpWriteRegister(u8 register, u8 data)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpWriteRegister

_vdpWriteRegister:
	ld	c,a
	ld	b,l
	call_main_rom	WRTVDP
	ret

;------------------------------------------------------------------------------
; u8 vdpReadRegister(u8 register)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpReadRegister

_vdpReadRegister:
	ld	h, #0x00
	ld	l, a
	cp	#8
	jr	nc, _vdpReadRegister_8_27

_vdpReadRegister_0_7:
	ld	de, #RG0SAV
	add	hl, de
	ld	a, (hl)
	ret

_vdpReadRegister_8_27:
	cp	#25
	jr	nc, _vdpReadRegister_25_27

_vdpReadRegister_8_24:
	ld	de, #RG8SAV - #8
	add	hl, de
	ld	a, (hl)
	ret

_vdpReadRegister_25_27:
	ld	de, #RG25SAV - #25
	add	hl, de
	ld	a, (hl)
	ret

;------------------------------------------------------------------------------
; u8 vdpReadStatusRegister0()
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpReadStatusRegister0

_vdpReadStatusRegister0:
	call_main_rom	RDVDP
	ret

;------------------------------------------------------------------------------
; void vdpSetActivePage(u8 page)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetActivePage

_vdpSetActivePage:
	ld	(#ACPAGE),a
	ret

;------------------------------------------------------------------------------
; void vdpSetDisplayPage(u8 page)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetDisplayPage

_vdpSetDisplayPage:
	ld	(#DPPAGE),a
	rrca
	rrca
	rrca
	and	#0b01100000
	or	#0b00011111
	ld	(#RG2SAV),a

	ld	c, #0x99	; c = port#1 (write control register)
	di
	out	(c), a
	ld	a, #0x80 + 2	; R2 = a
	out	(c), a
	ei
	ret

;------------------------------------------------------------------------------
; void vdpSetPage()
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetPage

_vdpSetPage:
	call_sub_rom	SETPAG
	ret

;------------------------------------------------------------------------------
; vdpReadVram(u16 address)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpReadVram

_vdpReadVram:
	call_main_rom	NRDVRM
	ret

;------------------------------------------------------------------------------
; void vdpWriteVram(u16 address, u8 data)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpWriteVram

_vdpWriteVram:
	pop	de	; de = return address
	pop	bc	; c = data
	ld	a,c
	dec	sp
	push	de	; store return address
	call_main_rom	NWRVRM
	ret

;------------------------------------------------------------------------------
; void vdpFillVram(u16 address, u16 size, u8 data)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpFillVram

_vdpFillVram:
	exx
	pop	de	; de = return address
	pop	bc	; c = data
	ld	a,c
	dec	sp
	push	de	; store return address
	exx
	ld	b,d	; bc = size
	ld	c,e
	call_main_rom	BIGFIL
	ret

;------------------------------------------------------------------------------
; void vdpCopyToVram(const void* ram, u16 vram, u16 size)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpCopyToVram

_vdpCopyToVram:
	ex	de, hl
	call_main_rom	NSTWRT
	ex	de, hl

	exx
	ld	a,  (#EXPTBL)
	ld	hl, #0x0007	; a = port#0 (write)
	call	RDSLT

	pop	de	; de' = return address
	pop	bc	; bc' = size
	push	de	; store return address
	exx

	ld	c, a	; c = port #0 (write)

	exx
	ld	a, b
	exx

_vdpCopyToVram_256:
	or	a	; a = size >> 8
	jp	z, _vdpCopyToVram_last

	ld	b, a
	otir

	dec	a
	jp	_vdpCopyToVram_256

_vdpCopyToVram_last:
	exx
	ld	a, c
	exx
	or	a	; a = size & 0xff
	ret	z

	ld	b, a
	otir

	ret

;------------------------------------------------------------------------------
; void vdpCopyFromVram(u16 vram, void* ram, u16 size)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpCopyFromVram

_vdpCopyFromVram:
	exx
	pop	de	; de' = return address
	pop	bc	; bc' = size
	ld	a,b
	push	de	; store return address
	exx
	ld	b,a	; bc = bc'
	exx
	ld	a,c
	exx
	ld	c,a
	call_main_rom	LDIRMV
	ret

;------------------------------------------------------------------------------
; void vdpSetScreenMode(u8 screenMode)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetScreenMode

_vdpSetScreenMode:
	call_main_rom	CHGMOD
	ret

;------------------------------------------------------------------------------
; void vdpSetScreenColor(u8 foregroundColor, u8 backgroundColor, u8 borderColor)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetScreenColor

_vdpSetScreenColor:
	pop	de	; de' = return address
	pop	bc	; c = borderColor
	dec	sp
	push	de	; store return address
	ld	(#FORCLR),a
	ld	a,l
	ld	(#BAKCLR),a
	ld	a,c
	ld	(#BDRCLR),a
	call_main_rom	CHGCLR
	ret

;------------------------------------------------------------------------------
; void vdpClearSprite()
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpClearSprite

_vdpClearSprite:
	call_main_rom	CLRSPR
	ret

;------------------------------------------------------------------------------
; void vdpSetSpriteMode(enum vdpSpriteMode spriteMode)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetSpriteMode

_vdpSetSpriteMode:
	and	#0b00000011
	ld	c, a

	ld	a, (#RG1SAV)
	and	#0b11111100
	or	c

	ld	b, a
	ld	c, #0x01
	call_main_rom	WRTVDP 	
	ret

;------------------------------------------------------------------------------
; void vdpSetLogicalOperation(u8 code)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetLogicalOperation

_vdpSetLogicalOperation:
	ld	(#LOGOPR), a
	ret

;------------------------------------------------------------------------------
; void vdpSetForegroundColor(u8 color)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetForegroundColor

_vdpSetForegroundColor:
	ld	(#FORCLR), a
	ret

;------------------------------------------------------------------------------
; void vdpSetBackgroundColor(u8 color)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetBackgroundColor
_vdpSetBackgroundColor:
	ld	(#BAKCLR), a
	ret

;------------------------------------------------------------------------------
; void vdpSetGraphicAccumlator(u16 x, u16 y)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetGraphicAccumlator
_vdpSetGraphicAccumlator:
	ld	(#GRPACX), hl
	ex	de, hl
	ld	(#GRPACY), hl
	ret

;------------------------------------------------------------------------------
; void vdpPrintChar(char c)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpPrintChar

_vdpPrintChar:
	call_main_rom	GRPPRT
	ret

;------------------------------------------------------------------------------
; void vdpPrint(char *str)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpPrint

_vdpPrint:
	ld	a,(hl)
	or	a
	ret	z
	push	hl
	call_main_rom	GRPPRT
	pop	hl
	inc	hl
	jp	_vdpPrint

;------------------------------------------------------------------------------
; void vdpPrintHexU8(u8 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpPrintHexU8
	.globl	_msxDosPrintHexU8_ToString

_vdpPrintHexU8:
	push	af
	rra
	rra
	rra
	rra
	and	#0x0f
	ld	b,#0x00
	ld	c,a
	ld	hl,#_msxDosPrintHexU8_ToString
	add	hl,bc
	ld	a,(hl)
	call_main_rom	GRPPRT
	pop	af

	and	#0x0f
	ld	b,#0x00
	ld	c,a
	ld	hl,#_msxDosPrintHexU8_ToString
	add	hl,bc
	ld	a,(hl)
	call_main_rom	GRPPRT
	ret

;------------------------------------------------------------------------------
; void vdpPrintHexU16(u16 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpPrintHexU16

_vdpPrintHexU16:
	push	hl
	ld	a,h
	call	_vdpPrintHexU8
	pop	hl

	ld	a,l
	call	_vdpPrintHexU8
	ret

;------------------------------------------------------------------------------
; void vdpPrintHexU32(u32 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpPrintHexU32

_vdpPrintHexU32:
	push	hl
	push	de
	ld	a,h
	call	_vdpPrintHexU8
	pop	de
	pop	hl

	push	de
	ld	a,l
	call	_vdpPrintHexU8
	pop	de

	push	de
	ld	a,d
	call	_vdpPrintHexU8
	pop	de

	ld	a,e
	call	_vdpPrintHexU8
	ret

;------------------------------------------------------------------------------
; void vdpSetPalette(u8 offset, u8 size, u8* colors)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_vdpSetPalette

_vdpSetPalette:
	pop	bc		; bc = return address
	pop	de		; de = colors
	push	bc		; store return address

	push	de		; de = colors
	push	hl		; l = size
	push	af		; a = offset

	ld	a, (#EXPTBL)
	ld	hl, #0x0007	; a = port#0 (write vram)
	call	RDSLT

	ld	c, a
	inc	c		; c = port#1 (write control register)
	pop	af

	and	#0x0f
	out	(c), a
	ld	a, #0x80 + #16	; #16 palette select register.
	out	(c), a

	inc	c		; c = port#2 (write palette register)
	
	pop	hl
	ld	b, l		; b = size
	sla	b		; b *= 2

	pop	hl		; hl = colors
	otir

	ret
