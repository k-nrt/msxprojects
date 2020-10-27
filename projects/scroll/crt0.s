; crt0.s for msx disk
        .module crt0
       	.globl	_main

	.area _HEADER (ABS)

	; initialize
	.org	0x8000

init::
	; stack at the top of bios work.
;	ld		sp,#0xf380
;	ld		sp,(0xfc4a)	;HIMEM
	ld		sp,(0xf674)

	;; slot0 - main-rom
	ld		a,(0xfcc1)
	ld		hl,#0x0000
	call	0x0024 ; ENASLT
	ei

	;; Initialise global variables
	call    gsinit
	call	_main
	jp	_exit

_exit::
	halt

	;; Ordering of segments for the linker.
	.area	_HOME
	.area   _CODE
	.area   _GSINIT
	.area   _GSFINAL

	.area   _DATA
	.area   _BSS
	.area   _HEAP

	.area   _GSINIT
gsinit::
	ld	bc, #l__INITIALIZER
	ld	a, b
	or	a, c
	jr	Z, gsinit_next
	ld	de, #s__INITIALIZED
	ld	hl, #s__INITIALIZER
	ldir

gsinit_next:
	.area   _GSFINAL
	ret
