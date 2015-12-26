;------------------------------------------------------------------------------
; extern s16 SinCos_GetSin(u8 rot)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_SinCos_GetSin
	.globl _g_s16SinTablePlus

_SinCos_GetSin:
	ld		hl,#2	;skip return addr.
	add		hl,sp
	ld		b, (hl)	; b = rot

	ld		h,#0x7f		 ; hl = 0x7f00

	ld		a, #0x80
	and		b
	jr		nz,Sin_Rot_128_255

Sin_Rot_0_127:
	ld		a, #0x40
	and		b
	jr		nz,Sin_Rot_64_127

Sin_Rot_0_63:
	ld		a,b		; a = rot
	sla		a
	ld		l,a		; hl = 0x7f00[rot]

	ld		e,(hl)
	inc		l
	ld		d,(hl)

	ex		de,hl
	ret	

Sin_Rot_64_127:
	ld		a,#127	; a = 127 - rot
	sub		b	

	sla		a
	ld		l,a		; hl = 0x7f00[127-rot]

	ld		e,(hl)
	inc		l
	ld		d,(hl)

	ex		de,hl
	ret	

Sin_Rot_128_255:
	ld		a, #0x40
	and		b
	jr		nz,Sin_Rot_192_255

Sin_Rot_128_191:
	ld		a,b		; a=rot-128
	sub		#128

	sla		a
	add		a,#0x80
	ld		l,a		; hl = 0x7f80[rot-128]

	ld		e,(hl)
	inc		l
	ld		d,(hl)

	ex		de,hl
	ret	

Sin_Rot_192_255:
	ld		a,#255	; a=255-rot
	sub		b

	sla		a
	add		a,#0x80
	ld		l,a		; hl = 0x7f80[255-rot]

	ld		e,(hl)
	inc		l
	ld		d,(hl)

	ex		de,hl
	ret	
;------------------------------------------------------------------------------
