;------------------------------------------------------------------------------
; pers_transform_clip_test
;------------------------------------------------------------------------------
                .include "pers_defs.s"

;-------------------------------------------------------------------------------
; PersTransformUnsignedTest
; fetch transform table only
; in inAddress 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; keep D 3d position z
; out CY 1 = clip, 0 = no clip
; use A,HL
;------------------------------------------------------------------------------
            
.macro  PersTransformUnsignedTest   inAddress
    ld      hl,#inAddress
    ld      a,l     ; hl |= z
    or      d
    ld      l,a      
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
.endm

;------------------------------------------------------------------------------
; in B view x
; in D view z   : near <= z < far
; out A clip bits
; use C,E,HL
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersTransformClipTestX

PersTransformClipTestX:
    xor     a           ; a = x
    or      b
    jp      p,PersTransformClipTestX_TestRight

PersTransformClipTestX_TestLeft:
    ld      a,b         ; e = -x
    neg
    ld      e,a
    PersTransformUnsignedTest   0xC100
    ld      a,#0
    ret     nc
    or      #kClipBit_Left
    ret

PersTransformClipTestX_TestRight:
    ld      e,a         ; e = x
    PersTransformUnsignedTest   0xC180
    ld      a,#0
    ret     nc
    or      #kClipBit_Right
    ret

;------------------------------------------------------------------------------
; in A clip bit z
; in B view y
; in D view z
; out A clip bits
; use C,E
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersTransformClipTestY

PersTransformClipTestY:
    ld      c,a
    and     #kClipBit_Near
    jp      z,PersTransformClipTestY_TestY

PersTransformClipTestY_TestNear:
    bit     7,b
    jp      nz,PersTransformClipTestY_ClipNearBottom

PersTransformClipTestY_ClipNearTop:
    ld      a,#kClipBit_Top
    or      c
    ret

PersTransformClipTestY_ClipNearBottom:
    ld      a,#kClipBit_Bottom
    or      c
    ret

PersTransformClipTestY_TestY:
    xor     a           ; a = y
    or      b
    jp      p,PersTransformClipTestY_TestBottom

PersTransformClipTestY_TestTop:
    ld      a,b         ; e = -y
    neg
    ld      e,a
    PersTransformUnsignedTest   0xC200
    ld      a,c
    ret     nc
    or      #kClipBit_Top
    ret

PersTransformClipTestY_TestBottom:
    ld      e,a         ; e = y
    PersTransformUnsignedTest   0xC280
    ld      a,c
    ret     nc
    or      #kClipBit_Bottom
    ret

;------------------------------------------------------------------------------
; in B view x
; in C view y
; in D view z
; in E near
; out A clip bits
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersTransformClipTest

PersTransformClipTest:

PersTransformClipTest_TestZ:
PersTransformClipTest_TestFar:
    xor     a
    or      d           ; a = z
    jp      p, PersTransformClipTest_TestNear
    ld      a,#kClipBit_Far
    ret

PersTransformClipTest_TestNear:
    ld      a,d        ; a = z - near
    cp      e
    jp      p, PersTransformClipTest_TestX
    ld      a,#kClipBit_Near

PersTransformClipTest_TestNearX:
    bit     7,b
    jp      z,PersTransformClipTest_TestNearClipRight

PersTransformClipTest_TestNearClipLeft:
    or      #kClipBit_Left
    jp      PersTransformClipTest_TestNearY

PersTransformClipTest_TestNearClipRight:
    or      #kClipBit_Right

PersTransformClipTest_TestNearY:
    bit     7,c
    jp      z,PersTransformClipTest_TestNearClipBottom

PersTransformClipTest_TestNearClipTop:
    or      #kClipBit_Top
    ret

PersTransformClipTest_TestNearClipBottom:
    or      #kClipBit_Bottom
    ret

PersTransformClipTest_TestX:
    xor     a           ; a = x
    or      b
    jp      p,PersTransformClipTest_TestRight

PersTransformClipTest_TestLeft:
    ld      a,b         ; e = -x
    neg
    ld      e,a
    PersTransformUnsignedTest   0xC100
    jp      nc,PersTransformClipTest_TestY
    ld      b,#kClipBit_Left
    jp      PersTransformClipTest_TestY

PersTransformClipTest_TestRight:
    ld      e,a         ; e = x
    PersTransformUnsignedTest   0xC180
    jp      nc,PersTransformClipTest_TestY
    ld      b,#kClipBit_Right

PersTransformClipTest_TestY:
    xor     a           ; a = y
    or      c
    jp      p,PersTransformClipTest_TestBottom

PersTransformClipTest_TestTop:
    ld      a,c         ; e = -y
    neg
    ld      e,a
    PersTransformUnsignedTest   0xC200
    ld      a,b
    ret     nc
    or      #kClipBit_Top
    ret

PersTransformClipTest_TestBottom:
    ld      e,c         ; e = y
    PersTransformUnsignedTest   0xC280
    ld      a,b
    ret     nc
    or      #kClipBit_Bottom
    ret

; in A = near
; in B = minX
; in C = maxX
; in D = minY
; in E = maxY
; in H = minZ
; in L = maxZ
; out A
                .area   _CODE
                .globl  PersTransformClipTestBBox


PersTransformClipTestBBox:
    dec     a               ; a = near - 1
PersTransformClipTestBBox_TestMaxZ:
    bit     7,l
    jp      nz,PersTransformClipTestBBox_MaxZ_Near  ; clip out
    cp      l               ; near - 1 - maxZ
    jp      m,PersTransformClipTestBBox_TestMinZ
       
PersTransformClipTestBBox_MaxZ_Near:
PersTransformClipTestBBox_Far_MinZ:
PersTransformClipTestBBox_Right_MinX:
PersTransformClipTestBBox_MaxX_Left:
PersTransformClipTestBBox_Bottom_MinY:
PersTransformClipTestBBox_MaxY_Top:
    ld      a,#kBBoxClip_Out
    ret

PersTransformClipTestBBox_TestMinZ:
    bit     7,h
    jp      nz,PersTransformClipTestBBox_Far_MinZ   ; clip out
    cp      h               ; near - 1 - minZ
    jp      m,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far

PersTransformClipTestBBox_MinZ_Near_MaxZ:
PersTransformClipTestBBox_MinZ_Near_MaxZ_TestRight:
    bit     7,b
    jp      nz,PersTransformClipTestBBox_MinZ_Near_MaxZ_TestLeft
                            ; 0 <= minX
    ld      a,#0x80         ; a = 0x80 | maxZ
    or      l
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)
    exx                     ; a = right
    sub     b               ; right - minX
    jp      m,PersTransformClipTestBBox_Right_MinX  ; clip out

PersTransformClipTestBBox_MinZ_Near_MaxZ_TestLeft:
    bit     7,c
    jp      z,PersTransformClipTestBBox_MinZ_Near_MaxZ_TestBottom
                            ; maxX < 0
    ld      a,l
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)
    exx                     ; a = left
    add     a,c             ; left + maxX
    jp      m,PersTransformClipTestBBox_MaxX_Left   ; clip out

PersTransformClipTestBBox_MinZ_Near_MaxZ_TestBottom:
    bit     7,d
    jp      nz,PersTransformClipTestBBox_MinZ_Near_MaxZ_TestTop
                            ; 0 <= minY
    ld      a,#0x80
    or      l
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)
    exx                     ; a = bottom
    sub     d               ; bottom - minY
    jp      m,PersTransformClipTestBBox_Bottom_MinY ; clip out

PersTransformClipTestBBox_MinZ_Near_MaxZ_TestTop:
    bit     7,e
    jp      z,PersTransformClipTestBBox_MinZ_Near_MaxZ_End
                            ; maxY < 0
    ld      a,l
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)
    exx                     ; a = top
    add     a,e             ; top + maxY
    jp      m,PersTransformClipTestBBox_MaxY_Top    ; clip out

PersTransformClipTestBBox_MinZ_Near_MaxZ_End:
    ld      a,#kBBoxClip_ScissorXYZ                 ; clip xyz
    ret

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far:
PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestLeft:
PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestLeft_MaxX:
    bit     7,c
    jp      z,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestLeft_MinX
                        ; maxX < 0
    ld      a,l
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)
    exx                 ; a = left at maxZ
    add     a,c         ; left + maxX
    jp      m,PersTransformClipTestBBox_MaxX_Left   ; Clip out

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestLeft_MinX:
    bit     7,b
    jp      z,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight_MinX
                            ; minX < 0
    ld      a,h
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)
    exx                     ; a = left at minZ
    add     a,b             ; left + minX
    jp      p,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight_MaxX    ; left -- minX  -- maxX

PersTransformClipTestBBox_MinX_Left_MaxX:
    ld      a,#kBBoxClip_ScissorXY
    ret

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight:
PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight_MinX:
    bit     7,b
    jp      nz,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight_MaxX
                            ; 0 < minX
    ld      a,#0x80
    or      l
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)          ; a = right at maxZ
    exx
    sub     b               ; right - minX
    jp      m,PersTransformClipTestBBox_Right_MinX  ; clip out

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestRight_MaxX:
    bit     7,c
    jp      nz,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop
                            ; 0 < maxX
    ld      a,#0x80
    or      h
    exx
    ld      h,#0xC1
    ld      l,a
    ld      a,(hl)          ; a = left at minZ
    exx
    sub     c               ; right - maxX
    jp      p,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop

PersTransformClipTestBBox_MinX_Right_MinX:
    ld      a,#kBBoxClip_ScissorXY
    ret

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop:
PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop_MaxY:
    bit     7,e
    jp      z,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop_MinY
                            ; maxY < 0
    ld      a,l
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)          ; a = top at maxZ
    exx
    add     a,e             ; top + maxY
    jp      m,PersTransformClipTestBBox_MaxY_Top    ; clip out

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestTop_MinY:
    bit     7,d
    jp      z,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestBottom_MinY     
                            ; minY < 0
    ld      a,h
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)          ; a = top at minZ
    exx
    add     a,d             ; top + minY
    jp      p,PersTransformClipTestBBox_NoClip

PersTransformClipTestBBox_MinY_Top_MaxY:
    ld      a,#kBBoxClip_ScissorXY
    ret

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestBottom_MinY:
    bit     7,d
    jp      nz,PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestBottom_MaxY
                            ; 0 < minY
    ld      a,#0x80
    or      l
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)          ; a = bottom at maxZ
    exx
    sub     d               ; bottom - minY
    jp      m,PersTransformClipTestBBox_Bottom_MinY ; clip out

PersTransformClipTestBBox_Near_MinZ_MaxZ_Far_TestBottom_MaxY:
    bit     7,e
    jp      nz,PersTransformClipTestBBox_NoClip
                            ; 0 < maxY
    ld      a,#0x80
    or      h
    exx
    ld      h,#0xC2
    ld      l,a
    ld      a,(hl)          ; a = bottom at minZ
    exx
    sub     e               ; bottom - maxY
    jp      p,PersTransformClipTestBBox_NoClip

PersTransformClipTestBBox_MinY_Bottom_MaxY:
    ld      a,#kBBoxClip_ScissorXY
    ret

PersTransformClipTestBBox_NoClip:
    ld      a,#kBBoxClip_In
    ret










