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
    .globl  ClipLeft
    .globl  ClipRight
    .globl  IntersectZero

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
    ld      b,h     ; bc = outer
    ld      c,l
    add     hl,de   ; hl = (hl+de)/2 signed
    sra     h
    rr      l

    ld      a,h     ; if hl == 0 then a = 0
    or      l
    bit     7,h
    jr      z,IntersectZero_Zero_Half_Inner

IntersectZero_Half_Zero_Inner:    ; half----zero----inner
    exx             ; compute Y value
    add     hl,de
    sra     h
    rr      l
    
    or      a
    ret     z       ; if a == 0 then hl = scissorY

    exx
    jr      IntersectZero_Loop   ; retry hl=half, de=inner

IntersectZero_Zero_Half_Inner:   ; outer----zero----half----inner
    exx                 ; compute Y value
    ex      de,hl       ; de = outerY, hl = innerY
    add     hl,de       ; hl = (hl+de)/2
    sra     h
    rr      l

    or      a           ; if a == 0 then hl = scissorY
    ret     z

    ex      de,hl       ; de = halfY / hl = outerY
    exx
    ex      de,hl       ; de = half
    ld      h,b         ; hl = outer
    ld      l,c
    jr      IntersectZero_Loop   ; retry hl = outer, de = half
