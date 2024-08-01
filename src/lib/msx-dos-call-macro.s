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
		push	ix
		push	iy
		ld	iy, (#EXBRSA - #0x0001)
		ld	ix, #param_Address
		call	CALSLT
		;ei			; in MSX1
		pop	iy
		pop	ix
	.endm
