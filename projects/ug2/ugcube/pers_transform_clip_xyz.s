;------------------------------------------------------------------------------
; pers_transform_clip_xyz
;------------------------------------------------------------------------------
                .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformClipXYZ
; DE = vramLow 16
; B  = vertex count
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  VDPReadBegin116DI
                .globl  PersTransformUnsigned
                .globl  PersTransformClipXYZ

PersTransformClipXYZ:
    ld      a,b
    ex      af,af'                      ; A' = vertex count
    ld      ix,#0xDF00                  ; ix = internal vertex buffer
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

PersTransformClipXYZ_LoopStart:
    ; var B = counter
    ; var C = vertex clip flag
    ; work A, DE, HL
    ld      c,#0x00         ; clear clip flag

PersTransformClipXYZ_LoadVertex:
    ; var B' = x + m_v3PositionX
    ; var H' = y + m_v3PositionY
    ; var A = z + m_v3PositionZ
    exx
    in      a,(c)
    add     a,d
    ld      b,a             ; b' = x + m_v3PositionX
    ld      5(ix),a         ; store 3D x

    in      a,(c)
    add     a,e
    ld      h,a             ; h' = y + m_v3PositionY
    ld      6(ix),a         ; store 3D y

    in      a,(c)
    add     a,l             ; a = z + m_v3PositionZ
    ld      5(ix),a         ; store 3D z
    exx
    jp      m,PersTransformClipXYZ_ClipNear

    ld      d,a             ; d = z + m_v3PositionZ
    ld      a,(#_g_persContext+#m_clipNear)
    cp      d
    jp      nc,PersTransformClipXYZ_ClipNear
    jp      p,PersTransformClipXYZ_ClipFar

PersTransformClipXYZ_TransformX:
    exx                     ; a = x + m_v3PositionX
    ld      a,b
    exx
    or      a
    jp      m,PersTransformClipXYZ_TestLeft

PersTransformClipXYZ_TestRight:
    ld      e,a             ; e = x + m_v3PositionX
    ld      hl,#0xC180
    push    de
    push    bc
    call    PersTransformUnsigned
    pop     bc
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    add     hl,de
    pop     de              ; d = z + m_v3PositionZ
    ld      1(ix),l         ; store screenX low
    ld      3(ix),h         ; store screenX high
    ex      af,af'          ; pop af
    jp      nc,PersTransformClipXYZ_TransformY

PersTransformClipXYZ_ClipRight:
    ld      a,#kClipBit_Right
    or      c
    ld      c,a
    jp      PersTransformClipXYZ_TransformY

PersTransformClipXYZ_LoopEnd:
    ld      0(ix),c         ; store vertex clip
    ld      de,#0x0008      ; ix += sizeof(SPersScreenPos)
    add     ix,de
    djnz    PersTransformClipXYZ_LoopStart
    ei
    ret

PersTransformClipXYZ_TestLeft:
    neg
    ld      e,a             ; e = -x
    ld      hl,#0xC100
    push    de
    push    bc
    call    PersTransformUnsigned
    pop     bc
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenX)
    or      a
    sbc     hl,de
    pop     de              ; d = z + screenZ  
    ld      1(ix),l         ; store screenX low
    ld      3(ix),h         ; store screenX high
    ex      af,af'          ; pop af
    jp      nc,PersTransformClipXYZ_TransformY

PersTransformClipXYZ_ClipLeft:
    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jp      PersTransformClipXYZ_TransformY

PersTransformClipXYZ_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    jp      PersTransformClipXYZ_LoopEnd

PersTransformClipXYZ_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    jp     PersTransformClipXYZ_LoopEnd

PersTransformClipXYZ_TransformY:
    exx                     ; a = y + m_v3PositionY
    ld      a,h
    exx
    or      a
    jp      m,PersTransformClipXYZ_TestTop

PersTransformClipXYZ_TestBottom :
    ld      e,a             ; e = y + m_v3PositionY
    ld      hl,#0xC280
    push    bc
    call    PersTransformUnsigned
    pop     bc
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    add     hl,de
    ld      2(ix),l         ; store screenY low
    ld      4(ix),h         ; store screenY high
    ex      af,af'          ; pop af
    jp      nc,PersTransformClipXYZ_LoopEnd

PersTransformClipXYZ_ClipBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    jp      PersTransformClipXYZ_LoopEnd

PersTransformClipXYZ_TestTop:
    neg
    ld      e,a             ; e = -y
    ld      hl,#0xC200
    push    bc
    call    PersTransformUnsigned
    pop     bc
    ex      af,af'          ; push af
    ld      hl,(#_g_persContext+#m_s16ScreenY)
    or      a
    sbc     hl,de
    ld      2(ix),l         ; store screenY low
    ld      4(ix),h         ; store screenY high
    ex      af,af'          ; pop af
    jp      nc,PersTransformClipXYZ_LoopEnd

PersTransformClipXYZ_ClipTop:
    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    jp      PersTransformClipXYZ_LoopEnd
