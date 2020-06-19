;------------------------------------------------------------------------------
; pers_transform_clip_xy
;------------------------------------------------------------------------------
            .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersTransformUnsigned
; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; out DE screen position [unsigned]
; out CY 1 = clip, 0 = no clip
; use A,B,C,A'
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  MulCoreUnsigned_BxDE_AHL
PersTransformUnsigned:
    ld      a,l     ; hl |= z
    or      d
    ld      l,a      
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jr      c,PersTransformUnsigned_Clip
    ld      h,#0xC0
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
    ex      de,hl   ; hl = screen position
    ld      d,a     ; D = z
    or      a       ; CY = 0
    ret

PersTransformUnsigned_Clip:
    ld      b,e         ; b = x
    ld      h,#0xDE
    ld      l,d
    sla     l
    ld      e,(hl)      ; de = screenZ/z
    inc     l
    ld      d,(hl)
    call    MulCoreUnsigned_BxDE_AHL
    ld      e,h         ; ahl = screenX[16:8]
    ld      d,a         ; de = screenX
    scf                 ; CY = 1
    ret

;------------------------------------------------------------------------------
; PersTransformClipXY
; DE = vramLow 16
; B  = vertex count
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  VDPReadBegin116DI
PersTransformClipXY:
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
    ;jr      c,PersTransformClipXY_ClipNear

    ld      d,a             ; d = z + m_v3PositionZ
    ld      a,(#_g_persContext+#m_clipNear)
    cp      d
    jr      nc,PersTransformClipXY_ClipNear

    bit     7,d
    jr      nz,PersTransformClipXY_ClipFar

PersTransformClipXY_TransformX:
    exx                     ; a = x + m_v3PositionX
    ld      a,b
    exx
    bit     7,a
    jr      nz,PersTransformClipXY_TestLeft

PersTransformClipXY_TestRight:
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
    jr      nc,PersTransformClipXY_TransformY

PersTransformClipXY_ClipRight:
    ld      a,#kClipBit_Right
    or      c
    ld      c,a
    jr      PersTransformClipXY_TransformY

PersTransformClipXY_TestLeft:
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
    jr      nc,PersTransformClipXY_TransformY

PersTransformClipXY_ClipLeft:
    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jr      PersTransformClipXY_TransformY

PersTransformClipXY_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    jr      PersTransformClipXY_LoopEnd

PersTransformClipXY_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    ;jr     PersTransformClipXY_LoopEnd
 
PersTransformClipXY_LoopEnd:
    ld      0(ix),c         ; store vertex clip
    ld      de,#0x0008      ; ix += sizeof(SPersScreenPos)
    add     ix,de
    djnz    PersTransformClipXY_LoopStart

    ei
    ret

PersTransformClipXY_TransformY:
    exx                     ; a = y + m_v3PositionY
    ld      a,h
    exx
    bit     7,a
    jr      nz,PersTransformClipXY_TestTop

PersTransformClipXY_TestBottom :
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
    jr      nc,PersTransformClipXY_LoopEnd

PersTransformClipXY_ClipBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    jr      PersTransformClipXY_LoopEnd

PersTransformClipXY_TestTop:
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
    jr      nc,PersTransformClipXY_LoopEnd

PersTransformClipXY_ClipTop:
    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    jr      PersTransformClipXY_LoopEnd

;------------------------------------------------------------------------------
; extern void PersTransformClipXYVram(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersTransformClipXYVram

_PersTransformClipXYVram:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    push    ix
    call    PersTransformClipXY
    pop     ix
    ret