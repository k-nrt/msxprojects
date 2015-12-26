;------------------------------------------------------------------------------
; SLOT.
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; extern u8 SLOTRead(u8 iSlot, u16 nAddress);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_SLOTReadDI

	RDSLT	= 0x000c

_SLOTReadDI:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; iSlot
	inc		hl
	ld		e,(hl)
	inc		hl
	ld		d,(hl)	; nAddress
	ex		de,hl
	call	RDSLT
;	ei
	ld		l, a	; result
	ret

;------------------------------------------------------------------------------
; extern void SLOTWrite(u8 iSlot, u16 nAddress, u8 nParam);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_SLOTWriteDI

	WRSLT	= 0x0014

_SLOTWriteDI:
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
	call	WRSLT
;	ei
	ret

;------------------------------------------------------------------------------
; extern void SLOTEnable(u8 iSlot, u16 nAddress);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_SLOTEnableDI

	ENASLT	= 0x0024

_SLOTEnableDI:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; iSlot
	inc		hl
	ld		e,(hl)
	inc		hl
	ld		d,(hl)	; nAddress
	ex		de,hl
	call	ENASLT
;	ei
	ret

;------------------------------------------------------------------------------
; extern u8 SLOTReadPrimaryRegister();
; extern SlotInfo SLOTGetSlotInfo(u16 address);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_SLOTGetSlotInfo

	RSLREG	= 0x0138		;read primary slot select register

	;. EXPTBL(FCC1H, 4)
	;. 内容	拡張スロット用のフラグテーブル。各スロットの拡張の有無
	EXPTBL	= 0xfcc1

	;. SLTTBL(FCC5H, 4)
	;.	内容	各拡張スロットレジスタ用の、現在のスロット選択状況
	SLTTBL	= 0xfcc5

_SLOTGetSlotInfo:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; address low
	inc		hl
	ld		a,(hl)	; address high
	rrca
	rrca
	rrca
	rrca
	rrca
	rrca			; a >>= 6
	and		#0x03
	ld		b,a		; b = page
	
	call	RSLREG		;read primary slot #
	ld		e,a

	xor		a
SHIFT_SLOT_REGISTER:
	cp		b
	jr		z, FIND_EXPANSION

	rrc		e
	rrc		e

	dec		b
	jr		SHIFT_SLOT_REGISTER

FIND_EXPANSION:
	ld		a,e		
	and		a, #0x03	; a = primary slot index
	
	ld		c,a
	ld		b,#0x00
	ld		hl,#EXPTBL	;see if this slot is expanded or not
	add		hl,bc

	ld		e, a		; e = save primary slot #
	ld		a,(hl)		; a = get the slot is expanded or not
	and		#0x80		; 1000000
	or		e			; set MSB if so
	ld		e,a			; save it to [e]

	ld		hl,#SLTTBL
	add		hl,bc
	ld		a,(hl)		; a = get what  is  currently  output
						; to expansion   slot    register
	and		#0x0c		; 00001100
	or		e			; a = Finaly form slot address
	ld		l,a
	
	ret


;------------------------------------------------------------------------------
