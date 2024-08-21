;------------------------------------------------------------------------------
; mtk_star
;------------------------------------------------------------------------------
		.include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformUnsignedFast
; fetch transform table only
; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; keep D 3d position z
; out E screen [unsigned]
; out CY 1 = clip, 0 = no clip
; use A
;------------------------------------------------------------------------------
		.globl  PersTransformUnsignedFast

;------------------------------------------------------------------------------
; h = x
; l = y
; _ACPAGE = active page
; _FORCLR = color
; _LOGOPR = logical operation
;------------------------------------------------------------------------------
		.globl	VDPPSet

		.globl	_VDPWait

;------------------------------------------------------------------------------
; MtkStarRender
;------------------------------------------------------------------------------
		.globl      MtkStarRender
		.globl      _g_mtkStars

		.area       _CODE
MtkStarRender:
	ld	a,(#_g_persContext + #m_clipNear)
	ld	e,a					; e' = clip near
	ld	a,(#_g_persContext + #m_s16ScreenX)
	ld	h,a					; h' = screen center x
	ld	a,(#_g_persContext + #m_s16ScreenY)
	ld	l,a					; l' = screen center y
	exx

	ld	b,#32
	ld	hl,#_g_mtkStars

MtkStarRenderLoop:
	ld	e,(hl)		; e = x
	inc	hl
	ld	c,(hl)		; c = y
	inc	hl
	ld	d,(hl)		; d = z
	inc	hl
	push	hl

MtkStarRenderZ:
	ld	a,d	; a = z
	exx
	cp	e	; clip near
	exx
	jp	m,MtkStarRenderNext

MtkStarRenderX:
	ld	a,e	; a = x
	or	a
	jp	p,MtkStarRenderRight

MtkStarRenderLeft:
	neg
	ld	e,a
	Macro_PersTransformUnsignedFast_Left MtkStarRenderNext
	exx
	ld	a,h
	exx
	sub	e	; a = center x - x
	jp	MtkStarRenderY

MtkStarRenderRight:
			; e = x
	Macro_PersTransformUnsignedFast_Right MtkStarRenderNext
	exx
	ld	a,h	; a = center x
	exx
	add	a,e	; a = x + center x

MtkStarRenderY:
	ex	af,af'	; a' = screen x

	ld	a,c	; a = y
	or	a
	jp	p,MtkStarRenderBottom

MtkStarRenderTop:
	neg
	ld	e,a
	Macro_PersTransformUnsignedFast_Top MtkStarRenderNext
	exx
	ld	a,l
	exx
	sub	e
	jp	MtkStarRenderPSet

MtkStarRenderBottom:
	ld	e,a
	Macro_PersTransformUnsignedFast_Bottom  MtkStarRenderNext
	exx
	ld	a,l
	exx
	add	a,e

MtkStarRenderPSet:
	ld	l,a
	ex	af,af'
	ld	h,a	; h = screen x, l = screen y
	
	call	_VDPWait
	call	VDPPSet

MtkStarRenderNext:
	pop	hl
	dec	b
	jp	nz,MtkStarRenderLoop
	ret

;------------------------------------------------------------------------------
; extern MtkStarRender();
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_MtkStarRender

_MtkStarRender:
	call	MtkStarRender
	ret
