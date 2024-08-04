;------------------------------------------------------------------------------
; VDP Command.
;------------------------------------------------------------------------------
	EXPTBL	= 0xFCC1	; Main ROM slot / ext slot [4].
	RDSLT	= 0x000C	; Read a byte from specified slot.
;------------------------------------------------------------------------------
; void VDPInit(void)
;------------------------------------------------------------------------------
	.globl	_VDPRead
	.globl	_VDPWrite
	.globl	_VDPInit

	.area	_CODE

_VDPInit:
	ld	a,#>_VDPRead		; VDPRead == 0x0006 ?
	or	a
	jp	nz,_VDPInit_LoadAddress
	ld	a,#<_VDPRead
	cp	#0x06
	jp	nz,_VDPInit_LoadAddress

	ld	a,#>_VDPWrite		; VDPWrite == 0x0007 ?
	or	a
	jp	nz,_VDPInit_LoadAddress
	ld	a,#<_VDPWrite
	cp	#0x07
	jp	nz,_VDPInit_LoadAddress
	ret

_VDPInit_LoadAddress:
					; VDPRead != 0x0006 && VDPWrite != 0x0007
					; store VDP port from MAIN-ROM
	ld	a,(#EXPTBL)		; a = EXPTBL[0] <---- MAIN-ROM slot
	ld	hl,#0x0006
	call	RDSLT
	ld	(#_VDPRead),a

	ld	a,(#EXPTBL)
	ld	hl,#0x0007
	call	RDSLT
	ld	(#_VDPWrite),a
	ret

;------------------------------------------------------------------------------
; void VDPSetPage(u8 nPage);
;------------------------------------------------------------------------------
	.area	_DATA

	.globl	_DFPAGE
	.globl	_ACPAGE
	.globl	_FORCLR

;------------------------------------------------------------------------------
; void VDPSetDisplayPage(u8 nPage);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPSetDisplayPage

_VDPSetDisplayPage:
	ld	(_DFPAGE),a	; a = nPage
	and	#0x03
	rrca
	rrca
	rrca
	or	#0x1f
	ld	e,a		; e = R#2 register value
	
	ld	a,(#_VDPWrite)
	inc	a
	ld	c,a		; c = write control register

	di
	out	(c),e		; send data
	ld	a,#0x80+#0x02	; R#2 = pattern name table address
	out	(c),a		; select register
	ei

	ret
;------------------------------------------------------------------------------
; void VDPWait()
; VDP コマンド終了待ち.
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPWait

_VDPWait:
	di
VDPWait_Loop:
	ld	a,(#_VDPWrite)
	inc	a
	ld	c,a		; c = write control register
	ld	a,#0x02		; S#2 = vdp command status
	out	(c),a
	ld	a,#0x80+#0x0f	; R#15 = read status register
	out	(c),a
	ld	a,(#_VDPRead)
	inc	a
	ld	c,a		; c = read status register 
	in	a,(c)
	and	#0x01
	jp	nz,VDPWait_Loop

	ld	a,(#_VDPWrite)
	inc	a
	ld	c,a		; c = write control register
	xor	a		; S#0 = interrupt status
	out	(c),a
	ld	a,#0x80+#0x0f	; R#15 = read status register
	out	(c),a
	ld	a,(#_VDPRead)
	inc	a
	ld	c,a		; c = read status register 
 	in	a,(c)
	ei
	
	ret
;------------------------------------------------------------------------------
; extern void VDPWaitLine( u8 sx, u8 sy, u8 ex, u8 ey, u8 nColor, u8 nPage ); 
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPWaitLine
	.globl	_VDPWaitLine_HL_DE
	.globl	VDP_HLDE_LINE
	
_VDPWaitLine:
	ld		hl,#2
	add		hl,sp
	ld		b,(hl)			; sx
	inc		hl
	ld		c,(hl)			; sy
	inc		hl
	ld		d,(hl)			; ex
	inc		hl
	ld		e,(hl)			; ey

;	inc		hl
;	ld		a,(hl)			; color
;	ld		(VDP_LINE_COLOR),a
;	inc		hl
;	ld		a,(hl)			; page
;	ld		(VDP_LINE_PAGE),a

	ld		h,b			; h = sx
	ld		l,c			; l = sy

; draw line (h,l) to (d,e)
_VDPWaitLine_HL_DE:
VDP_HLDE_LINE:

SET_ARG:
	ld		b,#0		; b = arg

SET_DIX:
	ld		a,d
	sub		h			; a = ex - sx
	jr		c,SET_DIX1
	ld		d,a			; d = width
	jr		SET_DIX_END	
SET_DIX1:
	set		2,b			; dix = 1 :left
	neg
	ld		d,a			; d = width
SET_DIX_END:

SET_DIY:	
	ld		a,e
	sub		l			; a = ey - sy
	jr		c,SET_DIY1
	ld		e,a			; e = height
	jr		SET_DIY_END
SET_DIY1:
	set		3,b			; diy = 1 :up
	neg	
	ld		e,a			; e = height
SET_DIY_END:

SET_MAJ:
	ld		a,e
	cp		d
	jr		c,SET_MAJ_END
	set		0,b			; maj = 1
	ld		a,d
	ld		d,e
	ld		e,a		
SET_MAJ_END:

WAIT_VDP:
	call	_VDPWait	; wait last vdp command

ISSUE_VDP_COMMAND:
	ld		a,(#_VDPWrite)
	inc		a
	ld		c,a			; issue vdp command
	ld		a,#36			; reg #36
	di
	out		(c),a
	ld		a,#0x80+#17		; set auto inc mode
	out		(c),a
	inc		c
	inc		c
	
CODE_DXL:
	out		(c),h	;36 dxl
	ld		h,#0
	out		(c),h	;37 dxh

CODE_DYL:
	out		(c),l	;38 dyl

CODE_DYH:					; page
	ld		a,(_ACPAGE)
	out		(c),a	;39 dyh
	
CODE_MAJ:
	out		(c),d	;40 majl
	out		(c),h	;41 majh

CODE_MIN:
	out		(c),e	;42 minl
	out		(c),h	;43 minh

CODE_CLR:
	ld		a,(_FORCLR)
	out		(c),a	;44 clr

CODE_ARG:
	out		(c),b	;45 arg

	ld		a,#0x70+#0
	out		(c),a		;46 cmr
	ei
	
	ret

;------------------------------------------------------------------------------
; extern void VDPWaitLine2( u8 sx, u8 sy, u8 ex, u8 ey, u8 nColor, u8 nPage ); 
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPWaitLine2
	
	;.globl	VDP_HLDE_LINE2
	
_VDPWaitLine2:
	ld		hl,#2
	add		hl,sp
	ld		b,(hl)			; sx
	inc		hl
	ld		c,(hl)			; sy
	inc		hl
	ld		d,(hl)			; ex
	inc		hl
	ld		e,(hl)			; ey

	ld		h,b			; h = sx
	ld		l,c			; l = sy


	;bit		7,h
	;ret		nz

	;bit		7,d
	;ret		nz

	;sla		h
	;sla		d

;	call	VDP_HLDE_LINE
;	ret

; draw line (h,l) to (d,e)

L2_SET_ARG:
	ld		b,#0		; b = arg

L2_SET_DIX:
	ld		a,d
	sub		h			; a = ex - sx
	jr		c,L2_SET_DIX1
	ld		d,a			; d = width
	jr		L2_SET_DIX_END	
L2_SET_DIX1:
	set		2,b			; dix = 1 :left
	neg
	ld		d,a			; d = width
L2_SET_DIX_END:

L2_SET_DIY:	
	ld		a,e
	sub		l			; a = ey - sy
	jr		c,L2_SET_DIY1
	ld		e,a			; e = height
	jr		L2_SET_DIY_END
L2_SET_DIY1:
	set		3,b			; diy = 1 :up
	neg	
	ld		e,a			; e = height
L2_SET_DIY_END:

L2_SET_MAJ:
	sla		d
	jr		c,L2_SET_MAJH

	ld		a,#0
	ld		(_L2_MAJH),a

	ld		a,e
	cp		d
	jr		c,L2_SET_MAJ_END
	set		0,b			; maj = 1
	ld		a,d
	ld		d,e
	ld		e,a		
	jr		L2_SET_MAJ_END

L2_SET_MAJH:
	ld		a,#1
	ld		(_L2_MAJH),a

L2_SET_MAJ_END:

L2_WAIT_VDP:
	call	_VDPWait	; wait last vdp command

L2_ISSUE_VDP_COMMAND:
	ld		c,#0x98			; issue vdp command
	inc		c
	ld		a,#36			; reg #36
	di
	out		(c),a
	ld		a,#0x80+#17		; set auto inc mode
	out		(c),a
	inc		c
	inc		c
	
	sla		h
	jr		c,L2_SET_DXH_1
	out		(c),h	;36 dxl
	ld		h,#0
	out		(c),h	;37 dxh
	jr		L2_SET_DY
L2_SET_DXH_1:
	out		(c),h	;36 dxl
	ld		h,#1
	out		(c),h	;37 dxh

L2_SET_DY:
	out		(c),l	;38 dyl

	ld		a,(_ACPAGE)
	out		(c),a	;39 dyh
	
	out		(c),d	;40 majl
	ld		a,(_L2_MAJH)
	out		(c),a	;41 majh

	out		(c),e	;42 minl
	xor		a
	out		(c),a	;43 minh

	ld		a,(_FORCLR)
	out		(c),a	;44 clr

	out		(c),b	;45 arg

	ld		a,#0x70+#0
	out		(c),a		;46 cmr
	ei
	
	ret

	.area	_INITIALIZED
_L2_MAJH:
	.ds		1

;------------------------------------------------------------------------------
; extern void VDPFill( u8 x, u8 y, u16 w, u16 h ); 
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPFill

_VDPFill:
	ld		hl,#2
	add		hl,sp

	ld		c,#0x98
	inc		c
	ld		a,#36

	di
	out		(c),a
	ld		a,#0x80+#17
	out		(c),a

	inc		c
	inc		c
	ld		a,(hl)	; x
	out		(c),a	;36
	ld		a,#0
	out		(c),a	;37

	inc		hl
	ld		a,(hl)	; y
	out		(c),a	;38

	ld		a,(_ACPAGE)	; nPage
	out		(c),a	;39
	
	inc		hl;
	ld		a,(hl)	; wl
	out		(c),a	;40

	inc		hl;
	ld		a,(hl)	; wh
	out		(c),a	;41
	
	inc		hl;
	ld		a,(hl)	; hl
	out		(c),a	;42
	inc		hl;
	ld		a,(hl)	; hh
	out		(c),a	;43
	
	ld		a,(_FORCLR)	; nColor
	out		(c),a	;44
	
	ld		a,#0
	out		(c),a	;45
	
	ld		a,#0xc0
	out		(c),a	;45
	ei
	
	ret



;------------------------------------------------------------------------------
; extern void VDPPrintU8DCore(u8 value)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPPrintU8DCore

	;GRPACX = 0xFCB7;
	;GRPACY = 0xFCB9;

_VDPPrintU8DCore:
	ld		hl,#2
	add		hl,sp

	;ld		a,(hl)
	;ld		(GRPACX),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACX+1),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACY),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACY+1),a

	;inc		hl
	ld		e,(hl)
	ld		d,#0
	ex		de,hl

	push	ix

	ld		a,#'2'
	ld		de,#200
	jr		EVAL_100

;------------------------------------------------------------------------------
; extern void VDPPrintU16DCore(u16 value)
;------------------------------------------------------------------------------
	EXTROM=0x015F
	GRPPRT=0x0089

	.area	_CODE 
	.globl	_VDPPrintU16DCore

_VDPPrintU16DCore:
	ld		hl,#2
	add		hl,sp

	;ld		a,(hl)
	;ld		(GRPACX),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACX+1),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACY),a

	;inc		hl
	;ld		a,(hl)
	;ld		(GRPACY+1),a

	;inc		hl
	ld		e,(hl)
	inc		hl
	ld		d,(hl)
	ex		de,hl

	push	ix

	ld		a,#'6'
	ld		de,#60000
	ld		bc,#10000
	call	EVAL_10000_Loop

	ld		a,#'9'
	ld		de,#9000
	ld		bc,#1000
	call	EVAL_10000_Loop

	ld		a,#'9'
	ld		de,#900

EVAL_100:
	ld		bc,#100
	call	EVAL_10000_Loop

	ld		a,#'9'
	ld		de,#90
	ld		bc,#10
	call	EVAL_10000_Loop

	ld		a,#'0'
	add		l
	ld		ix,#GRPPRT
	di
	call	EXTROM
	ei

	pop		ix
	ret

EVAL_10000_Loop:
	or		a
	push	hl
	sbc		hl,de	; hl < de 
	jr		nc,EVAL_10000_Print

	pop		hl

	or		a
	ex		de,hl
	sbc		hl,bc
	ex		de,hl
	dec		a
	jr		EVAL_10000_Loop

EVAL_10000_Print:
	pop		de
	ld		ix,#GRPPRT
	di
	call	EXTROM
	ei
	ret

;------------------------------------------------------------------------------
; extern void VDPPrintU8XCore(u8 value);
; extern void VDPPrintU16XCore(u16 value);
;------------------------------------------------------------------------------
	.area	_CODE 
	.globl	_VDPPrintU8XCore
	.globl	_VDPPrintU16XCore

_VDPPrintU8XCore:
	ld		hl,#2
	add		hl,sp
	push	ix

	ld		c,(hl)
	call	VDP_PRINT_HEX

	pop		ix
	ret

_VDPPrintU16XCore:
	ld		hl,#2
	add		hl,sp
	push	ix

	ld		b,(hl)
	inc		hl
	ld		c,(hl)

	push	bc
	call	VDP_PRINT_HEX

	pop		bc

	ld		c,b
	call	VDP_PRINT_HEX

	pop		ix
	ret

VDP_PRINT_HEX:
	ld		a,c
	rra
	rra
	rra
	rra
	and		#0x0f
	ld		d,#0
	ld		e,a
	ld		hl,#VDP_PRINT_X_TABLE
	add		hl,de
	ld		a,(hl)
	exx
	ld		ix,#GRPPRT
	di
	call	EXTROM
	ei
	exx

	ld		a,c
	and		#0x0f
	ld		d,#0
	ld		e,a
	ld		hl,#VDP_PRINT_X_TABLE
	add		hl,de
	ld		a,(hl)
	ld		ix,#GRPPRT
	di
	call	EXTROM
	ei

	ret

VDP_PRINT_X_TABLE:
	.ascii		"0123456789ABCDEF"

;------------------------------------------------------------------------------[eof]
