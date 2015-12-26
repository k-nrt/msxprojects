;------------------------------------------------------------------------------
; UxRender
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_UxRenderObjectNoClip
;	.globl	_UxRenderSetRenderPage
;	.globl	_UxRenderSetRenderColor

	.globl	VDP_HLDE_LINE

;------------------------------------------------------------------------------
; void UxRenderSetRenderPage( u8 nPage );
;------------------------------------------------------------------------------
;_UxRenderSetRenderPage:
;	ld		HL,#2
;	add		HL,SP
;	ld		A,(HL)	
;	ld		(VDP_LINE_PAGE),A
;	ret

;------------------------------------------------------------------------------
; void UxRenderSetRenderColor( u8 nColor );
;------------------------------------------------------------------------------
;_UxRenderSetRenderColor:
;	ld		HL,#2
;	add		HL,SP
;	ld		A,(HL)	
;	ld		(VDP_LINE_COLOR),A
;	ret

;------------------------------------------------------------------------------
; void UxRenderObjectNoClip( struct SUxVertex *pVertex, u8 *pIndex, u8 nbIndices );
;------------------------------------------------------------------------------
_UxRenderObjectNoClip:
	ld		HL,#2
	add		HL,SP
	ld		E,(HL)	
	inc		HL
	ld		D,(HL)			; PVERTEXSAVE = pVertex
	
	inc		DE
	inc		DE
	ld		(PVERTEXSAVE),DE
	
	inc		HL
	ld		E,(HL)
	inc		HL
	ld		D,(HL)			; DE = pIndex
	
	inc		HL
	ld		B,(HL)			; B = nbIndices;

	ex		DE,HL			; HL = pIndex

LOOP:						; for( nbIndices )
	ld		E,(HL)			; DE = start index
	ld		D,#0
	inc		HL

	;ex		DE,HL			; DE *= sizeof(SUxVertex)
	;add		HL,HL
	;add		HL,HL
	;add		HL,HL
	;ex		DE,HL
	
	push	HL
	ld		HL,(PVERTEXSAVE)
	add		HL,DE

	ld		A,(HL)			; A = sx
	inc		HL
	inc		HL
	ld		C,(HL)			; C = sy
	pop		HL
	
	ld		E,(HL)			; DE = end index
	ld		D,#0
	inc		HL

	;ex		DE,HL			; DE *= sizeof(SUxVertex)
	;add		HL,HL
	;add		HL,HL
	;add		HL,HL
	;ex		DE,HL

	push	HL
	ld		HL,(PVERTEXSAVE)
	add		HL,DE

	ld		D,(HL)			; D = ex
	inc		HL
	inc		HL
	ld		E,(HL)			; E = ey
	
	ld		H,A				; H = sx			
	ld		L,C				; L = sy

	push	BC
	call	VDP_HLDE_LINE
	pop		BC

	pop		HL
			
	djnz	LOOP
		
	ret

	.area _BSS

PVERTEXSAVE:
	.dw		0x0000
;------------------------------------------------------------------------------[eof]
