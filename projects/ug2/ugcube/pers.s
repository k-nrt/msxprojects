;------------------------------------------------------------------------------
; pers
;------------------------------------------------------------------------------
            .include    "pers_defs.s"

;------------------------------------------------------------------------------
; PersRegister
; in A  vram high 1
; in HL vram low 16
; in HL' vertex address
; in B'  vertex count
; in E'  vertex left shift count 0 - 5
;------------------------------------------------------------------------------
            .area   _CODE

PersRegister:
    ex      af,af'          ; a' = vram high 1
    ld      a,(#VDPWrite)
    inc     a
    ld      c,a             ; c = write control register
    ex      af',af          ; a = vram high 1

    rlc     h               ; ah <<= 2
    rla
    rlc     h
    rla
    and     #0x07
    di
    out     (c),a           ; a = vram address upper 3 bits
    ld      a,#0x80+#14     ; 0b10xxxxxx 
    out     (c),a           ; a = write control register #14

    ld      a,l
    out     (c),a           ; a = vram address lower 8 bits
    srl     h               ; h >>= 2
    srl     h               ; h = restore vram address and fill 0 to upper 2 bits
    ld      a,#0x40         ; 0b01xxxxxx
    or      h
    out     (c),a           ; a = write vram address middle 6 bits

    dec     c               ; c = write vram
    ld      a,c
    exx                     ; hl = vertex address, b = vertex count, e = shift
    ld      c,a 

PersRegister_Loop:
    ld      a,(hl)          ; x
    call    PersRegister_Shift
    out     (c),a
    inc     hl
    ld      a,(hl)          ; y
    call    PersRegister_Shift
    out     (c),a
    inc     hl
    ld      a,(hl)          ; z
    call    PersRegister_Shift
    out     (c),a
    inc     hl

    djnz    PersRegister_Loop
    ei
    ret

PersRegister_Shift:
    ld      d,e
PersRegister_Shift_Loop:
    dec     d
    ret     c  
    sla     a
    jr      PersRegister_Shift_Loop

;------------------------------------------------------------------------------
; extern u16 PersRegisterVertices(s8x3 *pVertices, u8 nbVertices, u8 shift);
;------------------------------------------------------------------------------
;	.area	_CODE
;	.globl	_PersRegisterVertices
;_PersRegisterVertices:
    ld      hl,#0x0002  ; skip return address
    add     hl,sp
    ld      e,(hl)      ; de = pVertices
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)      ; b = nbVertices
    inc     hl
    ld      c,(hl)      ; c = shift
    ex      de,hl       ; hl = pVertices
    ld      e,c         ; e = shift
    ld      a,b         ; a = nbVertices
    exx                 ; b' = nbVertices, e' = shift, hl' = pVertices

    ld      d,#0x00
    ld      e,a
    ld      h,d
    ld      l,a
    add     hl,hl
    add     hl,de       ; hl = nbVertices*3
    ex      de,hl       ; de = nbVertices*3
    ld      a,(#_g_persContext+#m_vramHigh)
    ld      hl,(#_g_persContext+#m_vramOffset)
    push    hl
    push    af
    add     hl,de
    adc     a,#0x00
    ld      (#_g_persContext+#m_vramHigh),a
    ld      (#_g_persContext+#m_vramOffset),hl
    pop     af
    pop     hl
    call    PersRegister
    ret


;------------------------------------------------------------------------------
            .area   _CODE
            .globl  VDPReadBegin116DI

;------------------------------------------------------------------------------
; PersGetTransformUnsigned
; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; keep D 3d position z
; out E screen [unsigned]
; out CY 1 = clip, 0 = no clip
; use A
;------------------------------------------------------------------------------
PersGetTransformUnsigned:
    ld      a,l     ; hl |= z
    or      d
    ld      l,a      
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jr      c,PersGetTransformUnsigned_Clip
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
    ld      d,a     ; D = z
    or      a       ; CY = 0
    ret

PersGetTransformUnsigned_Clip:
    scf             ; CY = 1
    ret

;------------------------------------------------------------------------------
; PersSetVerticesVram
; DE = vramLow 16
; B  = vertex count
;------------------------------------------------------------------------------
;PersSetVerticesVram:
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

    ; var A' = model clip flags

    ; const C' = i/o vram read
    ; const D' = m_v3PositionX
    ; const E' = m_v3PositionY
    ; const L' = m_v3PositionZ

PersSetVerticesVram_LoopStart:
    ; var B = counter
    ; var C = vertex clip flags

    ld      c,#0x00 ; clear clip flag

PersSetVerticesVram_LoadVertex:
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
    ;jr      c,PersSetVerticesVram_ClipNear

    ld      d,a     ; d = z + m_v3PositionZ
    ld      a,(#_g_persContext+#m_clipNear)
    cp      d
    jr      nc,PersSetVerticesVram_ClipNear

    bit     7,d
    jr      nz,PersSetVerticesVram_ClipFar

PersSetVerticesVram_TransformX:
    exx             ; a = x + m_v3PositionX
    ld      a,b
    exx
    bit     7,a
    jr      nz,PersSetVerticesVram_TestLeft

PersSetVerticesVram_TestRight:
    ld      e,a     ; e = x + m_v3PositionX
    ld      hl,#0xC180
    call    PersGetTransformUnsigned
    jr      c,PersSetVerticesVram_ClipRight
    ld      a,(#_g_persContext+#m_s16ScreenX)
    add     a,e
    ld      1(ix),a
    jr      PersSetVerticesVram_TransformY

PersSetVerticesVram_ClipRight:
    ld      a,#kClipBit_Right
    or      c
    ld      c,a
    jr      PersSetVerticesVram_TransformY

PersSetVerticesVram_TestLeft:
    neg
    ld      e,a     ; e = -x
    ld      hl,#0xC100
    call    PersGetTransformUnsigned
    jr      c,PersSetVerticesVram_ClipLeft
    ld      a,(#_g_persContext+#m_s16ScreenX)
    sub     e
    ld      1(ix),a
    jr      PersSetVerticesVram_TransformY

PersSetVerticesVram_ClipLeft:
    ld      a,#kClipBit_Left
    or      c
    ld      c,a
    jr      PersSetVerticesVram_TransformY

PersSetVerticesVram_ClipNear:
    ld      a,#kClipBit_Near
    or      c
    ld      c,a
    jr      PersSetVerticesVram_LoopEnd

PersSetVerticesVram_ClipFar:
    ld      a,#kClipBit_Far
    or      c
    ld      c,a
    ;jr      PersSetVerticesVram_LoopEnd
 
PersSetVerticesVram_LoopEnd:
    ld      0(ix),c         ; store vertex clip
    ex      af,af'          ; a' |= c
    or      c
    ex      af,af' 
    ld      de,#0x0008      ; ix += sizeof(SPersScreenPos)
    add     ix,de
    djnz    PersSetVerticesVram_LoopStart

    ei
    ex      af,af'          ; A = model clip flag
    ret

PersSetVerticesVram_TransformY:
    exx             ; a = y + m_v3PositionY
    ld      a,h
    exx
    bit     7,a
    jr      nz,PersSetVerticesVram_TestTop

PersSetVerticesVram_TestBottom :
    ld      e,a     ; e = y + m_v3PositionY
    ld      hl,#0xC280
    call    PersGetTransformUnsigned
    jr      c,PersSetVerticesVram_ClipBottom
    ld      a,(#_g_persContext+#m_s16ScreenY)
    add     a,e
    ld      2(ix),a
    jr      PersSetVerticesVram_LoopEnd

PersSetVerticesVram_ClipBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ld      c,a
    jr      PersSetVerticesVram_LoopEnd

PersSetVerticesVram_TestTop:
    neg
    ld      e,a     ; e = -y
    ld      hl,#0xC200
    call    PersGetTransformUnsigned
    jr      c,PersSetVerticesVram_ClipTop
    ld      a,(#_g_persContext+#m_s16ScreenY)
    sub     e
    ld      2(ix),a
    jr      PersSetVerticesVram_LoopEnd

PersSetVerticesVram_ClipTop:
    ld      a,#kClipBit_Top
    or      c
    ld      c,a
    jr      PersSetVerticesVram_LoopEnd

;------------------------------------------------------------------------------
; extern u8 PersTransformNoClipVram(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersTransformNoClipVram
            .globl  PersTransformNoClip

_PersTransformNoClipVram:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    push    ix
    call    PersTransformNoClip
    pop     ix
    ret

;------------------------------------------------------------------------------
; extern void PersTransformClipXYVram(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersTransformClipXYVram
            .globl PersTransformClipXY

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

;------------------------------------------------------------------------------
; extern void PersTransformClipXYZVram(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersTransformClipXYZVram
            .globl PersTransformClipXYZ

_PersTransformClipXYZVram:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    push    ix
    call    PersTransformClipXYZ
    pop     ix
    ret

;------------------------------------------------------------------------------
; extern u8 PersTransformViewPosition(s8x3 *pViewPosition, s16x2 *pScreenPosition);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersTransformViewPosition
            .globl  PersTransformPosition

_PersTransformViewPosition:
    push    ix          ; save ix
    di
    ld      hl,#0x0000
    add     hl,sp       ; hl = sp

    pop     af          ; ix
    pop     af          ; ret
    pop     de          ; de = pViewPosition
    pop     ix          ; ix = pScreenPosition
    ld      sp,hl       ; restore sp
    ei
    ex      de,hl       ; hl = pViewPosition

    ld      d,(hl)      ; d = x
    inc     hl
    ld      e,(hl)      ; e = y
    inc     hl
    ld      b,(hl)      ; b = z

    ; in D = x + m_v3PositionX
    ; in E = y + m_v3PositionY
    ; in B = z + m_v3PositionZ
    ; out IX = s16x2 position
    ; out C  = clip flag
    call    PersTransformPosition

    ld      l,c         ; result = clip flag
    pop     ix          ; restore ix
    ret

;------------------------------------------------------------------------------
; extern u8 PersClipPoint(s8 x, s8 y, s8 z, s8 near);
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  _PersClipPoint
                .globl  PersTransformClipTest

_PersClipPoint:
    ld      hl,#0x0002
    add     hl,sp
    ld      b,(hl)      ; b = x
    inc     hl
    ld      c,(hl)      ; c = y
    inc     hl
    ld      d,(hl)      ; d = z
    inc     hl
    ld      e,(hl)      ; e = near
    call    PersTransformClipTest
    ld      l,a
    ret
;------------------------------------------------------------------------------
; extern u8 PersClipBBox(const SBBox *pBBox, s8 x, s8 y, s8 z, s8 near);
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  _PersClipBBox
                .globl  PersTransformClipTestBBox

_PersClipBBox:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)          ; de = pBBox
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)          ; b = x
    inc     hl
    ld      c,(hl)          ; c = y
    inc     hl
    ld      a,(hl)          ; a = z
    inc     hl
    ld      l,(hl)          ; l = near
    ld      h,a             ; h = z

    ld      a,(de)          ; b' = minX + x
    add     a,b
    exx
    ld      b,a
    exx
    inc     de

    ld      a,(de)          ; c' = maxX + x
    add     a,b
    exx
    ld      c,a
    exx
    inc     de

    ld      a,(de)          ; d' = minY + y
    add     a,c
    exx
    ld      d,a
    exx
    inc     de
    
    ld      a,(de)          ; e' = maxY + y
    add     a,c
    exx
    ld      e,a
    exx
    inc     de

    ld      a,(de)          ; h' = minZ + z
    add     a,h
    exx
    ld      h,a
    exx
    inc     de

    ld      a,(de)          ; l' = maxZ + z
    add     a,h
    exx
    ld      l,a
    exx
    
    ld      a,l             ; a = near
    exx                     ; bc = minX+x,maxX+x
                            ; de = minY+y,maxY+y
                            ; hl = minZ+z,maxZ+z

    call    PersTransformClipTestBBox

    ld      l,a
    ret

;------------------------------------------------------------------------------
; extern u16 PersCreateBBox(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
                .area   _CODE
                .globl _PersCreateBBox

;------------------------------------------------------------------------------
; PersCreateBBox
; in B  vram high 1
; in C  vertex count
; in DE vram low 16 to read vertices
; in HL vram low 16 to write bbox
; use A,A',BC',DE',HL'
;------------------------------------------------------------------------------
                .globl PersCreateBBox

_PersCreateBBox:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)      ; de = vramOffset
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      c,(hl)      ; c = nbVertices

    ld      a,(#_g_persContext+#m_vramHigh)
    ld      b,a         ; b = vram high

    ld      hl,(#_g_persContext+#m_vramOffset)  ; hl = vram offset

    call    PersCreateBBox

    ld      de,#0x0006                          ; g_persContext.m_vramOffset += 6
    ld      hl,(#_g_persContext+#m_vramOffset)
    ld      b,h                                 ; bc = hl
    ld      c,l
    add     hl,de
    ld      (#_g_persContext+#m_vramOffset),hl

    ld      h,b                                 ; return vram offset
    ld      l,c
    ret

;------------------------------------------------------------------------------
; extern u8 PersClipBBoxVram(u16 vramOffset, s8 x, s8 y, s8 z, s8 near);
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  _PersClipBBoxVram

_PersClipBBoxVram:
    ld      hl,#0x0002
    add     hl,sp

    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl

    ld      a,(#_g_persContext+#m_vramHigh)
    ld      b,a

    call    VDPReadBegin116DI

    ld      b,(hl)      ; b = x
    inc     hl
    ld      d,(hl)      ; d = y
    inc     hl
    ld      e,(hl)      ; e = z
    inc     hl
    ld      a,(hl)      ; a' = near
    ex      af,af'

    in      a,(c)       ; b' = minX + x
    add     a,b
    exx
    ld      b,a
    exx

    in      a,(c)       ; c' = maxX + x
    add     a,b
    exx
    ld      c,a
    exx

    in      a,(c)       ; d' = minY + y
    add     a,d
    exx
    ld      d,a
    exx

    in      a,(c)       ; e' = maxY + y  
    add     a,d
    exx
    ld      e,a
    exx

    in      a,(c)       ; h' = minZ + z
    add     a,e
    exx
    ld      h,a
    exx

    in      a,(c)       ; l' = maxZ + z
    ei
    add     a,e
    exx
    ld      l,a
    
    ex      af,af'      ; a = near
    
    call    PersTransformClipTestBBox

    ld      l,a
    ret
      


