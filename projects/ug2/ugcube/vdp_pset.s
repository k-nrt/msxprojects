;------------------------------------------------------------------------------
; vdp pset
;------------------------------------------------------------------------------
	.include	"vdp_defs.s"

;------------------------------------------------------------------------------
; h = x
; l = y
; _ACPAGE = active page
; _FORCLR = color
; _LOGOPR = logical operation
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	VDPPSet
VDPPSet:
	ld	a,(#_VDPWrite)
	inc	a
	ld	c,a		; issue vdp command

	ld	a,#36		; reg R#36
	di	
	out	(c),a
	ld	a,#0x80+#17	; set auto inc mode
	out	(c),a

	inc	c
	inc	c

VDPPSet_dx:
	out	(c),h		; R#36 dxl = h
	xor     a
	out	(c),a		; R#37 dxh = 0

VDPPSet_dy:

	out	(c),l		; R#38 dyl = l

	ld	a,(_ACPAGE)	; active page
	out	(c),a		; R#39 dyh = active page

VDPPSet_clr:
	out	(c),a		; R#40 nxl
	out	(c),a		; R#41 nyh
	out	(c),a		; R#42 nyl
	out	(c),a		; R#43 nyh

	ld      a,(_FORCLR)	; R#44 clr = foreground color
	out     (c),a

VDPPSet_arg:
	xor	a
	out	(c),a		; R#45 arg = 0

VDPPSet_cmr:
	ld	a,(_LOGOPR)
	or	#0x50		; pset command
	out	(c),a		; R#46 cmr = 0x50 | _LOGOPR
	ei
	ret

;------------------------------------------------------------------------------
; void VDPPSet(u8 x, u8 y);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPPSet

_VDPPSet:
	ld	hl,#0x0002
	add	hl,sp

	ld	d,(hl)		; d = x
	inc	hl
	ld	e,(hl)		; e = y
	ex	de,hl		; h = x, l = y

	call	VDPPSet

	ret
	