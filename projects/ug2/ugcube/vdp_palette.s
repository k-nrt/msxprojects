;------------------------------------------------------------------------------
; VDP palette
;------------------------------------------------------------------------------
            .area   _CODE

            VDPRead  = 0x0006
            VDPWrite = 0x0007
;------------------------------------------------------------------------------
; in L start
; in B count
; in DE palette data
;------------------------------------------------------------------------------
VDPPaletteWrite:
    ld      a,(#VDPWrite)
    inc     a
    ld      c,a             ; c = write control register

    ld      a,l
    and     #0x0f           ; palette start

    di
    out     (c),a           ; a = palette start index
    ld      a,#0x80+#16     ; 0b10xxxxxx 
    out     (c),a           ; a = write control register #16

    inc     c               ; c = i/o write palette

VDPPaletteWrite_Loop:
    ld      a,(de)
    out     (c),a
    inc     de
    out     (c),a
    inc     de

    djnz    VDPPaletteWrite_Loop

    ei
    ret

;------------------------------------------------------------------------------
; extern void VDPPaletteWrite(u8 start, const u16 *p0GRB, u8 count); 
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _VDPPaletteWrite

_VDPPaletteWrite:
    ld      hl,#0x0002
    add     hl,sp

    ld      c,(hl)
    inc     hl
    
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    
    ld      b,(hl)
    ld      l,c
    call    VDPPaletteWrite
    ret
