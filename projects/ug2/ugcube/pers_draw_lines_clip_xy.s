;------------------------------------------------------------------------------
; pers draw lines
;------------------------------------------------------------------------------
            .include    "pers_defs.s"

            .area   _CODE
            .globl  PersDrawLinesClipXY
            .globl  _PersDrawLinesClipXY

            .globl  VDP_HLDE_LINE
            
            .globl  _g_clipLineInOut
            .globl  _g_clipRect
            .globl  ClipLeft
            .globl  ClipRight

m_s16Left       = 0
m_s16Right      = 2
m_s16Top        = 4
m_s16Bottom     = 6

;------------------------------------------------------------------------------
; PersDrawLinesClipXY
; in hl = line address
; in b  = line count
; use a, de
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersDrawLinesClipXY

PersDrawLinesClipXY:
    ld      a,b             ; b' = loop counter
    exx
    ld      b,a

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoopStart:
    exx
                            ; hl = line address
                            ; b' = loop counter

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoadPointers:
    ld      e,(hl)          ; de = pos0 address
    inc     hl
    ld      d,(hl)
    inc     hl

    ld      c,(hl)          ; bc = pos1 sddress
    inc     hl
    ld      b,(hl)
    inc     hl

    ex      de,hl           ; de = line address, hl = pos0 address

    ld      a,(bc)          ; a = clip bits0 | clip bits1
    or      (hl)
    jp      nz,PersDrawLinesClipXY_ClipLine

;------------------------------------------------------------------------------
PersDrawLinesClipXY_DrawLineNoClip:
                            ; no any clips
                            ; de = line address
                            ; hl = pos0 address
                            ; bc = pos1 address
    push    de
    inc     hl
    ld      d,(hl)          ; d = x0
    inc     hl
    ld      e,(hl)          ; e = y0

    inc     bc
    ld      a,(bc)          ; h = x1
    ld      h,a
    inc     bc
    ld      a,(bc)          ; l = y1
    ld      l,a

    call    VDP_HLDE_LINE
    pop     hl              ; hl = line address

;------------------------------------------------------------------------------
PersDrawLinesClipXY_Next:
                            ; hl = line address
                            ; b' = loop counter
    exx
    djnz    PersDrawLinesClipXY_LoopStart
    ret

;------------------------------------------------------------------------------
PersDrawLinesClipXY_ClipLine:
                            ; a = clip bits0 | clip bits1
                            ; de = line address
                            ; hl = pos0 address
                            ; bc = pos1 address

;------------------------------------------------------------------------------
PersDrawLinesClipXY_Test_Near_Far_Left_Right_Top_Bottom:
    ex      af,af'          ; a' = clip bits0 | clip bits1
    ld      a,(bc)          ; a = clip bits0 & clip bits1
    and     (hl)
    jp      z,PersDrawLinesClipXY_Test_Near_Far_Left_Right_Top_Bottom_End
    
                            ; out of clip planes
    ex      de,hl           ; hl = line address
    jp      PersDrawLinesClipXY_Next
PersDrawLinesClipXY_Test_Near_Far_Left_Right_Top_Bottom_End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_Test_Near_Far:
    ex      af,af'
    and     #kClipBit_Near|#kClipBit_Far    ; (clip bits0 | clip bits1) & (kClipBit_Near | kClipBit_Far)
    jp      z,PersDrawLinesClipXY_Test_Near_Far_End

                            ; out of near or far clips
    ex      de,hl           ; hl = line address
    jp      PersDrawLinesClipXY_Next
PersDrawLinesClipXY_Test_Near_Far_End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoadX0:
                            ; de = line address
                            ; hl = pos0 address
                            ; bc = pos1 address
    push    de              ; (sp) = line address
    push    bc              ; (sp) = pos1 address

    ld      b,(hl)          ; b = clip bit0
    inc     hl
    ld      e,(hl)          ; e = x0 low
    inc     hl
    ld      a,b             ; a = clip bit0 & (kClipBit_Left | kClipBit_Right)
    and     #kClipBit_Left | #kClipBit_Right
    jp      z,PersDrawLinesClipXY_LoadX0Byte

PersDrawLinesClipXY_LoadX0Word:
    inc     hl
    ld      d,(hl)      ; d = x0 high
    dec     hl          ; hl = y0 addr
    jp      PersDrawLinesClipXY_LoadX0End

PersDrawLinesClipXY_LoadX0Byte:
    ld      d,#0        ; d = 0

PersDrawLinesClipXY_LoadX0End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoadY0:
                        ; b  = clip bits0
                        ; de = x0
                        ; hl = pos0 y low addr
    ld      a,b         ; a = clip bits0 & (kClipBit_Top | kClipBit_Bottom)
    and     #kClipBit_Top | kClipBit_Bottom
    jp      z,PersDrawLinesClipXY_LoadY0Byte

PersDrawLinesClipXY_LoadY0Word:
    ld      a,(hl)      ; a = y0 low
    inc     hl
    inc     hl
    ld      h,(hl)      ; h = y0 high
    ld      l,a         ; l = y0 low
    jp      PersDrawLinesClipXY_LoadY0End

PersDrawLinesClipXY_LoadY0Byte:
    ld      l,(hl)
    ld      h,#0

PersDrawLinesClipXY_LoadY0End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoadX1:
    exx                 ; b = loop counter
                        ; b'  = clip bit0
                        ; de' = x0
                        ; hl' = y0
    pop     hl          ; hl = pos1 addr
    push    bc          ; (sp) = loop counter
    ld      b,(hl)      ; b = clip bit1
    inc     hl          ; hl = x1 low address
    ld      e,(hl)      ; e = x1 low
    inc     hl          ; hl = y1 low address
    ld      a,b         ; a = clip bit1 & (kClipBit_Left | kClipBit_Right)
    and     #kClipBit_Left | #kClipBit_Right
    jp      z,PersDrawLinesClipXY_LoadX1Byte

PersDrawLinesClipXY_LoadX1Word:
    inc     hl          ; hl = x1 high address
    ld      d,(hl)      ; d = x1 high
    dec     hl          ; hl = y1 low address
    jp      PersDrawLinesClipXY_LoadX1End

PersDrawLinesClipXY_LoadX1Byte:
    ld      d,#0        ; d = 0

PersDrawLinesClipXY_LoadX1End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_LoadY1:
                        ; b  = clip bit1
                        ; de = x1
                        ; hl = pos1 y low addr
    ld      a,b         ; a = clip bit1 & (kClipBit_Top | kClipBit_Bottom)
    and     #kClipBit_Top | kClipBit_Bottom
    jp      z,PersDrawLinesClipXY_LoadY1Byte

PersDrawLinesClipXY_LoadY1Word:
    ld      a,(hl)      ; a = y1 low
    inc     hl
    inc     hl
    ld      h,(hl)      ; h = y1 high
    ld      l,a         ; l = y1 low
    jp      PersDrawLinesClipXY_LoadY1End

PersDrawLinesClipXY_LoadY1Byte:
    ld      l,(hl)
    ld      h,#0

PersDrawLinesClipXY_LoadY1End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_Scissor:
                        ; b = clip bit1
                        ; de = x1
                        ; hl = y1
                        ; b' = clip bit0
                        ; de' = x0
                        ; hl' = y0
    push    hl          ; (sp) = hl = y1
    exx
    push    de          ; (sp) = de' = x0
    exx
    pop     hl          ; hl = (sp) = de' = x0
    exx
    pop     de          ; de' = (sp) = hl = y1
    ld      a,b         ; a = clip bit0
    exx                 ; b = clip bit1
    ld      c,a         ; c = clip bit0
                        ; de = x1
                        ; hl = x0
                        ; de' = y1
                        ; hl' = y0

    or      b           ; a = clip bit0 | clip bit1

            .globl  IntersectZero

.macro  Macro_Intersect_X0_BC_X1
                            ; get new x0 and y0
                            ; x0 < bc < x1
                            ; in/out de = x1 
                            ; in/out hl = x0
                            ; in/out de' = y1
                            ; in/out hl' = y0
                            ; use a,a', bc,bc'

    push    bc              ; (sp) = border
    push    de              ; (sp) = x1
    or      a               ; hl = x0 - bc
    sbc     hl,bc

    ex      de,hl           ; de = x1 - bc
    or      a
    sbc     hl,bc
    ex      de,hl

    exx
    push    de              ; (sp) = y1
    exx
                            ; in de = x1 (plus)
                            ; in hl = x0 (minus)
                            ; in de' = y1
                            ; in hl' = y0
                            ; out hl = intersect y
    call    IntersectZero
                            ; hl = new y0 (intersect y)
    pop     de              ; de = y1
    exx     
    pop     de              ; de = x1
    pop     hl              ; hl = new x0 (border)
.endm

;------------------------------------------------------------------------------
PersDrawLinesClipXY_ScissorLeft:
                        ; a = clip bit0 | clip bit1
                        ; b = clip bit1
                        ; c = clip bit0
                        ; de = x1
                        ; hl = x0
                        ; de' = y1
                        ; hl' = y0
    Macro_Bit_Left  a
    jp      z,PersDrawLinesClipXY_ScissorLeft_End

    Macro_Bit_Left  b
    jp      nz,PersDrawLinesClipXY_ScissorLeft_X1_X0

PersDrawLinesClipXY_ScissorLeft_X0_X1:
    push    bc
    ld      bc,(#_g_clipRect + #m_s16Left)
    Macro_Intersect_X0_BC_X1
    pop     bc
    ld      a,b
    or      c
    jp      PersDrawLinesClipXY_ScissorLeft_End

PersDrawLinesClipXY_ScissorLeft_X1_X0:
    push    bc
    ld      bc,(#_g_clipRect + #m_s16Left)
    ex      de,hl
    exx
    ex      de,hl
    exx
    Macro_Intersect_X0_BC_X1
    pop     bc
    ld      a,b
    or      c
    ex      de,hl
    exx
    ex      de,hl
    exx

PersDrawLinesClipXY_ScissorLeft_End:

;------------------------------------------------------------------------------
PersDrawLinesClipXY_ScissorRight:
                        ; a = clip bit0 | clip bit1
                        ; de = x1
                        ; hl = x0
                        ; de' = y1
                        ; hl' = y0

    Macro_Bit_Right   a
    jp      z,PersDrawLinesClipXY_ScissorTop

    push    af
    ld      bc,(#_g_clipRect + #m_s16Right)
    call    ClipRight
    or      a
    jp      m,PersDrawLinesClipXY_ScissorEnd
    pop     af

;------------------------------------------------------------------------------
PersDrawLinesClipXY_ScissorTop:
                        ; a = clip bit0 | clip bit1
                        ; de = x1
                        ; hl = x0
                        ; de' = y1
                        ; hl' = y0

    exx
    Macro_Bit_Top a
    jp      z,PersDrawLinesClipXY_ScissorBottom

    push    af
    ld      bc,(#_g_clipRect + #m_s16Top)
    call    ClipLeft
    or      a
    jp      m,PersDrawLinesClipXY_ScissorEnd
    pop     af

;------------------------------------------------------------------------------
PersDrawLinesClipXY_ScissorBottom:
                        ; a = clip bit0 | clip bit1
                        ; de = y1
                        ; hl = y0
                        ; de' = x1
                        ; hl' = x0

    Macro_Bit_Bottom a
    jp      z,PersDrawLinesClipXY_DrawLine

    ld      bc,(#_g_clipRect + #m_s16Bottom)
    call    ClipRight
    or      a
    jp      m,PersDrawLinesClipXY_ScissorEnd + #0x0001

;------------------------------------------------------------------------------
PersDrawLinesClipXY_DrawLine:
                        ; de = y1
                        ; hl = y0
                        ; de' = x1
                        ; hl' = x0
    ld  a,l             ; a' = y0
    ex  af,af'          
    ld  a,e             ; a = y1
    exx
    ld  d,e             ; d = x1
    ld  e,a             ; e = y1
    ld  h,l             ; h = x0
    ex  af,af'
    ld  l,a             ; l = y0

    call    VDP_HLDE_LINE

    pop     bc          ; b' = loop counter
    exx
    pop     hl          ; hl = line address    
    jp      PersDrawLinesClipXY_Next

PersDrawLinesClipXY_ScissorEnd:
    pop     af
    pop     bc          ; b' = loop counter
    exx
    pop     hl          ; hl = line address    
    jp      PersDrawLinesClipXY_Next

;------------------------------------------------------------------------------
; extern void PersDrawLinesClipXY(const u16* pLines, u8 nbLines);
;------------------------------------------------------------------------------
            .globl  _PersDrawLinesClipXY

_PersDrawLinesClipXY:
    ld      hl,#0x0002
    add     hl,sp

    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    ex      de,hl

    call    PersDrawLinesClipXY

    ret