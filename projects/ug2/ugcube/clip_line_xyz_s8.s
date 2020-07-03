;------------------------------------------------------------------------------
; clip_line_xyz_s8
;------------------------------------------------------------------------------
    .area   _DATA
    .globl  _g_clipRect
    .globl  _g_clipLineInOut

;------------------------------------------------------------------------------
; get intersection to zero
; in D innerZ (plus)
; in E outerZ (minus)
; in D' innerX
; in E' innerY
; in H' outerX
; in L' outerY
; use a
; out H intersectionX
; our L intersectionY
;------------------------------------------------------------------------------
                .area _CODE
IntersectZeroS8x3:
IntersectZeroS8x3_Loop:
IntersectZeroS8x3_Test_Outer:
    xor     a
    or      e           ; e == 0
    jp      nz,IntersectZeroS8x3_Test_Inner
    exx                 ; h = outerX, l = outerY
    ret

IntersectZeroS8x3_Test_Inner:
    xor     a
    or      d           ; d == 0
    jp      nz,IntersectZeroS8x3_Make_Half
    exx
    ex      de,hl       ; h = innerX, l = innerY
    ret

IntersectZeroS8x3_Make_Half:
    ld      a,d         ; a = (d+e)/2 signed
    add     a,e
    sra     a
    jp      p,IntersectZeroS8x3_Zero_Half_Inner

IntersectZeroS8x3_Half_Zero_Inner:    ; half----zero----inner
    ld      e,a         ; e = half
    exx                 ; compute X,Y value
    ld      a,d         ; outer = (outer + inner)/2 signed
    add     a,h
    sra     a
    ld      h,a

    ld      a,e
    add     a,l
    sra     a
    ld      l,a
    exx
    jp      IntersectZeroS8x3_Loop   ; retry h,l=half, d,e=inner

IntersectZeroS8x3_Zero_Half_Inner:   ; outer----zero----half----inner
    ld      d,a         ; d = half
    exx                 ; compute X,Y value
    ld      a,d         ; inner = (outer + inner)/2 signed
    add     a,h
    sra     a
    ld      d,a

    ld      a,e
    add     a,l
    sra     a
    ld      e,a
    exx
    jp      IntersectZeroS8x3_Loop   ; retry h,l = outer, d,e = half

;------------------------------------------------------------------------------
; ClipLineS8x3
; in B near
; inout D SZ
; inout E EZ
; inout D' SX
; inout E' SY
; inout H' EX
; inout L' EY
; out A -1 out 0 intersect 1 = in
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  ClipLineS8x3
ClipLineS8x3:
ClipLineS8x3_Test:
    ld      a,d             ; a = sz - near
    sub     b
    jp      m,ClipLineS8x3_SZ_Near

ClipLineS8x3_Near_SZ:
    ex      af,af'          ; push a = sz - near
    ld      a,e
    sub     b
    jp      m,ClipLineS8x3_EZ_Near_SZ

ClipLineS8x3_Near_SZ_EZ:
    ld      a,#01
    ret             ; no clip

ClipLineS8x3_SZ_Near:
    ex      af,af'
    ld      a,e
    sub     b
    jp      p,ClipLineS8x3_SZ_Near_EZ

ClipLineS8x3_SZ_EZ_Near:
    ld      a,#-1   ; clipped
    ret

ClipLineS8x3_EZ_Near_SZ:
    ld      e,b     ; d = sz, e = ez = near
    push    de
    ld      e,a     ; e = outer = ez - near   
    ex      af,af'
    ld      d,a     ; d = inner = sz - near
    exx
    push    de      ; push inner sxsy
    exx
    call    IntersectZeroS8x3
    pop     de      ; de' = sxsy, hl' = intersect exey
    exx
    pop     de      ; d = sz, e = near
    xor     a       ; a = intersect
    ret

ClipLineS8x3_SZ_Near_EZ:
    ld      d,b     ; d = sx = near, e = ez
    push    de
    ld      d,a     ; d = inner = ez - near
    ex      af,af'
    ld      e,a     ; e = outer = sz - near
    exx
    ex      de,hl   ; de = inner exey, hl = outer sxsy
    push    de      ; push inner exey
    exx
    call    IntersectZeroS8x3
    pop     de
    ex      de,hl   ; hl' = inner exey, de' = intersect sxsy
    exx
    pop     de      ; d = near, e = ez;
    xor     a       ; a = intersect
    ret