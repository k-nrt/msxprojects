;------------------------------------------------------------------------------
; pers_transform_clip_xy
;------------------------------------------------------------------------------
                .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformClipXY
; DE = vramLow 16
; B  = vertex count
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  VDPReadBegin116DI
                .globl  PersTransformUnsigned
                .globl  PersTransformClipXY

PersTransformClipXY:
    ld      a,b
    ex      af,af'                      ; A' = vertex count
    ld      ix,#PersScreenPositionsAddress
    ld      a,(#_g_persContext+#m_vramHigh)
    ld      b,a 
    call    VDPReadBegin116DI           ; B = vram high 1, DE = vram low 16

    ex      af,af'
    ld      b,a                         ; B = vertex count
    ld      a,c                         ; A = i/o port vram read
    exx
    ld      c,a                         ; C' = i/o port vram read
    ld      hl,#_g_persContext+#m_v3PositionX
    ld      d,(hl)                      ; D' = m_v3PositionX
    inc     hl
    ld      e,(hl)                      ; E' = m_v3PositionY
    inc     hl
    ld      l,(hl)                      ; L' = m_v3PositionZ
    exx

    ; const C' = i/o vram read
    ; const D' = m_v3PositionX
    ; const E' = m_v3PositionY
    ; const L' = m_v3PositionZ

PersTransformClipXY_LoopStart:
    ; var B = counter
    ; var C = vertex clip flag
    ; work A, DE, HL
    ld      c,#0x00         ; clear clip flag

PersTransformClipXY_LoadVertex:
    ; var B' = x + m_v3PositionX
    ; var H' = y + m_v3PositionY
    ; var A = z + m_v3PositionZ
    exx
    in      a,(c)
    add     a,d
    ld      b,a             ; b' = x + m_v3PositionX

    in      a,(c)
    add     a,e
    ld      h,a             ; h' = y + m_v3PositionY

    in      a,(c)
    add     a,l             ; a = z + m_v3PositionZ

    exx
    jp      m,PersTransformClipXY_ClipNear

    ld      d,a             ; d = z + m_v3PositionZ
    ld      a,(#_g_persContext+#m_clipNear)
    cp      d
    jp      nc,PersTransformClipXY_ClipNear
    jp      p,PersTransformClipXY_ClipFar

PersTransformClipXY_TransformX:
    exx                     ; a = x + m_v3PositionX
    ld      a,b
    exx
    or      a
    jp      m,PersTransformClipXY_TestLeft

PersTransformClipXY_TestRight:
    ld      e,a             ; e = x + m_v3PositionX
    ld      hl,#PersTransform_Right
    push    de
    push    bc
    call    PersTransformUnsigned
    pop     bc
    jp      c,PersTransformClipXY_TestRight_StoreWord

PersTransformClipXY_TestRight_StoreByte:
    ld      a,(#_g_persContext+#m_s16ScreenX)
    add     a,e
    ld      1(ix),a         ; store screenX low
    ld      3(ix),#0x00     ; store screenX high
    pop     de              ; d = z + m_v3PositionZ
    jp      PersTransformClipXY_TransformY

PersTransformClipXY_TestRight_StoreWord:
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    add     hl,de
    ld      1(ix),l         ; store screenX low
    ld      3(ix),h         ; store screenX high

                            ; 8:8 RcpZ precision workaround
                            ; hl = right - screenX
    ld      de,(#_g_persContext+#m_viewPortRight)
    or      a
    ex      de,hl
    sbc     hl,de
    pop     de              ; d = z + m_v3PositionZ
    jp      p,PersTransformClipXY_TransformY

    ld      a,#kClipBit_Right   ; c |= kClipBit_Right
    or      c
    ld      c,a
    jp      PersTransformClipXY_TransformY

PersTransformClipXY_LoopEnd:
    ld      0(ix),c         ; store vertex clip
    ld      de,#0x0008      ; ix += sizeof(SPersScreenPos)
    add     ix,de
    djnz    PersTransformClipXY_LoopStart

    ei
    ret

PersTransformClipXY_TestLeft:
    neg
    ld      e,a             ; e = -x
    ld      hl,#PersTransform_Left
    push    de
    push    bc
    call    PersTransformUnsigned
    pop     bc
    jp      c,PersTransformClipXY_TestLeft_StoreWord

PersTransformClipXY_TestLeft_StoreByte:
    ld      a,(#_g_persContext+#m_s16ScreenX)
    sub     a,e
    ld      1(ix),a         ; store screenX low
    ld      3(ix),#0x00     ; store screenX high
    pop     de              ; d = z + m_v3PositionZ
    jp      PersTransformClipXY_TransformY

PersTransformClipXY_TestLeft_StoreWord:
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    or      a
    sbc     hl,de
    ld      1(ix),l         ; store screenX low
    ld      3(ix),h         ; store screenX high

                            ; 8:8 RcpZ precision workaround
                            ; hl = screenX - left
    ld      de,(#_g_persContext+#m_viewPortLeft)
    or      a
    sbc     hl,de
    pop     de              ; d = z + screenZ  
    jp      p,PersTransformClipXY_TransformY

    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jp      PersTransformClipXY_TransformY

PersTransformClipXY_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    jp      PersTransformClipXY_LoopEnd

PersTransformClipXY_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    jp      PersTransformClipXY_LoopEnd
  
PersTransformClipXY_TransformY:
    exx                     ; a = y + m_v3PositionY
    ld      a,h
    exx
    or      a
    jp      m,PersTransformClipXY_TestTop

PersTransformClipXY_TestBottom:
    ld      e,a             ; e = y + m_v3PositionY
    ld      hl,#PersTransform_Bottom
    push    bc
    call    PersTransformUnsigned
    pop     bc
    jp      c,PersTransformClipXY_TestBottom_StoreWord

PersTransformClipXY_TestBottom_StoreByte:
    ld      a,(#_g_persContext+#m_s16ScreenY)
    add     a,e
    ld      2(ix),a         ; store screenY low
    ld      4(ix),#0x00     ; store screenY high
    jp      PersTransformClipXY_LoopEnd

PersTransformClipXY_TestBottom_StoreWord:
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    add     hl,de
    ld      2(ix),l         ; store screenY low
    ld      4(ix),h         ; store screenY high

                            ; 8:8 RcpZ precision workaround
                            ; a = bottom - screenY
    ld      de,(#_g_persContext+#m_viewPortBottom)
    ex      de,hl
    or      a
    sbc     hl,de
    jp      p,PersTransformClipXY_LoopEnd

    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    jp      PersTransformClipXY_LoopEnd

PersTransformClipXY_TestTop:
    neg
    ld      e,a             ; e = -y
    ld      hl,#PersTransform_Top
    push    bc
    call    PersTransformUnsigned
    pop     bc
    jp      c,PersTransformClipXY_TestTop_StoreWord

PersTransformClipXY_TestTop_StoreByte:
    ld      a,(#_g_persContext+#m_s16ScreenY)
    sub     a,e
    ld      2(ix),a         ; store screenY low
    ld      4(ix),#0x00     ; store screenY high
    jp      PersTransformClipXY_LoopEnd

PersTransformClipXY_TestTop_StoreWord:
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    or      a
    sbc     hl,de
    ld      2(ix),l         ; store screenY low
    ld      4(ix),h         ; store screenY high

                            ; 8:8 RcpZ precision workaround
                            ; hl = screenY - top
    ld      de,(#_g_persContext+#m_viewPortTop)
    or      a
    sbc     hl,de
    jp      p,PersTransformClipXY_LoopEnd

    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    jp      PersTransformClipXY_LoopEnd
