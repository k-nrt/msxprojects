;------------------------------------------------------------------------------
; void UgxCopyToVRAM8Internal(void *pDst, const void* pSrc, u8 sizeInBytes)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_UgxCopyToVRAM8Internal

_UgxCopyToVRAM8Internal:
	ld		a,(#0x0007)
	ld		c,a
	inc		c			; write port 1

	ld		hl,#2
	add		hl,sp

	ld		e,(hl)		; e = dst address low
	inc		hl
	ld		d,(hl)		; d = dst address high
	inc		hl

	ld		a,d
	rlca	
	rlca
	and		#0x03		
	or		#0x04		; a = (d >> 6) | 0x04

	di
	out		(c),a		

	ld		a,#0x8e		
	out		(c),a		; write R#14 = a

	ld		a,e
	out		(c),a		; a = dst address low.
	ld		a,d

	and		#0x3f		; a = (d & 0x3f) | 0x40
	or		#0x40		
	out		(c),a

	ld		e,(hl)
	inc		hl
	ld		d,(hl)
	inc		hl
	ld		b,(hl)		; b = size

	dec		c			; write port 0
	ex		de,hl		; hl = src assress.

_UgxCopyToVRAM8Internal_Loop:
	ld		a,(hl)
	out		(c),a
	inc		hl
	djnz	_UgxCopyToVRAM8Internal_Loop;

	ei

	ret

;------------------------------------------------------------------------------
; extern u8 UgxGetValueU8(u8 x, u8 z);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_UgxGetValueU8

_UgxGetValueU8:
	ld		hl,#2
	add		hl,sp
	ld		b,(hl)	; b = x
	inc		hl
	ld		l,(hl)	; l = z

_UgxGetValueU8_BL_L:
	
	ld		c,#0x0F	; [7] c = 0x0F
	ld		a,b		; [4]
	cp		#0x10	; [7]
	jr		NC,_PERSGetValueU8_X_GreaterEqual16 ; [12]

_PERSGetValueU8_X_Less16:
	and		a,c		; [4] a &= 0x0F
	add		a,#0xc0	; [7] a = 0xc0 + (x&0xf)
	ld		h,a		; [4] hl = &pers_x1[x&0xf][z]
	ld		l,(hl)	; [7] l = pers_x1[x&0xf][z]
	ret				; 8x4 + 5x4 = 52

_PERSGetValueU8_X_GreaterEqual16:
	and		a,c		; [4] a &= 0x0F
	add		a,#0xc0	; [7] a = 0xc0 + (x&0xf)
	ld		h,a		; [4] hl = &pers_x1[x&0xf][z]
	ld		e,(hl)	; [7] e = pers_x1[x&0xf][z]

	ld		a,b		; [4] a = x ; cy = 0
	rra				; [4] a = (cy << 8) | (a >> 1);
	rra				; [4]
	rra				; [4]
	rra				; [4]
	and		a,c		; [4] a = (x>>4) & 0x0xf
	add		a,#0xd0	; [7] a = 0xd0 + (x>>4)
	ld		h,a		; [4] hl = &pers_x16[x>>4][z]
	ld		a,e		; [4] a = e = pers_x1[x&0xf][z]
	add		a,(hl)	; [7] a = pers_x1[x&0xf][z] + pers_x16[x>>4][z] 
	jr		c,_PERSGetValueU8_X_Overflow
	ld		l,a		; [4]
	ret				; 8x6 + 5x12 = 48 + 60 = 108

_PERSGetValueU8_X_Overflow:
	ld		l,#0xff
	ret

;------------------------------------------------------------------------------
; extern u8 UgxGetValueU16Internal(u16 u16X, u16 u16Z)
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_UgxGetValueU16Internal

_UgxGetValueU16Internal:
	ld	hl,#2
	add	hl,sp

;pers.c:380: register u16 x = u16X;
	ld	b,(hl)
	inc	hl
	ld	c,(hl)	; cb = x
	inc	hl

;pers.c:381: register u16 z = u16Z;
	ld	e,(hl)
	inc	hl
	ld	a,(hl)
	ld	l,e		; al = z

_UgxGetValueU16Internal_CB_AL_L:

	;pers.c:382: if (x < 256 && z < 256)
		or	c
		jr	nz,_UgxGetValueU16Internal_Shift1

	;pers.c:384: return UgxGetValueU8(x,z);
		; b = x, l = z
		jr	_UgxGetValueU8_BL_L

_UgxGetValueU16Internal_Shift1:
	;pers.c:387: x >>= 1;
		srl	c
		rr	b

	;pers.c:388: z >>= 1;
		srl	a
		rr	l

	;pers.c:390: if (x < 256 && z < 256)
		or	c
		jr	nz,_UgxGetValueU16Internal_Shift2

	;pers.c:392: return UgxGetValueU8(x,z);
		; b = x, l = z
		jr	_UgxGetValueU8_BL_L

_UgxGetValueU16Internal_Shift2:
	;pers.c:395: x >>= 1;
		srl	c
		rr	b
	;pers.c:396: z >>= 1;
		srl	a
		rr	l

	;pers.c:398: if (x < 256 && z < 256)
		or	c
		jr	nz,_UgxGetValueU16Internal_Failure

	;pers.c:400: return UgxGetValueU8(x, z);
		; b = x, l = z
		jr	_UgxGetValueU8_BL_L

_UgxGetValueU16Internal_Failure:
	;pers.c:403: return 0xff;
		ld	l,#0xFF
		ret

;_UgxGetValueU16Internal:
;	push	ix
;	ld	ix,#0
;	add	ix,sp
;
;pers.c:380: register u16 x = u16X;
;	ld	l,4 (ix)
;	ld	h,5 (ix)
;
;pers.c:381: register u16 z = u16Z;
;	ld	e,6 (ix)
;	ld	d,7 (ix)
;
;pers.c:382: if (x < 256 && z < 256)
;	ld	a,h
;	sub	a, #0x01
;	jr	NC,00102$
;	ld	a,d
;	sub	a, #0x01
;	jr	NC,00102$
;
;pers.c:384: return UgxGetValueU8(x,z);
;	ld	b,e
;	ld	c, l
;	push	bc
;	call	_UgxGetValueU8
;	pop	af
;	jr	00110$
;
;00102$:
;pers.c:387: x >>= 1;
;	srl	h
;	rr	l
;pers.c:388: z >>= 1;
;	srl	d
;	rr	e
;pers.c:390: if (x < 256 && z < 256)
;	ld	a,h
;	sub	a, #0x01
;	jr	NC,00105$
;	ld	a,d
;	sub	a, #0x01
;	jr	NC,00105$
;pers.c:392: return UgxGetValueU8(x,z);
;	ld	b,e
;	ld	c, l
;	push	bc
;	call	_UgxGetValueU8
;	pop	af
;	jr	00110$
;00105$:
;pers.c:395: x >>= 1;
;	srl	h
;	rr	l
;pers.c:396: z >>= 1;
;	srl	d
;	rr	e
;pers.c:398: if (x < 256 && z < 256)
;	ld	a,h
;	sub	a, #0x01
;	jr	NC,00108$
;	ld	a,d
;	sub	a, #0x01
;	jr	NC,00108$
;pers.c:400: return UgxGetValueU8(x, z);
;	ld	b,e
;	ld	c, l
;	push	bc
;	call	_UgxGetValueU8
;	pop	af
;	jr	00110$
;00108$:
;pers.c:403: return 0xff;
;	ld	l,#0xFF
;00110$:
;	pop	ix
;	ret

;------------------------------------------------------------------------------
; extern u8 UgxPersXInternal()
; extern u8 UgxPersYInternal()
;------------------------------------------------------------------------------
	.area	_DATA
	.globl	_s_s16ViewX
	.globl	_s_s16ViewY
	.globl	_s_s16ViewZ
	.globl	_s_u8ScreenX
	.globl	_s_u8ScreenY

	.area	_CODE
	.globl	_UgxPersXInternal
	.globl	_UgxPersYInternal

_UgxPersXInternal_start::
_UgxPersXInternal:
;pers.c:420: register s16 x = s_s16ViewX;
	ld	de,(_s_s16ViewX)

_UgxPersXInternal_XIsZero:
	ld	a,d
	or	e
	jr	nz,_UgxPersXInternal_XIsNotZero

	ld	hl,#_s_u8ScreenX + 0		; s_u8ScreenX = UGX_CENTER_X;
	ld	(hl), #0x80

	ld	l,#0x01						; return TRUE;
	ret

_UgxPersXInternal_XIsNotZero:
	ld	hl,(_s_s16ViewZ)			; register s16 z = s_s16ViewZ;

	bit	7, d						; if (x < 0)
	jr	z,_UgxPersXInternal_XPlus

_UgxPersXInternal_XMinus:	
		xor		a,a									; u8 sx = UgxGetValueU16Internal(-x, z);
		sub		a,e
		ld		b,a
		ld		a,#0x00
		sbc		a,d
		ld		c,a									; cb = -x
		ld		a,h									; al = z

		call	_UgxGetValueU16Internal_CB_AL_L

		ld		a,#0x70								; if (UGX_LEFT < sx)
		sub		a, l
		jr		nc,_UgxPersXInternal_XMinus_NotClipped

		ld		l,#0x00								; return FALSE;
		ret

	_UgxPersXInternal_XMinus_NotClipped:
		ld		a,#0x80							; s_u8ScreenX = UGX_CENTER_X - sx;
		sub		a, l
		ld		(_s_u8ScreenX),a

		ld		l,#0x01							; return TRUE
		ret

_UgxPersXInternal_XPlus:
		ld		c,d
		ld		b,e								; cb = x
		ld		a,h								; al = z
		call	_UgxGetValueU16Internal_CB_AL_L	; l = UgxGetValueU16Internal(x, z)

		ld		a,#0x6F							; if (UGX_RIGHT < sx)
		sub		a, l
		jr		nc,_UgxPersXInternal_XPlus_NotClipped

		ld		l,#0x00							; return FALSE
		ret

	_UgxPersXInternal_XPlus_NotClipped:
		ld		a,#0x80							; s_u8ScreenX = UGX_CENTER_X + sx;
		add		a,l
		ld		(_s_u8ScreenX),a

		ld		l,#0x01							; return true
		ret

_UgxPersXInternal_end::

_UgxPersYInternal_start::
_UgxPersYInternal:
	ld	de,(_s_s16ViewY)			; register s16 x = s_s16ViewY;

_UgxPersYInternal_YIsZero:
	ld	a,d
	or	e
	jr	nz,_UgxPersYInternal_YIsNotZero

	ld	hl,#_s_u8ScreenY + 0		; s_u8ScreenY = UGX_CENTER_Y;
	ld	(hl), #0x6A

	ld	l,#0x01						; return TRUE;
	ret

_UgxPersYInternal_YIsNotZero:
	ld	hl,(_s_s16ViewZ)			; register s16 z = s_s16ViewZ;

	bit	7, d						; if (x < 0)
	jr	z,_UgxPersYInternal_YPlus

_UgxPersYInternal_YMinus:	
		xor		a,a									; u8 sx = UgxGetValueU16Internal(-x, z);
		sub		a,e
		ld		b,a
		ld		a,#0x00
		sbc		a,d
		ld		c,a									; cb = -x
		ld		a,h									; al = z

		call	_UgxGetValueU16Internal_CB_AL_L

		ld		a,#0x49								; if (UGX_LEFT < sx)
		sub		a, l
		jr		nc,_UgxPersYInternal_YMinus_NotClipped

		ld		l,#0x00								; return FALSE;
		ret

	_UgxPersYInternal_YMinus_NotClipped:
		ld		a,#0x6A							; s_u8ScreenX = UGX_CENTER_X - sx;
		sub		a, l
		ld		(_s_u8ScreenY),a

		ld		l,#0x01							; return TRUE
		ret

_UgxPersYInternal_YPlus:
		ld		c,d
		ld		b,e								; cb = x
		ld		a,h								; al = z
		call	_UgxGetValueU16Internal_CB_AL_L	; l = UgxGetValueU16Internal(x, z)

		ld		a,#0x49							; if (UGX_RIGHT < sx)
		sub		a, l
		jr		nc,_UgxPersYInternal_YPlus_NotClipped

		ld		l,#0x00							; return FALSE
		ret

	_UgxPersYInternal_YPlus_NotClipped:
		ld		a,#0x6A							; s_u8ScreenX = UGX_CENTER_X + sx;
		add		a,l
		ld		(_s_u8ScreenY),a

		ld		l,#0x01							; return true
		ret

_UgxPersYInternal_end::

;------------------------------------------------------------------------------
; extern void UgxPersModelInternal()
;------------------------------------------------------------------------------
	.area	_DATA
	.globl	_g_v3ModelPosition
	.globl	_s_ugxContext
	.globl	_g_nbVertices
	.globl	_g_pInVertices
	.globl	_g_pOutVertices
	.globl	_g_u8Clip

	.area	_CODE
	.globl	_UgxPersModelInternal

_UgxPersModelInternal_start::
_UgxPersModelInternal:
	; _g_u8Clip = 0
	xor	a
	ld	(#_g_u8Clip),a

_UgxPersModelInternal_LoopStart:

_UgxPersModelInternal_XStart:

	; s_s16ViewX = g_v3ModelPosition.x + (s16)*pInVertices;
	; pInVertices++

	ld	hl,(_g_pInVertices)
	ld	c,(hl)
	bit	7,c
	jr	nz,_UgxPersModelInternal_XMinus

	_UgxPersModelInternal_XPlus:
		ld	a,(#_g_v3ModelPosition + 0)
		add	a,c
		ld	(#_s_s16ViewX+0),a
		ld	a,(#_g_v3ModelPosition + 1)
		adc	a,#0x0
		ld	(#_s_s16ViewX+1),a
		jr	_UgxPersModelInternal_XEnd

	_UgxPersModelInternal_XMinus:
		ld	a,(#_g_v3ModelPosition + 0)
		add	a,c
		ld	(#_s_s16ViewX+0),a
		ld	a,(#_g_v3ModelPosition + 1)
		adc	a,#0xff
		ld	(#_s_s16ViewX+1),a
		jr	_UgxPersModelInternal_XEnd

_UgxPersModelInternal_XEnd:
	inc	hl

_UgxPersModelInternal_YStart:

	; s_s16ViewY = g_v3ModelPosition.y + (s16)*pInVertices;
	; pInVertices++

	ld	c,(hl)
	bit	7,c
	jr	nz,_UgxPersModelInternal_YMinus

	_UgxPersModelInternal_YPlus:
		ld	a,(#_g_v3ModelPosition + 2)
		add	a,c
		ld	(#_s_s16ViewY+0),a
		ld	a,(#_g_v3ModelPosition + 3)
		adc	a,#0x0
		ld	(#_s_s16ViewY+1),a
		jr	_UgxPersModelInternal_YEnd

	_UgxPersModelInternal_YMinus:
		ld	a,(#_g_v3ModelPosition + 2)
		add	a,c
		ld	(#_s_s16ViewY+0),a
		ld	a,(#_g_v3ModelPosition + 3)
		adc	a,#0xff
		ld	(#_s_s16ViewY+1),a
		jr	_UgxPersModelInternal_YEnd

_UgxPersModelInternal_YEnd:
	inc	hl

_UgxPersModelInternal_ZStart:

	; s_s16ViewZ = g_v3ModelPosition.z + (s16)*pInVertices;
	; pInVertices++

	ld	c,(hl)
	bit	7,c
	jr	nz,_UgxPersModelInternal_ZMinus

	_UgxPersModelInternal_ZPlus:
		ld	a,(#_g_v3ModelPosition + 4)
		add	a,c
		ld	(#_s_s16ViewZ+0),a
		ld	a,(#_g_v3ModelPosition + 5)
		adc	a,#0x0
		ld	(#_s_s16ViewZ+1),a
		jr	_UgxPersModelInternal_ZEnd

	_UgxPersModelInternal_ZMinus:
		ld	a,(#_g_v3ModelPosition + 4)
		add	a,c
		ld	(#_s_s16ViewZ+0),a
		ld	a,(#_g_v3ModelPosition + 5)
		adc	a,#0xff
		ld	(#_s_s16ViewZ+1),a
		jr	_UgxPersModelInternal_ZEnd

_UgxPersModelInternal_ZEnd:
	inc	hl
	ld	(_g_pInVertices),hl

; if (s_s16ViewZ < 16)
	ld	a,(#_s_s16ViewZ + 0)
	sub	a, #0x10
	ld	a,(#_s_s16ViewZ + 1)
	sbc	a, #0x00
	jr	nc,_UgxPersModelInternal_PersX

_UgxPersModelInternal_Clipped:
	ld	hl,(#_g_pOutVertices)
	ld	(hl),#0xff
	inc	hl
	ld	(hl),#0xff
	inc	hl
	ld	(#_g_pOutVertices),hl
	ld	hl,#_g_u8Clip
	inc	(hl)

_UgxPersModelInternal_LoopEnd:
	ld		hl,#_g_nbVertices
	dec		(hl)
	jp		nz,_UgxPersModelInternal_LoopStart
	;ld		a,(#_g_u8Clip)
	;ld		l,a
	ret

_UgxPersModelInternal_PersX:
	call	_UgxPersXInternal
	dec		l
	jr		nz,_UgxPersModelInternal_Clipped

	ld		a,(#_s_u8ScreenX)
	ld		hl,(#_g_pOutVertices)
	ld		(hl),a

_UgxPersModelInternal_PersY:
	call	_UgxPersYInternal
	dec		l
	jr		nz,_UgxPersModelInternal_Clipped

	ld		a,(#_s_u8ScreenY)
	ld		hl,(#_g_pOutVertices)
	inc		hl
	ld		(hl),a
	inc		hl
	ld		(_g_pOutVertices),hl

	jr		_UgxPersModelInternal_LoopEnd


;------------------------------------------------------------------------------
; extern void UgxOnCommandRenderObjectNoClip(SUgxCommandRenderObject *pCommand);
;------------------------------------------------------------------------------
	.area _DATA

g_pVertexSave:
	.dw		0x0000

	.area	_CODE
	.globl	_UgxOnCommandRenderObjectClip
	.globl	_UgxOnCommandRenderObjectNoClip
	.globl	_VDPWaitLine_HL_DE

	FORCLR = 0xF3E9

_UgxOnCommandRenderObjectNoClip:
	ld	hl,#2
	add	hl,sp
	
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl	; hl = pCommand

;typedef struct 
;{
;	u8 eType;
;	u8 u8Color;
;	void *pVertex;
;	void *pIndex;
;	u8 nbLines;
;	//SUgxModel *pObject;
;} SUgxCommandRenderObject;

	inc	hl			; skip eType
	ld	a,(hl)		; a = pCommand->u8Color;
	ld	(FORCLR),a

	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		; de = pVertex 

	ld	(g_pVertexSave),de

	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		; de = pIndex
	inc	hl
	ld	b,(hl)		; b = nbLines

	ex	de,hl		; hl = pIndex

	; for( nbIndices )

_UgxOnCommandRenderObjectNoClip_Loop:
	ld		e,(hl)			; a = start index
	sla		e
	ld		d,#0			; de = vertex offset
	inc		hl

	push	hl
	ld		hl,(g_pVertexSave)
	add		hl,de			; hl = vertex address

	ld		a,(hl)			; a = sx
	inc		hl
	ld		c,(hl)			; c = sy
	pop		hl
	
	ld		e,(hl)			
	sla		e
	ld		d,#0			; de = end index
	inc		hl

	push	hl
	ld		hl,(g_pVertexSave)
	add		hl,de

	ld		d,(hl)			; d = ex
	inc		hl
	ld		e,(hl)			; e = ey
	
	ld		h,a				; h = sx			
	ld		l,c				; l = sy

	push	bc
	call	_VDPWaitLine_HL_DE
	pop		bc

	pop		hl
			
	djnz	_UgxOnCommandRenderObjectNoClip_Loop
		
	ret

_UgxOnCommandRenderObjectClip:
	ld	hl,#2
	add	hl,sp
	
	ld	e,(hl)
	inc	hl
	ld	d,(hl)
	ex	de,hl	; hl = pCommand

;typedef struct 
;{
;	u8 eType;
;	u8 u8Color;
;	void *pVertex;
;	void *pIndex;
;	u8 nbLines;
;	//SUgxModel *pObject;
;} SUgxCommandRenderObject;

	inc	hl			; skip eType
	ld	a,(hl)		; a = pCommand->u8Color;
	ld	(FORCLR),a

	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		; de = pVertex 

	ld	(g_pVertexSave),de

	inc	hl
	ld	e,(hl)
	inc	hl
	ld	d,(hl)		; de = pIndex
	inc	hl
	ld	b,(hl)		; b = nbLines

	ex	de,hl		; hl = pIndex

	; for( nbIndices )

_UgxOnCommandRenderObjectClip_Loop:

_UgxOnCommandRenderObjectClip_FetchStartVertex:
	ld		e,(hl)			; a = start index
	sla		e
	ld		d,#0			; de = vertex offset
	inc		hl

	push	hl
	ld		hl,(g_pVertexSave)
	add		hl,de			; hl = vertex address

	ld		c,(hl)			; c = sx
	inc		hl
	ld		a,(hl)			; a = sy

	inc		a				; if( sy == 0xff) continue
	jr		nz,_UgxOnCommandRenderObjectClip_FetchEndVertex

	pop		hl
	inc		hl				; hl = next start index
	djnz	_UgxOnCommandRenderObjectClip_Loop
	ret

_UgxOnCommandRenderObjectClip_FetchEndVertex:
	dec		a

	pop		hl

	ld		e,(hl)			
	sla		e
	ld		d,#0			; de = end index
	inc		hl

	push	hl

	ld		hl,(g_pVertexSave)
	add		hl,de

	ld		d,(hl)			; d = ex
	inc		hl
	ld		e,(hl)			; e = ey
	
	ld		l,a				; l = sy

	ld		a,e
	inc		a				; if( ey == 0xff ) continue
	jr		z,_UgxOnCommandRenderObjectClip_LoopCotinue

	ld		h,c				; h = sx			

	push	bc
	call	_VDPWaitLine_HL_DE
	pop		bc

_UgxOnCommandRenderObjectClip_LoopCotinue:
	pop		hl
	djnz	_UgxOnCommandRenderObjectClip_Loop
	ret
