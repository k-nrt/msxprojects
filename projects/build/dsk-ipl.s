; disk boot loader
	.module boot

  	.area   _HEADER (ABS)
	.org    0xc000

	.db		0xEB, 0xFE, 0x90
	.ascii	\ALESTE20\
	.dw		0x0200	; Sector size.
	.db		0x02	; Cluster size.
	.dw		0x0001	; 
	.db		0x02	; FAT
	.dw		0x0070	; Directory entry.
	.dw		0x05A0	; Sector.
	.db		0xF9	; Media ID.
	.dw		0x0003	; FAT size.
	.dw		0x0009	; Sectors per track.
	.dw		0x0002	; side.
	.dw		0x0000	; hide

	jr		BOOT
	
	.db		"V","O","L","_","I","D"
	.db		0x00
	.db		"H","O","G","E"
	.db		0x00, 0x00, 0x00, 0x00, 0x00

    .globl  BOOT
BOOT:
	ret		nc
	ld		(PAGE1DISKROM+1),de	

	ld		hl,#3
	add		hl,de
	ld		(PAGE1RAMPATCH+1),hl	
	
	ld		hl,#PAGE1DISKROM
	ld		de,#0xf87f          ; set disk rom to page1
	ld		bc,#4
	ldir

	ld		hl,#PAGE1RAMPATCH
	ld		de,#0xf87f+4        ; set ram to page1
	ld		bc,#4
	ldir

	ld		de,	#0x8000         ; de = transfer address
	ld		c,	#0x1a           ; set transfer address
	call	0xf37d              ; BDOS
	
	ld		de,	#0x0002         ; de = logical sector
	ld		hl,	#0x2000         ; h = 32 = sector count = 16k, l = A:
    ld		c,	#0x2f           ; c = read sector
	call	0xf37d              ; BDOS

	call	0xf87f
	call	0xf87f+4

	jp		0x8000

PAGE1DISKROM:
	call	0x0000
	ret
	
PAGE1RAMPATCH:
	call	0x0000
	ret

    .area   _DATA