	;; crt0.s for msx rom
	.module crt0
	.globl	_main

	.globl	l__INITIALIZER
	.globl	s__INITIALIZED
	.globl	s__INITIALIZER
	.globl	l__DATA
	.globl	s__DATA

	.area _HEADER (ABS)

	;; initialize
	.org	0x0100

init:
	;; Stack at the top of bios work.
	;ld		sp,(0xf674)

	;; slot0 - main-rom
	;ld		a,(0xfcc1)
	;ld		hl,#0x0000
	;call	0x0024 ; ENASLT
	;ei

	;; Initialise global variables
	call	gsinit
	ld	hl,#0x0080
	ld	e,(hl)
	ld	d,#0x00
	inc	hl
	add	hl,de
	ld	(hl),#0x00
	ld	hl,#0x0081
	call	_main
	jp	_exit

	;; Ordering of segments for the linker.
	.area	_HOME
	.area	_CODE
	.area	_INITIALIZER
	.area   _GSINIT
	.area   _GSFINAL

	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP

	.area	_CODE
__clock::
	ld	a,#2
	rst	0x08
	ret

_exit::
	;; Exit - special code to the emulator
	ret

	.area   _GSINIT
gsinit::

	; Default-initialized global variables.
	ld	bc, #l__DATA
	ld	a, b
	or	a, c
	jr	Z, zeroed_data
	ld	hl, #s__DATA
	ld	(hl), #0x00
	dec	bc
	ld	a, b
	or	a, c
	jr	Z, zeroed_data
	ld	e, l
	ld	d, h
	inc	de
	ldir

zeroed_data:

	; Explicitly initialized global variables.
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
