;------------------------------------------------------------------------------
; model_zx_plane
;------------------------------------------------------------------------------
        .area   _CODE
        .globl  _g_modelZXPlane

        .macro  LINE    inIndex0, inIndex1
        .word   0xDF00+inIndex0*8, 0xDF00+inIndex1*8
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

_g_modelZXPlane:
    .byte   -32,32
    .byte    0,0
    .byte   -32,32
    .byte   4
    .byte   4
    .word   Vertices
    .word   Indices
