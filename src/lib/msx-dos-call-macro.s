;------------------------------------------------------------------------------
; Macro.
;------------------------------------------------------------------------------
; CALSLT
;	MSX1 disable interrupt
;	MSX2 save interrupt state

	.macro	call_main_rom	param_Address
		push	ix
		push	iy
		ld	iy, (#EXPTBL - #0x0001)
		ld	ix, #param_Address
		call	CALSLT
		;ei			; in MSX1
		pop	iy
		pop	ix
	.endm

	.macro	call_sub_rom	param_Address
					; make calling sequence on stack
					; sp+0    inc    sp
					; sp+1    inc    sp
					; sp+2    ld     ix,#param_Address
					; sp+6    nop
					; sp+7    jp     extrom

		exx			; save arguments
		ex	af,af'

		ld	hl,#EXTROM
		push	hl
		ld	hl,#0xc300	; jp xxxx, nop
		push	hl
		ld	hl,#param_Address
		push	hl		; sub-rom entry
		ld	hl,#0x21dd	; ld ix,#param_Address
		push	hl
		ld	hl,#0x3333	; inc sp, inc sp
		push	hl
		ld	hl,#0x0000
		add	hl,sp
		ld	a,#0xc3
		ld	(#H_NMI),a
		ld	(#H_NMI + #0x0001),hl

		ex	af,af'		; restore registers
		exx

		push	ix
		push	iy
		ld	ix,#NMI
		ld	iy,(#EXPTBL - #0x0001)
		call	CALSLT
		pop	iy
		pop	ix
		ei

		ld	hl,#10		; restore stack position
		add	hl,sp
		ld	sp,hl
		ret
	.endm
