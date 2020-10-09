;------------------------------------------------------------------------------
; pers definitions
;------------------------------------------------------------------------------
;------------------------------------------------------------------------------
; context
;------------------------------------------------------------------------------
    .area   _DATA
    .globl  _g_persContext

;------------------------------------------------------------------------------
; constants
;------------------------------------------------------------------------------
; i/o ports
VDPRead         = 0x0006
VDPWrite        = 0x0007

; clip bits
kClipBit_Near   = 0x01
kClipBit_Far    = 0x02
kClipBit_Left   = 0x04
kClipBit_Right  = 0x08
kClipBit_Top    = 0x10
kClipBit_Bottom = 0x20

.macro  Macro_Bit_Left inReg
    bit     2,inReg
.endm

.macro  Macro_Bit_Right inReg
    bit     3,inReg
.endm

.macro  Macro_Bit_Top inReg
    bit     4,inReg
.endm

.macro  Macro_Bit_Bottom inReg
    bit     5,inReg
.endm

; box clip
kBBoxClip_Out        = 0
kBBoxClip_In         = 1
kBBoxClip_ScissorXY  = 2
kBBoxClip_ScissorXYZ = 3

; context members
m_v3PositionX       = 0
m_v3PositionY       = 1
m_v3PositionZ       = 2
m_clipNear          = 3
m_s16ScreenZ        = 4
m_s16ScreenX        = 6
m_s16ScreenY        = 8
m_viewPortLeft      = 10
m_viewPortRight     = 12
m_viewPortTop       = 14
m_viewPortBottom    = 16
m_vramHigh          = 18
m_vramOffset        = 19

;------------------------------------------------------------------------------
; tables
;------------------------------------------------------------------------------
PersTransform_Pointers  = 0xC000    ; z -> 2d pos array     2*128 entries.
PersTransform_Left      = 0xC100    ; z -> left clip pos    1*128 entries.
PersTransform_Right     = 0xC180    ; z -> right clip pos   1*128 entries.
PersTransform_Top       = 0xC200    ; z -> top clip pos     1*128 entries.
PersTransform_Bottom    = 0xC280    ; z -> bottom clip pos  1*128 entries.

PersTransform_Positions         = 0xC300    ; 2d pos arrays 0xC300 - 0xDCFF
PersTransform_Positions_End     = 0xDD00
PersTransform_RcpZ              = 0xDD00    ; z -> RcpZ     2*128 entries
PersScreenPositionsAddress      = 0xDE00    ; internal vertex buffer 8*64 entries.

;------------------------------------------------------------------------------
; load 2d position from transform table
; in d = 3d z pos
; in e = 3d x or y pos
; out d = z (keep)
; out e = 2d pos
; use a, hl
;------------------------------------------------------------------------------
.macro  Macro_Load2DPosiion
    ld      h,#PersTransform_Pointers >> 8
    ld      l,d
    sla     l       ; HL = ptr + z*2
    ld      a,(hl)
    inc     hl
    ld      h,(hl)
    ld      l,a     ; HL = 2d position top address
    ld      a,d     ; A = z
    ld      d,#0x00
    add     hl,de   ; HL = 2d x address
    ld      e,(hl)
    ld      d,a     ; D = z
.endm

;------------------------------------------------------------------------------
; test clip border and load 2d position
; in inClipOutLabel = jump if position is clipped out
; in d = 3d z
; in e = 3d x or y
; out d = 3d z (keep)
; out e = 2d position
; use a, hl
;------------------------------------------------------------------------------
.macro Macro_PersTransformUnsignedFast_Left   inClipOutLabel
    ld      h,#PersTransform_Left >> 8
    ld      l,d
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,inClipOutLabel
    Macro_Load2DPosiion
    ;or      a       ; CY = 0
.endm

.macro Macro_PersTransformUnsignedFast_Right    inClipOutLabel
    ld      h,#PersTransform_Right >> 8
    ld      l,d
    bit     7,l
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,inClipOutLabel
    Macro_Load2DPosiion
    ;or      a       ; CY = 0
.endm

.macro Macro_PersTransformUnsignedFast_Top      inClipOutLabel
    ld      h,#PersTransform_Top >> 8
    ld      l,d
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,inClipOutLabel
    Macro_Load2DPosiion
    ;or      a       ; CY = 0
.endm

.macro Macro_PersTransformUnsignedFast_Bottom   inClipOutLabel
    ld      h,#PersTransform_Bottom >> 8
    ld      l,d
    bit     7,l
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,inClipOutLabel
    Macro_Load2DPosiion
    ;or      a       ; CY = 0
.endm
