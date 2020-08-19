;------------------------------------------------------------------------------
; pers_create_bbox
;------------------------------------------------------------------------------
                .include    "pers_defs.s"
                .area   _CODE
                .globl  PersCreateBBox

;------------------------------------------------------------------------------
; VDPReadBegin116DI
; in B  vram high 1
; in DE vram low 16
; out C i/o address vram read
;     disable interrupt
; use A
;------------------------------------------------------------------------------
                .globl  VDPReadBegin116DI

;------------------------------------------------------------------------------
; VDPWriteBeginDI
; in B  vram high 1
; in DE vram low 16
; out C i/o address vram write
;     disable interrupt 
; use A
;------------------------------------------------------------------------------
                .globl  VDPWriteBeginDI

;------------------------------------------------------------------------------
; PersCreateBBox
; in B  vram high 1
; in C  vertex count
; in DE vram low 16 to read vertices
; in HL vram low 16 to write bbox
; use A,A',BC',DE',HL'
;------------------------------------------------------------------------------
PersCreateBBox:
    push    bc
    ld      a,c                 ; a' = vertex count
    ex      af,af'
    call    VDPReadBegin116DI
    ex      af,af'
    ld      b,a                 ; b = vertex count

LoadFirstVertex:
    in      a,(c)               ; b' = c' = x[0]
    exx
    ld      b,a
    ld      c,a
    exx

    in      a,(c)               ; d' = e' = y[0]
    exx
    ld      d,a
    ld      e,a
    exx

    in      a,(c)               ; h' = l' = z[0]
    exx
    ld      h,a
    ld      l,a
    exx

    dec     b                   ; decrement vertex count
    jp      z,StoreVram

Test_X_Start:
    in      a,(c)               ; a = x
    exx
Test_MinX:
    cp      b
    jp      p,Test_MaxX
    ld      b,a
Test_MaxX:
    cp      c
    jp      m,Test_X_End
    ld      c,a
Test_X_End:
    exx

Test_Y_Start:
    in      a,(c)               ; a = y
    exx
Test_MinY:
    cp      d
    jp      p,Test_MaxY
    ld      d,a
Test_MaxY:
    cp      e
    jp      m,Test_Y_End
    ld      e,a
Test_Y_End:
    exx

Test_Z_Start:
    in      a,(c)               ; a = z
    exx
Test_MinZ:
    cp      h
    jp      p,Test_MaxZ
    ld      h,a
Test_MaxZ:
    cp      l
    jp      m,Test_Z_End
    ld      l,a
Test_Z_End:
    exx

    djnz    Test_X_Start

StoreVram:
    ei
    pop     bc                  ; b = vram high 1 to write
    ex      de,hl               ; de = vram low 16 to write
    call    VDPWriteBeginDI

StoreVram_MinX:
    exx
    ld      a,b
    exx
    out     (c),a
StoreVram_MaxX:
    exx
    ld      a,c
    exx
    out     (c),a
StoreVram_MinY:
    exx
    ld      a,d
    exx
    out     (c),a
 StoreVram_MaxY:
    exx
    ld      a,e
    exx
    out     (c),a
StoreVram_MinZ:
    exx
    ld      a,h
    exx
    out     (c),a
StoreVram_MaxZ:
    exx
    ld      a,l
    exx
    out     (c),a
    ei
    ret 
