;------------------------------------------------------------------------------
; pers_transform_clip_xyz
;------------------------------------------------------------------------------
                .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformNoClip
; DE = vramLow 16
; B  = vertex count
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  VDPReadBegin116DI
                .globl  PersTransformUnsignedFast
                .globl  PersTransformNoClip

PersTransformNoClip:
    ld      a,b
    ex      af,af'                      ; A' = vertex count
    ld      ix,#0xDF00
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

    ; var A' = model clip flags

    ; const C' = i/o vram read
    ; const D' = m_v3PositionX
    ; const E' = m_v3PositionY
    ; const L' = m_v3PositionZ

PersTransformNoClip_LoopStart:
    ; var B = counter
    ; var C = vertex clip flags

    ld      c,#0x00 ; clear clip flag

PersTransformNoClip_LoadVertex:
    ; var B' = x + m_v3PositionX
    ; var H' = y + m_v3PositionY
    ; var A = z + m_v3PositionZ
    exx
    in      a,(c)
    add     a,d
    ld      b,a     ; b' = x + m_v3PositionX

    in      a,(c)
    add     a,e
    ld      h,a     ; h' = y + m_v3PositionY

    in      a,(c)
    add     a,l     ; a = z + m_v3PositionZ

    exx
    ;jr      c,PersTransformNoClip_ClipNear

    ld      d,a     ; d = z + m_v3PositionZ
    ld      a,(#_g_persContext+#m_clipNear)
    cp      d
    jr      nc,PersTransformNoClip_ClipNear

    bit     7,d
    jr      nz,PersTransformNoClip_ClipFar

PersTransformNoClip_TransformX:
    exx             ; a = x + m_v3PositionX
    ld      a,b
    exx
    bit     7,a
    jr      nz,PersTransformNoClip_TestLeft

PersTransformNoClip_TestRight:
    ld      e,a     ; e = x + m_v3PositionX
    ld      hl,#0xC180
    call    PersTransformUnsignedFast
    jr      c,PersTransformNoClip_ClipRight
    ld      a,(#_g_persContext+#m_s16ScreenX)
    add     a,e
    ld      1(ix),a
    jr      PersTransformNoClip_TransformY

PersTransformNoClip_ClipRight:
    ld      a,#kClipBit_Right
    or      c
    ld      c,a
    jr      PersTransformNoClip_TransformY

PersTransformNoClip_TestLeft:
    neg
    ld      e,a     ; e = -x
    ld      hl,#0xC100
    call    PersTransformUnsignedFast
    jr      c,PersTransformNoClip_ClipLeft
    ld      a,(#_g_persContext+#m_s16ScreenX)
    sub     e
    ld      1(ix),a
    jr      PersTransformNoClip_TransformY

PersTransformNoClip_ClipLeft:
    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jr      PersTransformNoClip_TransformY

PersTransformNoClip_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    jr      PersTransformNoClip_LoopEnd

PersTransformNoClip_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    ;jr      PersTransformNoClip_LoopEnd
 
PersTransformNoClip_LoopEnd:
    ld      0(ix),c         ; store vertex clip
    ex      af,af'          ; a' |= c
    or      c
    ex      af,af' 
    ld      de,#0x0008      ; ix += sizeof(SPersScreenPos)
    add     ix,de
    djnz    PersTransformNoClip_LoopStart

    ei
    ex      af,af'          ; A = model clip flag
    ret

PersTransformNoClip_TransformY:
    exx             ; a = y + m_v3PositionY
    ld      a,h
    exx
    bit     7,a
    jr      nz,PersTransformNoClip_TestTop

PersTransformNoClip_TestBottom :
    ld      e,a     ; e = y + m_v3PositionY
    ld      hl,#0xC280
    call    PersTransformUnsignedFast
    jr      c,PersTransformNoClip_ClipBottom
    ld      a,(#_g_persContext+#m_s16ScreenY)
    add     a,e
    ld      2(ix),a
    jr      PersTransformNoClip_LoopEnd

PersTransformNoClip_ClipBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    jr      PersTransformNoClip_LoopEnd

PersTransformNoClip_TestTop:
    neg
    ld      e,a     ; e = -y
    ld      hl,#0xC200
    call    PersTransformUnsignedFast
    jr      c,PersTransformNoClip_ClipTop
    ld      a,(#_g_persContext+#m_s16ScreenY)
    sub     e
    ld      2(ix),a
    jr      PersTransformNoClip_LoopEnd

PersTransformNoClip_ClipTop:
    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    jr      PersTransformNoClip_LoopEnd
