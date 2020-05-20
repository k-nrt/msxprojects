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
