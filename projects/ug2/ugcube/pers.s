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
PersSetVerticesVram:
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
; extern u8 PersSetVerticesVram(u16 vramOffset, u8 nbVertices);
;------------------------------------------------------------------------------
            .area   _CODE
            .globl  _PersSetVerticesVram
_PersSetVerticesVram:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    push    ix
    call    PersSetVerticesVram
    pop     ix
    ret