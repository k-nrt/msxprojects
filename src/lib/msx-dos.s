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
; System scratch area
;------------------------------------------------------------------------------
	.area	_SYSTEM_SCRATCH (ABS)

	.org	0x005c
	.globl	_msxDosDefaultFCB0
_msxDosDefaultFCB0:

	.org	0x006c
	.globl	_msxDosDefaultFCB1

_msxDosDefaultFCB1:

;------------------------------------------------------------------------------
; void msxDosReset()
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosReset

_msxDosReset:
	ld	c, #0x00
	jp	0x0005

;------------------------------------------------------------------------------
; void msxDosPrintChar(char c)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosPrintChar

_msxDosPrintChar:
	push	ix
	ld	e,a
	ld	c, #0x02
	call	0x0005
	pop	ix
	ret

;------------------------------------------------------------------------------
; void msxDosPrint(const char *str)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosPrint

_msxDosPrint:
	push	ix

_msxDosPrint_Loop:
	ld	a,(hl)
	or	a
	jp	nz, _msxDosPrint_Print
	pop	ix
	ret

_msxDosPrint_Print:
	push	hl
	ld	e,a
	ld	c, #0x02
	call	0x0005
	pop	hl
	inc	hl
	jp	_msxDosPrint_Loop

;------------------------------------------------------------------------------
; void msxDosPrintHexU8(u8 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosPrintHexU8
	.globl	_msxDosPrintHexU8_ToString

_msxDosPrintHexU8:
	push	ix

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
	ld	e,(hl)
	ld	c, #0x02
	call	0x0005
	pop	af

	and	#0x0f
	ld	b,#0x00
	ld	c,a
	ld	hl,#_msxDosPrintHexU8_ToString
	add	hl,bc
	ld	e,(hl)
	ld	c, #0x02
	call	0x0005

	pop	ix
	ret

_msxDosPrintHexU8_ToString:
	.ascii	"0123456789ABCDEF"

;------------------------------------------------------------------------------
; void msxDosPrintHexU16(u16 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosPrintHexU16

_msxDosPrintHexU16:
	ld	a,h
	push	hl
	call	_msxDosPrintHexU8
	pop	hl
	ld	a,l
	jp	_msxDosPrintHexU8

;------------------------------------------------------------------------------
; void msxDosPrintHexU32(u32 hex)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosPrintHexU32

_msxDosPrintHexU32:
	push	de
	call	_msxDosPrintHexU16
	pop	hl
	jp	_msxDosPrintHexU16

;------------------------------------------------------------------------------
; u8 msxDosOpen(msxDosFCB *fcb)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosOpen

_msxDosOpen:
	push	ix
	ex	de,hl
	ld	c,#0x0f
	call	0x0005
	pop	ix
	ret

;------------------------------------------------------------------------------
; u8 msxDosSetReadAddress(void* address)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosSetReadAddress

_msxDosSetReadAddress:
	push	ix
	ex	de,hl
	ld	c,#0x1a
	call	0x0005
	pop	ix
	ret

;------------------------------------------------------------------------------
; u8 msxDosReadRandomRecord(msxDosFCB *fcb, u16 recordSize)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosReadRandomRecord

_msxDosReadRandomRecord:
	push	ix
	ex	de,hl
	ld	c,#0x27
	call	0x0005
	pop	ix
	ret

;------------------------------------------------------------------------------
; u8 msxDosClose(msxDosFCB *fcb)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosClose

_msxDosClose:
	push	ix
	ex	de,hl
	ld	c,#0x10
	call	0x0005
	pop	ix
	ret

;------------------------------------------------------------------------------
; void msxDosRun(void* run)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxDosRun

_msxDosRun:
	exx
	ld	hl,(0x0006)
	dec	hl
	ld	de,#_msxDosRun_JumpHLEnd
	ex	de,hl
	ld	bc,#_msxDosRun_JumpHLEnd-#_msxDosRun_JumpHL+#1
	lddr
	inc	de
	push	de
	exx
	ret

_msxDosRun_JumpHL:
	push	hl
	ld	a,(EXPTBL)
	ld	h,#0x00
	call	ENASLT
	ret
	nop
_msxDosRun_JumpHLEnd:
	nop

;------------------------------------------------------------------------------
; void msxMemCopy(void* to, const void* from, u16 size)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxMemCopy

_msxMemCopy:
	exx
	pop	de	; de' = return address
	pop	bc	; bc' = size
	ld	a, b
	push	de	; store return address
	exx

	ld	b,a	; bc = bc' = size
	exx
	ld	a,c
	exx
	ld	c,a

	ex	de, hl
	ldir

	ret

;------------------------------------------------------------------------------
; void msxClearKeyBuffer()
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_msxClearKeyBuffer

_msxClearKeyBuffer:
	call_main_rom	KILBUF
	ret
