;------------------------------------------------------------------------------
; vdp_read.s
;------------------------------------------------------------------------------

;------------------------------------------------------------------------------
; VDPReadBegin314DI
; in B  vram high 3
; in DE vram low 14
; out C i/o address vram read
;     disable interrupt 
;------------------------------------------------------------------------------
        .area   _CODE
        .globl  VDPReadBegin116DI
        .globl  VDPReadBegin314DI

        VDPRead  = 0x0006
        VDPWrite = 0x0007

VDPReadBegin116DI:
    rlc     d               ; bd <<= 2
    rl      b
    rlc     d
    rl      b               ; b = address high 3
    srl     d               ; d >>= 2
    srl     d               ; de = address low 14

VDPReadBegin314DI:

    ld      a,(#VDPWrite)
    inc     a
    ld      c,a             ; c = write control register

    ld      a,b
    and     #0x07

    di
    out     (c),a           ; a = vram address upper 3 bits
    ld      a,#0x80+#14     ; 0b10xxxxxx 
    out     (c),a           ; a = write control register #14

    ld      a,e
    out     (c),a           ; a = vram address lower 8 bits
    ld      a,#0x00         ; 0b00xxxxxx
    or      d
    out     (c),a           ; a = write vram address middle 6 bits

    ld      a,(#VDPRead)
    ld      c,a             ; c = i/o read  vram
    ret 

;------------------------------------------------------------------------------
; extern u8 VDPReadByte(u8 addressHigh, u16 addressLow);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _VDPReadByte

_VDPReadByte:
    ld      hl,#02
    add     hl,sp
    ld      b,(hl)
    inc     hl
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    call    VDPReadBegin116DI

    in      a,(c)
    ei
    ret

;------------------------------------------------------------------------------
; extern void VDPReadBytes(u8 addressHigh, u16 addressLow, void* values, u16 size);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _VDPReadBytes

_VDPReadBytes:
    ld      hl,#02
    add     hl,sp
    ld      b,(hl)
    inc     hl
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    call    VDPReadBegin116DI

    ld      hl,#05
    add     hl,sp
    ld      e,(hl)              ; de = values
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      a,(hl)              ; hl = size
    inc     hl
    ld      h,(hl)
    ld      l,a

_VDPReadBytes_Loop:
    in      a,(c)
    ld      (de),a
    inc     de
    dec     hl
    ld      a,h
    or      l
    jr      nz,_VDPReadBytes_Loop
    ei
    ret
