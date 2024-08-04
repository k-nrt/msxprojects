;------------------------------------------------------------------------------
; vdp_write.s
;------------------------------------------------------------------------------
	.include	"vdp_defs.s"

;------------------------------------------------------------------------------
; VDPWriteBeginDI
; in B  vram high 1
; in DE vram low 16
; out C i/o address vram write
;     disable interrupt 
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	VDPWriteBeginDI

VDPWriteBeginDI:
	ld	a,(#_VDPWrite)
	inc	a
	ld	c,a		; c = write control register
	ld	a,b		; a = vram high 1

	rlc	d		; ad <<= 2
	rla
	rlc	d
	rla
	and	#0x07
	di

	out	(c),a		; a = vram address upper 3 bits
	ld	a,#0x80+#14	; 0b10xxxxxx 
	out	(c),a		; a = write control register #14

	ld	a,e
	out	(c),a		; a = vram address lower 8 bits
	srl	d		; d >>= 2
	srl	d		; d = restore vram address and fill 0 to upper 2 bits
	ld	a,#0x40		; 0b01xxxxxx
	or	d
	out	(c),a		; a = write vram address middle 6 bits

	dec	c		; c = i/o write vram
	ret

;------------------------------------------------------------------------------
; extern void VDPWriteByte(u8 addressHigh, u16 addressLow, u8 value);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPWriteByte

_VDPWriteByte:
	ld	hl,#2		; skip return address
	add	hl,sp

	ld	b,(hl)		; b = addressHigh
	inc	hl
	ld	e,(hl)		; de = addressLow
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	h,(hl)		; h = value
	
	call	VDPWriteBeginDI

	ld	a,h
	out	(c),a
	ei
	ret

;------------------------------------------------------------------------------
; extern void VDPWriteBytes(u8 addressHigh, u16 addressLow, const u8* values, u16 size);
;------------------------------------------------------------------------------
	.area	_CODE
	.globl	_VDPWriteBytes

_VDPWriteBytes:
	ld	hl,#2		; skip return address
	add	hl,sp

	ld	b,(hl)		; b = addressHigh
	inc	hl
	ld	e,(hl)		; de = addressLow
	inc	hl
	ld	d,(hl)

	call	VDPWriteBeginDI

	ld	hl,#5		; skip ret adressHigh addressLow
	add	hl,sp
	ld	e,(hl)		; de = values
	inc	hl
	ld	d,(hl)
	inc	hl
	ld	a,(hl)
	inc	hl
	ld	h,(hl)		; hl = size
	ld	l,a

_VDPWriteBytes_Loop:
	ld	a,(de)
	out	(c),a
	inc	de
	dec	hl
	ld	a,h
	or	l
	jr	nz,_VDPWriteBytes_Loop
	ei
	ret
