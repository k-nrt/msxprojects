                .include    "pers_defs.s"

;------------------------------------------------------------------------------
; model_zx_plane
;------------------------------------------------------------------------------
        .area   _CODE
        .globl  _g_meshZXPlane

        .macro  LINE    inIndex0, inIndex1
        .word   PersScreenPositionsAddress+inIndex0*8, PersScreenPositionsAddress+inIndex1*8
        .endm

Vertices:
    .byte   -32, 0, -32
    .byte    32, 0, -32
    .byte    32, 0,  32
    .byte   -32, 0,  32

Indices:
    LINE    0,1
    LINE    1,2
    LINE    2,3
    LINE    3,0

_g_meshZXPlane:
    .byte   4
    .byte   4
    .word   Vertices
    .word   Indices
