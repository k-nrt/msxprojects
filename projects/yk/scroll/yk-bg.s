;------------------------------------------------------------------------------
; Macro.
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; vdpWaitCommand
; use : A, C = 0x99
;------------------------------------------------------------------------------
vdpWaitCommand:
	ld	c, #0x99

vdpWaitCommand_Loop:
	ld	a, #0x02	; a = S2
	out	(c), a
	ld	a, #0x80 + #15	; R15 = S2
	out	(c), a
	in	a, (c)

	and	#0x01
	jp	nz, vdpWaitCommand_Loop

	xor	a			; a = S0
	out	(c), a
	ld	a, #0x80 + #15		; R15 = S0
	out	(c), a
	in	a, (c)

	ret

vdpWaitCommandUnsafe:
	ld	c, #0x99

vdpWaitCommandUnsafe_Loop:
	ld	a, #0x02	; a = S2
	out	(c), a
	ld	a, #0x80 + #15	; R15 = S2
	out	(c), a
	in	a, (c)

	and	#0x01
	jp	nz, vdpWaitCommandUnsafe_Loop
	ret

vdpRestoreStatusRegister:
	ld	c, #0x99
	xor	a			; a = S0
	out	(c), a
	ld	a, #0x80 + #15		; R15 = S0
	out	(c), a
	in	a, (c)
	ret



;------------------------------------------------------------------------------
; void ykDrawBg(u8 *map, u16 dy)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_ykDrawBg

_ykDrawBg:
	ld	b, #0x00
	di

_ykDrawBg_Loop:
	ld	a, (hl)
	ld	c, a
	push	hl

	rla				; sx = h = (a << 3) & 0xf8
	rla
	rla
	and	#0xf8
	ld	h, a

	ld	a, c			; sy = l = (a >> 2) & 0x38
	rra
	rra
	and	#0x38
	ld	l, a

	ld	c, #0x99		; c = port#1 (write register)

	call	vdpWaitCommandUnsafe_Loop

	ld	a, #32			; R32
	out	(c), a

	ld	a, #0x80 + #17	; R17 = 32 (auto increment write)
	out	(c),a

	inc	c
	inc	c

	out	(c), h			; R32:SX = h
	xor	a
	out	(c), a

	out	(c), l			; R34:SY = l
	out	(c), a


	out	(c), b			; R36:DX = b
	out	(c), a

	out	(c), e			; R38:DY = de
	out	(c), d

	ld	a, #0x08		; R40:NX = 8
	out	(c), a
	xor	a
	out	(c), a

	ld	a, #0x08		; R42:NX = 8
	out	(c), a
	xor	a
	out	(c), a

	out	(c), a			; R44:CLR = 0

	out	(c), a			; R45:ARG = 0

	ld	a, #0b11010000
	out	(c), a			; R46:CMR = HMMM

	pop	hl
	inc	hl
	ld	a, #0x08
	add	a, b
	ld	b, a
	jp	nc, _ykDrawBg_Loop

	call	vdpRestoreStatusRegister
	ei
	ret
