;------------------------------------------------------------------------------
; Scaler Math
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	MulCore_AxE_HL
    .globl  MulCore_AxDE_HL

	.globl	_Mul_S8xS8

;------------------------------------------------------------------------------
; extern s16 Mul_S8xS8(s8 x, s8 y);
;------------------------------------------------------------------------------
_Mul_S8xS8:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)
    inc     hl
	ld		e,(hl)
	call	MulCore_AxE_HL
	ret