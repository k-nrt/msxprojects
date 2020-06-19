;------------------------------------------------------------------------------
; clip
;------------------------------------------------------------------------------
    .area   _DATA
    .globl  _g_clipRect
    .globl  _g_clipLineInOut

_g_clipRect:
    .dw     0   ; left
    .dw     0   ; right
    .dw     0   ; top
    .dw     0   ; bottom

_g_clipLineInOut:
    .dw     0   ; sx
    .dw     0   ; sy
    .dw     0   ; ex
    .dw     0   ; ey

;------------------------------------------------------------------------------
    .area	_CODE
    .globl  _ClipLeft
    .globl  _ClipRight
    .globl  _ClipTop
    .globl  _ClipBottom
    .globl  _ClipRect
    .globl  _ClipRect_VDPWaitLine
    .globl  ClipLeft
    .globl  ClipRight
    .globl  IntersectZero
    .globl  VDP_HLDE_LINE

_ClipLeft:
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl' = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de' = ey
    exx
    ld      bc,(#_g_clipRect+#0x00)         ; bc = left
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de = ex
    call    ClipLeft
    ld      (#_g_clipLineInOut+#0x00),hl
    ld      (#_g_clipLineInOut+#0x04),de
    exx
    ld      (#_g_clipLineInOut+#0x02),hl
    ld      (#_g_clipLineInOut+#0x06),de
    ret

_ClipRight:
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl' = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de' = ey
    exx
    ld      bc,(#_g_clipRect+#0x02)         ; bc = right
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de = ex
    call    ClipRight
    ld      (#_g_clipLineInOut+#0x00),hl
    ld      (#_g_clipLineInOut+#0x04),de
    exx
    ld      (#_g_clipLineInOut+#0x02),hl
    ld      (#_g_clipLineInOut+#0x06),de
    ret

_ClipTop:
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl' = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de' = ex
    exx
    ld      bc,(#_g_clipRect+#0x04)         ; bc = top
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de = ey
    call    ClipLeft
    ld      (#_g_clipLineInOut+#0x02),hl
    ld      (#_g_clipLineInOut+#0x06),de
    exx
    ld      (#_g_clipLineInOut+#0x00),hl
    ld      (#_g_clipLineInOut+#0x04),de
    ret

_ClipBottom:
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl' = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de' = ex
    exx
    ld      bc,(#_g_clipRect+#0x06)         ; bc = bottom
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de = ey
    call    ClipRight
    ld      (#_g_clipLineInOut+#0x02),hl
    ld      (#_g_clipLineInOut+#0x06),de
    exx
    ld      (#_g_clipLineInOut+#0x00),hl
    ld      (#_g_clipLineInOut+#0x04),de
    ret

_ClipRect:
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl' = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de' = ey
    exx
    ld      bc,(#_g_clipRect+#0x00)         ; bc = left
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de = ex
    call    ClipLeft
    ld      bc,(#_g_clipRect+#0x02)
    call    ClipRight
    exx
    ld      bc,(#_g_clipRect+#0x04)
    call    ClipLeft
    ld      bc,(#_g_clipRect+#0x06)
    call    ClipRight
    ld      (#_g_clipLineInOut+#0x02),hl
    ld      (#_g_clipLineInOut+#0x06),de
    exx
    ld      (#_g_clipLineInOut+#0x00),hl
    ld      (#_g_clipLineInOut+#0x04),de
    ret

;------------------------------------------------------------------------------
; void ClipRect_VDPWaitLine();
;------------------------------------------------------------------------------
_ClipRect_VDPWaitLine:
    ld      hl,(#_g_clipLineInOut+#0x02)    ; hl' = sy
    ld      de,(#_g_clipLineInOut+#0x06)    ; de' = ey
    exx
    ld      bc,(#_g_clipRect+#0x00)         ; bc = left
    ld      hl,(#_g_clipLineInOut+#0x00)    ; hl = sx
    ld      de,(#_g_clipLineInOut+#0x04)    ; de = ex
    call    ClipLeft
    inc     a
    ret     z
    ld      bc,(#_g_clipRect+#0x02)
    call    ClipRight
    inc     a
    ret     z
    exx
    ld      bc,(#_g_clipRect+#0x04)
    call    ClipLeft
    inc     a
    ret     z
    ld      bc,(#_g_clipRect+#0x06)
    call    ClipRight
    inc     a
    ret     z       ; hl = sy, de = ey, hl'= sx, de'= ex

    ld      a,e     ; a'= ey
    ex      af,af'
    ld      a,l     ; a = sy
    exx
    ld      h,l     ; h = sx
    ld      l,a     ; l = sy
    ld      d,e     ; d = ex
    ex      af,af'
    ld      e,a     ; e = ey
    call    VDP_HLDE_LINE
    ret

;------------------------------------------------------------------------------
; Clip Left
; in BC left
; in,out HL sx
; in,out DE ex
; in,out HL' sy
; in,out DE' ey
; use BC
; out A -1=out, 0=intersect, 1=in 
;------------------------------------------------------------------------------
ClipLeft:
    push    bc
    push    hl
    push    de
    or      a
    sbc     hl,bc           ; hl = sx - left        
    ex      de,hl           ; de = sx - left, hl = ex
    jr      nc,ClipLeft_Left_SX

ClipLeft_SX_Left:
    or      a
    sbc     hl,bc           ; hl = ex - left
    jr      nc,ClipLeft_SX_Left_EX
ClipLeft_SX_EX_Left:
    ld      a,#-1           ; a = clip out
    pop     de              ; de = ex
    pop     hl              ; hl = sx
    pop     bc
    ret
ClipLeft_SX_Left_EX:
    ex      de,hl           ; hl = sx - left, de = ex - left
    exx
    push    hl              ; push sy
    push    de              ; push ey
    exx
    call    IntersectZero
    pop     de              ; de' = ey, hl' = new sy
    pop     af              ; af' = old sy
    exx     
    pop     de              ; de = ex
    pop     af              ; af = old sx
    pop     hl              ; hl = new sx (left)
    xor     a               ; a = intersect 
    ret

ClipLeft_Left_SX:
    or      a 
    sbc     hl,bc           ; hl = ex - left, de = sx - left
    jr      c,ClipLeft_EX_Left_SX
ClipLeft_Left_SX_EX:
    ld      a,#1            ; a = clip in
    pop     de              ; de = ex
    pop     hl              ; hl = sx
    pop     bc              ; bc = left
    ret
ClipLeft_EX_Left_SX:
    exx
    ex      de,hl           ; hl' = ey, de'= sy
    push    hl              ; push ey
    push    de              ; push sy
    exx
    call    IntersectZero
    pop     de              ; hl' = new ey, de' = sy
    pop     af              ; af = old ey
    exx
    pop     af              ; af = old ex
    pop     de              ; de = sx
    pop     hl              ; hl = new ex (left)
    xor     a               ; a = intersect
    ret

;------------------------------------------------------------------------------
; Clip Right
; in BC right
; in,out HL sx
; in,out DE ex
; in,out HL' sy
; in,out DE' ey
; use BC
; out A -1=out, 0=intersect, 1=in 
;------------------------------------------------------------------------------
ClipRight:
    push    bc
    push    hl
    push    de
    or      a
    sbc     hl,bc           ; hl = sx - right       
    ex      de,hl           ; de = sx - right, hl = ex
    jr      nc,ClipRight_Right_SX

ClipRight_SX_Right:
    or      a
    sbc     hl,bc           ; hl = ex - right
    jr      nc,ClipRight_SX_Right_EX
ClipRight_SX_EX_Right:
    ld      a,#1            ; a = clip in
    pop     de              ; de = ex
    pop     hl              ; hl = sx
    pop     bc
    ret
ClipRight_SX_Right_EX:
    ex      de,hl           ; hl = sx - right, de = ex - right
    exx
    push    hl              ; push sy
    push    de              ; push ey
    exx
    call    IntersectZero
    ex      de,hl           ; de' = new ey
    pop     af              ; af' = old ey
    pop     hl              ; hl' = sy
    exx     
    pop     af              ; af = old ex
    pop     hl              ; hl = sx
    pop     de              ; de = new ex (right)
    xor     a               ; a = intersect 
    ret

ClipRight_Right_SX:
    or      a 
    sbc     hl,bc           ; hl = ex - right, de = sx - right
    jr      c,ClipRight_EX_Right_SX
ClipRight_Right_SX_EX:
    ld      a,#-1           ; a = clip out
    pop     de              ; de = ex
    pop     hl              ; hl = sx
    pop     bc              ; bc = right
    ret
ClipRight_EX_Right_SX:
    exx
    ex      de,hl           ; hl' = ey, de'= sy
    push    hl              ; push ey
    push    de              ; push sy
    exx
    call    IntersectZero
    pop     af              ; hl' = new sy, af' = old sy
    pop     de              ; de = ey
    exx
    pop     de              ; de = ex
    pop     af              ; af = old sx
    pop     hl              ; hl = new sx (right)
    xor     a               ; a = intersect
    ret

;------------------------------------------------------------------------------
; get intersection to zero
; in DE innerX (plus)
; in HL outerX (minus)
; in DE' innerY
; in HL' outerY
; use a bc
; out HL intersectionY
;------------------------------------------------------------------------------
	.area	_CODE

IntersectZero:
IntersectZero_Loop:
IntersectZero_Test_Outer:
    ld      a,l
    or      h
    jr      nz,IntersectZero_Test_Inner
    exx             ; hl = outerY
    ret

IntersectZero_Test_Inner:
    ld      a,e
    or      d
    jr      nz,IntersectZero_Make_Half
    exx
    ex      de,hl   ; hl = innerY
    ret

IntersectZero_Make_Half:
    ld      b,h     ; bc = outer
    ld      c,l
    add     hl,de   ; hl = (hl+de)/2 signed
    sra     h
    rr      l

    bit     7,h
    jr      z,IntersectZero_Zero_Half_Inner

IntersectZero_Half_Zero_Inner:    ; half----zero----inner
    exx             ; compute Y value
    add     hl,de
    sra     h
    rr      l
    exx
    jr      IntersectZero_Loop   ; retry hl=half, de=inner

IntersectZero_Zero_Half_Inner:   ; outer----zero----half----inner
    exx                 ; compute Y value
    ex      de,hl       ; de = outerY, hl = innerY
    add     hl,de       ; hl = (hl+de)/2
    sra     h
    rr      l
    ex      de,hl       ; de = halfY / hl = outerY
    exx
    ex      de,hl       ; de = half
    ld      h,b         ; hl = outer
    ld      l,c
    jr      IntersectZero_Loop   ; retry hl = outer, de = half