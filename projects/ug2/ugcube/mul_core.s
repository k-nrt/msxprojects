;------------------------------------------------------------------------------
; MulCore
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	MulCore_AxE_HL
    .globl  MulCore_AxDE_HL

;------------------------------------------------------------------------------
; Mul HL = AxE
; in    A signed
; in    E signed
; out   HL	signed
; use   D,A
;------------------------------------------------------------------------------
MulCore_AxE_HL:
	ld		h,#0
	ld		l,#0

	ld		d,#0
	bit		7,e
	jr		z,MulCore_AxE_HL_Test_A
	dec		d							; expand sign bit

MulCore_AxE_HL_Test_A:
	bit		7,a
	jr		z,MulCore_AxE_HL_Loop		; A multiply lower 8 bit only

	neg									; A = -A

MulCore_AxE_HL_A_Minus_Loop:			; multiply A lower 8 bit
	or		a
	jr		z, MulCore_AxE_HL_A_Minus_End

	srl		a							; a >>= 1
	jr		nc,MulCore_AxE_HL_A_Minus_Next

	add		hl,de

MulCore_AxE_HL_A_Minus_Next:
	sla		e
	rl		d							; de <<= 1
	jr		MulCore_AxE_HL_A_Minus_Loop

MulCore_AxE_HL_A_Minus_End:
	ld		a,h							; HL = -HL
	cpl
	ld		h,a
	ld		a,l
	cpl
	ld		l,a
	inc		hl
	ret

MulCore_AxE_HL_Loop:
	or		a
	ret		z

	srl		a							; a >>= 1
	jr		nc,MulCore_AxE_HL_Next

	add		hl,de

MulCore_AxE_HL_Next:
	sla		e
	rl		d							; de <<= 1
	jr		MulCore_AxE_HL_Loop

;------------------------------------------------------------------------------
; Mul HL = AxDE
; in    A
; in    DE
; out   HL
; use   A
;------------------------------------------------------------------------------
MulCore_AxDE_HL:
    ld      h,#0
    ld      l,#0

MulCore_AxDE_HL_Loop:
	or		a
	ret		z

	srl		a	
	jr		nc,MulCore_AxDE_HL_Next

	add		hl,de

MulCore_AxDE_HL_Next:
	sla		e
	rl		d
	jr		MulCore_AxDE_HL_Loop

;------------------------------------------------------------------------------
