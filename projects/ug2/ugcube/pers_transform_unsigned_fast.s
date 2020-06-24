;------------------------------------------------------------------------------
; PersTransformUnsignedFast
; fetch transform table only
; in HL 0xC100 = left, 0xC180 = right, 0xC200 = top, 0xC280 = bottom
; in D 3d clipped z [near --- 127]
; in E 3d position [unsigned]
; keep D 3d position z
; out E screen [unsigned]
; out CY 1 = clip, 0 = no clip
; use A
;------------------------------------------------------------------------------
                .area   _CODE
                .globl  PersTransformUnsignedFast
                
PersTransformUnsignedFast:
    ld      a,l     ; hl |= z
    or      d
    ld      l,a      
    ld      a,(hl)  ; a = max
    cp      e       ; max - x
    jp      c,PersTransformUnsignedFast_Clip
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

PersTransformUnsignedFast_Clip:
    scf             ; CY = 1
    ret
