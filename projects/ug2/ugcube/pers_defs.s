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

; screen positions
PersScreenPositionsAddress = 0xDE00