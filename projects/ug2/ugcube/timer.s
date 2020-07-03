;------------------------------------------------------------------------------
; timer_entry
;------------------------------------------------------------------------------
            .area   _DATA
            .globl  _g_pTimerCallback
H_TIMI_Save:
    .ds     5

_g_pTimerCallback:
    .ds     2

            .area   _CODE
            .globl  _SetTimer

H_TIMI = 0xFD9F

H_TIMI_HookSource:
    rst     0x30            ; CALLF
    .db     0               ; slot
    .dw     TimerEntry      ; caller address
    ret

RSLREG  = 0x0138
EXPTBL  = 0xFCC1		; slot is expanded or not

MakeSlotSelecter_Page1:
    call    RSLREG
    rrca
    rrca
    and     #0x03
    ld      c,a
    ld      b,#0
	ld      hl,#EXPTBL	; see if this slot is expanded or not
	add     hl,bc
    ld      c,a 		; save primary slot #
	ld      a,(hl)		; get the slot is expanded or not
	and     #0x80
	or      c           ; set MSB if so
    jp      p,MakeSlotSelecter_Page1_End
	ld      c,a         ; save it to [C]
	inc     hl  		; Point to SLTTBL entry
	inc     hl
	inc     hl
	inc     hl
	ld      a,(hl)		; Get what is currently output to expansion slot register
    and     #0x0c       ; mask page 1
	or      c           ; +>is expanded
                        ; |   ++>ext slot
                        ; |   ||++>primary slot
                        ; 00000000
MakeSlotSelecter_Page1_End:
    ret

_SetTimer:
    ld      hl,#H_TIMI
    ld      de,#H_TIMI_Save
    ld      bc,#0x0005
    ldir
    
    ld      hl,#0x0002
    add     hl,sp
    ld      e,(hl)
    inc     hl
    ld      d,(hl)
    ld      (#_g_pTimerCallback),de

    di
    ld      hl,#H_TIMI_HookSource
    ld      de,#H_TIMI
    ld      bc,#0x0005
    ldir

    call    MakeSlotSelecter_Page1
    ld      (#H_TIMI+#1),a
    ei
    ret

CallHL:
    jp      (hl)
            
TimerEntry:
    di
    ld      hl,(_g_pTimerCallback)
    call    CallHL
    call    H_TIMI_Save
    ret