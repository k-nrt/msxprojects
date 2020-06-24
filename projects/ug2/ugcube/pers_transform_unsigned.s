;------------------------------------------------------------------------------
; PersTransformUnsigned
; fetch transform table or calc transform position if 3D position is out of table
; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; out DE screen position [unsigned]
; out CY 1 = clip, 0 = no clip
; use A,B,C,A'
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  MulCoreUnsigned_BxDE_AHL
                .globl  PersTransformUnsigned

PersTransformUnsigned:
    ld      a,l     ; hl |= z
    or      d
    ld      l,a      
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,PersTransformUnsigned_Clip
    ld      h,#0xC0
    ld      l,d
    sla     l       ; HL = ptr + z*2
    ld      a,(hl)
    inc     hl
    ld      h,(hl)
    ld      l,a     ; HL = 2d position top address
    ld      d,#0x00
    add     hl,de   ; HL = 2d x address
    ld      e,(hl)  ; de = screen position
    xor     a       ; CY = 0
    ld      d,a
    ret

PersTransformUnsigned_Clip:
    ld      b,e         ; b = x
    ld      h,#0xDE
    ld      l,d         ; d = z
    sla     l
    ld      e,(hl)      ; de = screenZ/z
    inc     l
    ld      d,(hl)
    call    MulCoreUnsigned_BxDE_AHL
    ld      e,h         ; ahl = screenX[16:8]
    ld      d,a         ; de = screenX
    scf                 ; CY = 1
    ret
