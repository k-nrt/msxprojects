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
	ld	h,#0
	ld	l,#0

	ld	d,#0
	bit	7,e
	jr	z,MulCore_AxE_HL_Test_A
	dec	d							; expand sign bit

MulCore_AxE_HL_Test_A:
	bit	7,a
	jr	z,MulCore_AxE_HL_Loop		; A multiply lower 8 bit only

	neg				; A = -A

MulCore_AxE_HL_A_Minus_Loop:			; multiply A lower 8 bit
	or	a
	jr	z, MulCore_AxE_HL_A_Minus_End

	srl	a							; a >>= 1
	jr	nc,MulCore_AxE_HL_A_Minus_Next

	add	hl,de

MulCore_AxE_HL_A_Minus_Next:
	sla	e
	rl	d							; de <<= 1
	jr	MulCore_AxE_HL_A_Minus_Loop

MulCore_AxE_HL_A_Minus_End:
	ld	a,h							; HL = -HL
	cpl
	ld	h,a
	ld	a,l
	cpl
	ld	l,a
	inc	hl
	ret

MulCore_AxE_HL_Loop:
	or	a
	ret	z

	srl	a							; a >>= 1
	jr	nc,MulCore_AxE_HL_Next

	add	hl,de

MulCore_AxE_HL_Next:
	sla	e
	rl	d							; de <<= 1
	jr	MulCore_AxE_HL_Loop

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
; MulCoreUnsigned_BxDE_AHL
; AHL = B x DE
; in B
; in DE
; out AHL
; use C A'
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	MulCoreUnsigned_BxDE_AHL

MulCoreUnsigned_BxDE_AHL:
	xor	a
	ld	h,a
	ld	l,a
	ld	c,a
	ex	af,af'			; push a

	; AHL = CDE x B

MulCoreUnsigned_BxDE_AHL_Loop:
	ld	a,b			; if b == 0 then end
	or	a
	jr	z,MulCoreUnsigned_BxDE_AHL_End
	;ld	a,d			; if cde == 0 then end
	;or	e
	;or	c
	;jr	z,MulCoreUnsigned_BxDE_AHL_End		

	srl	b
	jr	nc,MulCoreUnsigned_BxDE_AHL_Next
 
	ex	af,af'			; pop a
	add	hl,de			; ahl += cde
	adc	a,c
	ex	af,af'			; push a

MulCoreUnsigned_BxDE_AHL_Next:
	sla	e			; cde <<= 1
	rl	d
	rl	c
	jr	MulCoreUnsigned_BxDE_AHL_Loop

MulCoreUnsigned_BxDE_AHL_End:
	ex	af,af'			; pop a
	ret

;------------------------------------------------------------------------------
; MulCoreUnsigned_BCxDE_HLHL
; HL'HL = BC x DE
; in	BC
; in	DE
; out	HL' high
;	HL low
; use	A
;	DE'
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	MulCoreUnsigned_BCxDE_HLHL
MulCoreUnsigned_BCxDE_HLHL:
	xor	a
	ld	h,a	; hl = 0
	ld	l,a
	exx
	ld	h,a	; hl' = 0
	ld	l,a	;
	ld	d,a	; de' = 0
	ld	e,a
	exx

MulCoreUnsigned_BCxDE_HLHL_Loop:
	ld	a,b	; bc == 0 ?
	or	c
	ret	z	; hl'hl = result

	srl	b	; bc >>= 1
	rr	c
	jp	nc,MulCoreUnsigned_BCxDE_HLHL_Next

	add	hl,de	; hl'hl += de'de
	exx
	adc	hl,de
	exx

MulCoreUnsigned_BCxDE_HLHL_Next:
	sla	e	; de'de <<= 1
	rl	d
	exx
	rl	e
	rl	d
	exx
	jp	MulCoreUnsigned_BCxDE_HLHL_Loop

;------------------------------------------------------------------------------
; MulCoreSigned_BCxDE_HLHL
; HL'HL = BC x DE
; in	BC
; in	DE
; out	HL' high
;	HL low
; use	A, DE'
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	MulCoreSigned_BCxDE_HLHL

MulCoreSigned_BCxDE_HLHL:
	bit	7,b
	jp	z,MulCoreSigned_BCxDE_HLHL_BCPlus

MulCoreSigned_BCxDE_HLHL_BCMinus:
	ld	a,b	; bc = -bc
	cpl
	ld	b,a
	ld	a,c
	cpl
	ld	c,a
	inc	bc

	bit	7,d
	jp	z,MulCoreSigned_BCxDE_HLHL_BCMinus_DEPlus

MulCoreSigned_BCxDE_HLHL_BCMinus_DEMinus:
	ld	a,d	; de = -de
	cpl
	ld	d,a
	ld	a,e
	cpl
	ld	e,a
	inc	de
	jp	MulCoreUnsigned_BCxDE_HLHL

MulCoreSigned_BCxDE_HLHL_BCMinus_DEPlus:
	call	MulCoreUnsigned_BCxDE_HLHL
	jp	MulCoreSigned_BCxDE_HLHL_NegHLHL

MulCoreSigned_BCxDE_HLHL_BCPlus:
	bit	7,d

MulCoreSigned_BCxDE_HLHL_BCPlus_DEPlus:
	jp	z,MulCoreUnsigned_BCxDE_HLHL

MulCoreSigned_BCxDE_HLHL_BCPlus_DEMinus:
	ld	a,d	; de = -de
	cpl
	ld	d,a
	ld	a,e
	cpl
	ld	e,a
	inc	de
	call	MulCoreUnsigned_BCxDE_HLHL

MulCoreSigned_BCxDE_HLHL_NegHLHL:
	xor	a	; a = 0, CY = 0
	ld	d,a	; de = 0
	ld	e,a
	ex	de,hl	; hl = 0 - hl
	sbc	hl,de

	exx		; hl' = 0 - hl' - cy
	ld	d,a
	ld	e,a
	ex	de,hl
	sbc	hl,de
	exx
	ret
;------------------------------------------------------------------------------
