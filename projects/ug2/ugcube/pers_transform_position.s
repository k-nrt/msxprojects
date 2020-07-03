;------------------------------------------------------------------------------
; pers_transform_position
;------------------------------------------------------------------------------
                .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformPosition
; in D = x + m_v3PositionX
; in E = y + m_v3PositionY
; in B = z + m_v3PositionZ
; out IX = s16x2 position
; out C  = clip flag
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersTransformUnsigned
                .globl  PersTransformPosition

PersTransformPosition:
    ld      c,#0x00         ; clear clip flag

PersTransformPosition_TestFar:
    ld      a,b
    or      a
    jp      m,PersTransformPosition_ClipFar

PersTransformPosition_TestNear:
    ld      a,(#_g_persContext+#m_clipNear)
    sub     b
    dec     a               ; a = near - z - 1 
    jp      p,PersTransformPosition_ClipNear

PersTransformPosition_TransformX:
    ld      a,d             ; a = x + m_v3PositionX
    or      a
    jp      m,PersTransformPosition_TestLeft

PersTransformPosition_TestRight:
    push    de
    ld      d,b             ; d = z + m_v3PositionZ
    ld      e,a             ; e = x + m_v3PositionX
    ld      hl,#0xC180
    push    bc

    ; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
    ; in D 3d clipped z [near --- 127]
    ; in E 3d position [unsigned]
    ; out DE screen position [unsigned]
    ; out CY 1 = clip, 0 = no clip
    ; use A,B,C,A'
    call    PersTransformUnsigned

    pop     bc              ; b = z + m_v3PositionZ, c = clip flag
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    add     hl,de
    pop     de              ; d = x + m_v3PositionX, e = y + m_v3PositionY
    ld      0(ix),l         ; store screenX low
    ld      1(ix),h         ; store screenX high
    ex      af,af'          ; pop af
    jp      nc,PersTransformPosition_TransformY

PersTransformPosition_ClipRight:
    ld      a,#kClipBit_Right
    or      c
    ld      c,a
    jp      PersTransformPosition_TransformY

PersTransformPosition_TestLeft:
    push    de
    ld      d,b             ; d = z + m_v3PositionZ
    neg
    ld      e,a             ; e = -(x + m_v3PositionX)
    ld      hl,#0xC100
    push    bc

    ; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
    ; in D 3d clipped z [near --- 127]
    ; in E 3d position [unsigned]
    ; out DE screen position [unsigned]
    ; out CY 1 = clip, 0 = no clip
    ; use A,B,C,A'
    call    PersTransformUnsigned

    pop     bc              ; b = z + m_v3PositionZ, c = clip flag
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    or      a
    sbc     hl,de
    pop     de              ; d = x + m_v3PositionX, e = y + m_v3PositionY
    ld      0(ix),l         ; store screenX low
    ld      1(ix),h         ; store screenX high
    ex      af,af'          ; pop af
    jp      nc,PersTransformPosition_TransformY

PersTransformPosition_ClipLeft:
    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jp      PersTransformPosition_TransformY

PersTransformPosition_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    ret

PersTransformPosition_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    ret
  
PersTransformPosition_TransformY:
    ld      a,e             ; a = y + m_v3PositionY
    or      a
    jp      m,PersTransformPosition_TestTop

PersTransformPosition_TestBottom :
    ld      d,b             ; d = z + m_v3PositionZ
    ;ld      e,a             ; e = y + m_v3PositionY
    ld      hl,#0xC280
    push    bc

    ; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
    ; in D 3d clipped z [near --- 127]
    ; in E 3d position [unsigned]
    ; out DE screen position [unsigned]
    ; out CY 1 = clip, 0 = no clip
    ; use A,B,C,A'
    call    PersTransformUnsigned
 
    pop     bc              ; b = z + m_v3PositionZ, c = clip flag
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    add     hl,de
    ld      2(ix),l         ; store screenY low
    ld      3(ix),h         ; store screenY high
    ex      af,af'          ; pop af
    ret     nc

PersTransformPosition_ClipBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    ret

PersTransformPosition_TestTop:
    ld      d,b             ; d = z + m_v3PositionZ
    neg
    ld      e,a             ; e = -(y + m_v3PositionY)
    ld      hl,#0xC200
    push    bc

    ; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
    ; in D 3d clipped z [near --- 127]
    ; in E 3d position [unsigned]
    ; out DE screen position [unsigned]
    ; out CY 1 = clip, 0 = no clip
    ; use A,B,C,A'
    call    PersTransformUnsigned

    pop     bc              ; b = z + m_v3PositionZ, c = clip flag
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    or      a
    sbc     hl,de
    ld      2(ix),l         ; store screenY low
    ld      3(ix),h         ; store screenY high
    ex      af,af'          ; pop af
    ret     nc

PersTransformPosition_ClipTop:
    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    ret
