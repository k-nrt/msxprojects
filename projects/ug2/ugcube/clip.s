;------------------------------------------------------------------------------
; clip
;------------------------------------------------------------------------------
    .area   _INITIALIZED
    .globl  _g_clipRect
    .globl  _g_clipLineInOut

_g_clipRect:
    .ds     2   ; left
    .ds     2   ; right
    .ds     2   ; top
    .ds     2   ; bottom

_g_clipLineInOut:
    .ds     2   ; sx
    .ds     2   ; sy
    .ds     2   ; ex
    .ds     2   ; ey

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
    jp      m,ClipLeft_SX_Left
    jp      ClipLeft_Left_SX

ClipLeft_ClipIn:
ClipLeft_Left_SX_EX:
    ld      a,#01           ; clip in
    pop     de
    pop     hl
    pop     bc
    ret

ClipLeft_ClipOut:
ClipLeft_SX_EX_Left:
    ld      a,#-1           ; clip out
    pop     de
    pop     hl
    pop     bc
    ret

ClipLeft_Left_SX:
    or      a 
    sbc     hl,bc           ; hl = ex - left, de = sx - left
    jp      m,ClipLeft_EX_Left_SX   ; intersect
    jp      ClipLeft_Left_SX_EX     ; clip in 

ClipLeft_SX_Left:
    or      a
    sbc     hl,bc           ; hl = ex - left
    jp      m,ClipLeft_SX_EX_Left   ; clip out
    jp      nz,ClipLeft_SX_Left_EX  ; intersect
    ld      a,#1                    ; ex is on left
    pop     de
    pop     hl
    pop     bc
    exx
    ld      l,e                     ; sy = ey
    ld      h,d
    exx
    ld      l,e                     ; sx = ex
    ld      h,d
    ret

ClipLeft_Intersect:
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
    jp      m,ClipRight_SX_Right
    jp      ClipRight_Right_SX 

ClipRight_ClipOut:
ClipRight_Right_SX_EX:
    ld      a,#-1           ; clip out
    pop     de
    pop     hl
    pop     bc
    ret

ClipRight_ClipIn:
ClipRight_SX_EX_Right:
    ld      a,#1            ; clip in
    pop     de
    pop     hl
    pop     bc
    ret

ClipRight_SX_Right:
                                        ; bc = right
                                        ; de = sx - right
                                        ; hl = ex
    or      a
    sbc     hl,bc                       ; hl = ex - right
    jp      m,ClipRight_SX_EX_Right     ; clip in
    jp      z,ClipRight_SX_EX_Right     ; clip in
    jp      ClipRight_SX_Right_EX       ; intersect

ClipRight_Right_SX:
                                        ; bc = right
                                        ; de = sx - right
                                        ; hl = ex
    or      a 
    sbc     hl,bc                       ; hl = ex - right
    jp      m,ClipRight_EX_Right_SX     ; intersect
    jp      nz,ClipRight_Right_SX_EX    ; clip out
    ld      a,#1                        ; ex is on left
    pop     de
    pop     hl
    pop     bc
    exx
    ld      l,e                         ; sy = ey
    ld      h,d
    exx
    ld      l,e                         ; sx = ex
    ld      h,d
    ret

ClipRight_Intersect:
ClipRight_SX_Right_EX:
                            ; de = sx - right
                            ; hl = ex - right
                            ; de' = ey
                            ; hl' = sy
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

ClipRight_EX_Right_SX:
                            ; de = sx - right
                            ; hl = ex - right
                            ; de' = ey
                            ; hl' = sy
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
    jp      nz,IntersectZero_Test_Inner
    exx             ; hl = outerY
    ret

IntersectZero_Test_Inner:
    ld      a,e
    or      d
    jp      nz,IntersectZero_Make_Half
    exx
    ex      de,hl   ; hl = innerY
    ret

IntersectZero_Make_Half:
    ld      b,h     ; bc = outer
    ld      c,l
    add     hl,de   ; hl = (hl+de)/2 signed
    sra     h
    jp      z,IntersectZero_Zero_Half_Inner
    rr      l

IntersectZero_Half_Zero_Inner:    ; half----zero----inner
    exx             ; compute Y value
    add     hl,de
    sra     h
    rr      l
    exx
    jp      IntersectZero_Loop   ; retry hl=half, de=inner

IntersectZero_Zero_Half_Inner:   ; outer----zero----half----inner
    rr      l
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
    jp      IntersectZero_Loop   ; retry hl = outer, de = half

;------------------------------------------------------------------------------
; s8 ClipLineS8x3(s8 s8Near);
;------------------------------------------------------------------------------
                .area   _INITIALIZED
                .globl  _g_clipLineS8x3
                
_g_clipLineS8x3:
	.ds	1
	.ds	1
	.ds	1

	.ds	1
	.ds	1
	.ds	1

                .area   _CODE
                .globl  _ClipLineS8x3
                .globl  ClipLineS8x3

_ClipLineS8x3:
    ld      hl,#0x0002
    add     hl,sp
    ld      b,(hl)
    exx
    ld      hl,#_g_clipLineS8x3
    ld      d,(hl)          ; sx
    inc     hl
    ld      e,(hl)          ; sy
    inc     hl
    ld      a,(hl)          ; a' = sz
    inc     hl
    ld      b,(hl)          ; b = ex
    inc     hl
    ld      c,(hl)          ; c = ey
    inc     hl
    ex      af,af'
    ld      a,(hl)          ; a = ez
    ld      h,b             ; h = b = ex
    ld      l,c             ; l = c = ey
    exx
    ld      e,a             ; e = ez
    ex      af,af'
    ld      d,a             ; d = sz
     
    ; in B near
    ; inout D SZ
    ; inout E EZ
    ; inout D' SX
    ; inout E' SY
    ; inout H' EX
    ; inout L' EY
    ; out A -1 out 0 intersect 1 = in
    call    ClipLineS8x3

    ld      b,a
    exx
    ld      a,h
    ex      af,af'
    ld      a,l
    exx

    ld      hl,#_g_clipLineS8x3+#5
    ld      (hl),e          ; ez
    dec     hl
    ld      (hl),a          ; ey
    dec     hl
    ex      af,af'          
    ld      (hl),a          ; ex
    dec     hl
    ld      (hl),d          ; sz
    dec     hl
    
    exx
    ld      a,d
    ex      af,af'
    ld      a,e
    exx
    ld      (hl),a          ; sy
    dec     hl
    ex      af,af'
    ld      (hl),a          ; sx

    ld      l,b
    ret
