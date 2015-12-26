;------------------------------------------------------------------------------
; extern u8 Math_MulU8xU8_U8(u8 x, u8 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_Math_MulU8xU8_U8

_Math_MulU8xU8_U8:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		d,(hl)	; x low
	inc		hl
	ld		e,(hl)	; y high

Mul_DxE_A:
	xor		a
	ld		b,#8

Mul_DxE_A_Loop:
	rrc		e
	jr		nc,Mul_DxE_A_Next

	add		a,d

Mul_DxE_A_Next:
	sla		d
	djnz	Mul_DxE_A_Loop

Mul_DxE_A_End:
	ld		l,a
	ret

;------------------------------------------------------------------------------
; extern u16 Math_MulU8xU8_U16(u8 x, u8 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_Math_MulU8xU8_U16

_Math_MulU8xU8_U16:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; a=x
	inc		hl
	ld		e,(hl)	; e=y

	ld		h,#0	; hl = 0
	ld		l,h
	ld		d,h		; d = 0

Mul_AxDE_HL:

Mul_AxDE_HL_Loop:
	or		a
	ret		z

	srl		a	
	jr		nc,Mul_AxDE_HL_Next

	add		hl,de

Mul_AxDE_HL_Next:
	sla		e
	rl		d
	jr		Mul_AxDE_HL_Loop

;------------------------------------------------------------------------------
; extern s16 Math_MulS8xS8_S16(s8 x, s8 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_Math_MulS8xS8_S16

_Math_MulS8xS8_S16:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		a,(hl)	; a=x
	inc		hl
	ld		e,(hl)	; e=y

	ld		h,#0	; hl = 0
	ld		l,h

	bit		7,e
	jr		nz, Mul_S8xS8_S16_DEMinus

Mul_S8xS8_S16_DEPlus:
	ld		d,h			; d = 0
	jr		Mul_AxDE_HL

Mul_S8xS8_S16_DEMinus:
	ld		d,#0xff		; d = 0xff
	jr		Mul_AxDE_HL

;------------------------------------------------------------------------------
; extern u32 Math_MulU8xU16_U32(u8 x, u16 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_Math_MulU8xU16_U32

_Math_MulU8xU16_U32:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		b,(hl)	; x low
	inc		hl
	ld		e,(hl)	; y low
	inc		hl
	ld		d,(hl)	; y high

	xor		a
	ld		c,a		; cde = 0
	ld		h,a		
	ld		l,a		; ahl = 0
	ex		af,af'
	xor		a
	ex		af,af'

Mul_BxCDE_to_AHL:

Mul_BxCDE_to_AHL_Loop:
	ex		af,af'
	cp		b
	jr		z,Mul_BxCDE_to_AHL_End
	ex		af,af'	
	srl		b	
	jr		nc,Mul_BxCDE_to_AHL_Next

	add		hl,de
	adc		a,c

Mul_BxCDE_to_AHL_Next:
	sla		e
	rl		d
	rl		c
	jr		Mul_BxCDE_to_AHL_Loop

Mul_BxCDE_to_AHL_End:
	ld		d,#0
	ld		e,a
	ret

;------------------------------------------------------------------------------
; extern u16 Math_MulU16xU16_U32(u16 x, u16 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_Math_MulU16xU16_U32

_Math_MulU16xU16_U32:
	ld		hl,#2	;skip return addr.
	add		hl,sp

	ld		c,(hl)	; x low
	inc		hl
	ld		b,(hl)	; x high
	inc		hl
	ld		e,(hl)	; y low
	inc		hl
	ld		d,(hl)	; y high

Mul_BCxDE_to_DEHL:

	xor		a
	ld		h,a		; hl'hl=0x00000000
	ld		l,a
	exx
	ld		h,a		; de'=0x0000
	ld		l,a
	ld		d,a
	ld		e,a
	exx

Mul_BCxDE_to_DEHL_Loop:
	ld		a,b		; bc == 0 ?
	or		c
	jr		z,Mul_BCxDE_to_DEHL_End

	srl		b
	rr		c
	jr		nc,Mul_BCxDE_to_DEHL_Next

	add		hl,de
	exx
	adc		hl,de
	exx

Mul_BCxDE_to_DEHL_Next:
	sla		e
	rl		d
	exx
	rl		e
	rl		d
	exx
	jr		Mul_BCxDE_to_DEHL_Loop

Mul_BCxDE_to_DEHL_End:
	exx
	push	hl
	exx
	pop		de
	ret

;------------------------------------------------------------------------------
