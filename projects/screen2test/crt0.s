	;; crt0.s for msx rom
	.module crt0
	.globl	_main

	.area _HEADER (ABS)

	;; rom header
	.org	0x4000
	.db	0x41, 0x42	; id
	.dw	init		; init
	.dw	0x0000		; statement
	.dw	0x0000		; device
	.dw	0x0000		; text
	.dw 0x0000		; reserve
	.dw 0x0000		; reserve
	.dw 0x0000		; reserve

	;; initialize
	.org	0x4010

init:
	;; Stack at the top of bios work.
	ld		sp,#0xf380

	call	0x0138 ; RSLREG
	ld		b,a
;	and		#0x0c0
	srl		a
	srl		a
	ld		hl, #0x8000
	call	0x0024 ; ENASLT
	ei

	;; Initialise global variables
	call    gsinit
	call	_main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_HOME
	.area   _CODE
	.area   _GSINIT
	.area   _GSFINAL

	.area   _DATA
	.area   _BSS
	.area   _HEAP

_exit::
	halt
	
	.area   _GSINIT
gsinit::
	.area   _GSFINAL
	ret

