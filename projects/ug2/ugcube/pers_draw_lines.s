;------------------------------------------------------------------------------
; pers draw lines
;------------------------------------------------------------------------------
            .include    "pers_defs.s"

            .area   _CODE
            .globl  PersDrawLines
            .globl  _PersDrawLines

            .globl  VDP_HLDE_LINE

;------------------------------------------------------------------------------
; PersDrawLines
; HL line address
; B line count
;------------------------------------------------------------------------------
            .area   _CODE
PersDrawLines:
PersDrawLines_LoopStart:
    ld      e,(hl)          ; de = screen pos address
    inc     hl
    ld      d,(hl)
    inc     hl
    ex      de,hl           ; de = line address, hl = screen pos address

    ld      a,(hl)          ; A = clip bits
    or      a
    jr      nz,PersDrawLines_LoopNextX

    inc     hl
    ld      a,(hl)          ; a = x
    inc     hl
    ex      af,af'
    ld      a,(hl)          ; a = y
    exx
    ld      e,a             ; E' = screenY
    ex      af,af'
    ld      d,a             ; D' = screenX
    exx

    ex      de,hl           ; hl = line address
    ld      e,(hl)          ; de = screen pos address
    inc     hl
    ld      d,(hl)
    inc     hl
    ex      de,hl           ; de = line address, hl = screen pos address

    ld      a,(hl)          ; A = clip bits
    or      a
    jr      nz,PersDrawLines_LoopNext

    inc     hl
    ld      a,(hl)          ; a = x
    inc     hl
    ex      af,af'
    ld      a,(hl)
    exx
    ld      l,a             ; L' = screenY
    ex      af,af'
    ld      h,a             ; H' = screenX

    call    VDP_HLDE_LINE
    exx

PersDrawLines_LoopNext:
    ex      de,hl           ; hl = line address
    djnz    PersDrawLines_LoopStart
    ret

PersDrawLines_LoopNextX:
    ex      de,hl           ; hl = line address
    inc     hl              ; skip line end address
    inc     hl
    djnz    PersDrawLines_LoopStart
    ret


;------------------------------------------------------------------------------
; extern void PersDrawLines(const u16* pLines, u8 nbLines);
;------------------------------------------------------------------------------
            .area   _CODE
_PersDrawLines:
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    inc     hl
    ld      b,(hl)
    ex      de,hl
    call    PersDrawLines
    ret
