;------------------------------------------------------------------------------
; SinCos
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	LoadSinCosLp_A_DE
	.globl	LoadSinLp_A_L
	.globl	LoadSinHp_A_DE
	.globl	_SinCos_GetSinCosLp
	.globl	_SinCos_GetSinLp
	.globl	_SinCos_GetCosLp
	.globl	_SinCos_GetSinHp
	.globl	_SinCos_GetCosHp
	.globl	_SinCos_RotateXYS8Lp

    .globl  MulCore_AxE_HL

;------------------------------------------------------------------------------
; LoadSinCosLp_A_DE
; load low precision(2:6) sin and cos to DE
; in	A = rot
; out	D = sin 2:6
; out 	E = cos 2:6
; use	HL
;------------------------------------------------------------------------------
LoadSinCosLp_A_DE:
	ld	h,#0x7e
	ld	l,a
	ld	d,(hl)
	add	a,#64
	ld	l,a  
	ld	e,(hl)
	ret

;------------------------------------------------------------------------------
; LoadSinLp_A_L
; load low precision(2:6) sin to A
; in	A = rot
; out	L = sin 2:6
; use	HL
;------------------------------------------------------------------------------
LoadSinLp_A_L:
	ld	h,#0x7e
	ld	l,a
	ld	l,(hl)
	ret

;------------------------------------------------------------------------------
; LoadSinHp_A_DE
; load high precision(2:14) sin to DE
; in	A = rot
; out	DE = sin 2:14
; use	HL
;------------------------------------------------------------------------------
LoadSinHp_A_DE:
	ld	h,#0x7f
	cp	#128
	jr	nc,LoadSinHp_128_255
LoadSinHp_0_127:
	cp	#64
	jr	nc,LoadSinHp_64_127
LoadSinHp_0_63:
	ld	l,a
	sla	l
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ret
LoadSinHp_64_127:
	neg
	add	a,#127  
	sla	a
	ld	l,a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ret
LoadSinHp_128_255: 
	cp	#192
	jr	nc,LoadSinHp_192_255
LoadSinHp_128_191:
	sub	#128
	sla	a
	or	#0x80
	ld	l,a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ret
LoadSinHp_192_255:
	neg
	add	a,#255
	sla	a
	or	#0x80
	ld	l,a
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ret

;------------------------------------------------------------------------------
; extern SinCosLp SinCos_GetSinCosLp(u8 rot);
;------------------------------------------------------------------------------
_SinCos_GetSinCosLp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)		;a = rot
	call	LoadSinCosLp_A_DE
	ex		de,hl
	ret

;------------------------------------------------------------------------------
; extern fp2_6 SinCos_GetSinLp(u8 rot);
;------------------------------------------------------------------------------
_SinCos_GetSinLp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)		;a = rot
	call	LoadSinLp_A_L
	ret

;------------------------------------------------------------------------------
; extern fp2_6 SinCos_GetCosLp(u8 rot);
;------------------------------------------------------------------------------
_SinCos_GetCosLp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)		;a = rot
	add		a,#64
	call	LoadSinLp_A_L
	ret

;------------------------------------------------------------------------------
; extern fp2_14 SinCos_GetSinHp(u8 rot);
;------------------------------------------------------------------------------
_SinCos_GetSinHp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)		;a = rot
	call	LoadSinHp_A_DE
	ex		de,hl
	ret

;------------------------------------------------------------------------------
; extern fp2_14 SinCos_GetCosHp(u8 rot);
;------------------------------------------------------------------------------
_SinCos_GetCosHp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		a,(hl)		;a = rot
	add		a,#64
	call	LoadSinHp_A_DE
	ex		de,hl
	ret

;------------------------------------------------------------------------------
; SinCos_RotateXYLp_A_BC_HL
; Rotate XY low precision
; in A Rot
; in B X
; in C Y
; out H X
; out L Y
; use DE
;------------------------------------------------------------------------------
SinCos_RotateXYLp_A_BC_HL:
	call	LoadSinCosLp_A_DE	; de=sincos

	push	bc
	push	de

SinCos_RotateXYLp_A_BC_HL_Y:
								; Y=X*sin+Y*cos
	ld		a,c					; a=Y
	ld		c,d					; c=sin
	call	MulCore_AxE_HL		; hl=Y*cos
	push	hl
	ld		a,b					; a=X
	ld		e,c					; e=c=sin
	call	MulCore_AxE_HL		; hl=X*sin
	pop		de					; de=Y*cos
	add		hl,de				; hl[10:6]=X*sin+Y*cos
	sla		l					; hl <<= 2
	rl		h
	sla		l
	rl		h					; h=Y

	pop		de					; de=sincos
	pop		bc					; bc=XY
	push	hl

SinCos_RotateXYLp_A_BC_HL_X:
								; X=X*cos-Y*sin
	ld		a,b					; a=X
	ld		b,d					; b=sin
	call	MulCore_AxE_HL		; hl=X*cos
	push	hl
	ld		a,c					; a=Y
	ld		e,b					; e=b=sin
	call	MulCore_AxE_HL		; hl=Y*sin
	pop		de					; de=X*cos
	ex		de,hl				; hl=X*cos de=Y*sin
	or		a					; cy=0
	sbc		hl,de				; hl[10:6]=X*cos-Y*sin
	sla		l					; hl <<= 2
	rl		h
	sla		l
	rl		h					; h=X
	pop		de					; d=Y
	ld		l,d					; hl=XY

	ret

;------------------------------------------------------------------------------
; extern s8XY SinCos_RotateXYS8Lp(s8 x, s8 y, u8 rot);
;------------------------------------------------------------------------------
_SinCos_RotateXYS8Lp:
	ld		hl,#2		;skip return addr.
	add		hl,sp
	ld		b,(hl)
	inc		hl
	ld		c,(hl)
	inc		hl
	ld		a,(hl)
	call	SinCos_RotateXYLp_A_BC_HL
	ret
