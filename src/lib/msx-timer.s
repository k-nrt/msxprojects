;------------------------------------------------------------------------------
; void msxTimerSetCallBack( void (pCallBack(void)) )
;------------------------------------------------------------------------------
		.area	_INITIALIZED
H_TIMI_Save:
	.ds	5

_g_pTimerCallback:
	.ds	2

		.area   _CODE
		.globl	_msxTimerSetCallBack

H_TIMI = 0xFD9F			; timer hook
RSLREG = 0x0138			; read primary slot register 
EXPTBL = 0xFCC1			; slot is expanded or not x4 
SLTTBL = 0xFCC5			; expansion slot x4 (EXPTBL+4)

GetExpansionSlotRegister:
				; input
				;              ++---- primary slot
				; in a = ******xx
	and	#0x03
	ld	c,a 		; c = save primary slot

	add	a,#<EXPTBL	; HL = EXPTBL + A
	ld	l,a
	ld	h,#>EXPTBL

	ld	a,(hl)		; a = slot is expanded or not
	and	#0x80
				;     ++---------- expanded or not
				;     |     ++---- primary slot
	or	c		; a = x00000xx
	jp	p,GetExpansionSlotRegister_End

				; slot is expanded
	ld	c,a		; c = save primary slot and expanded bit
	inc	hl  		; Point to SLTTBL entry
	inc	hl
	inc	hl
	inc	hl
	ld	a,(hl)		; Get what is currently output to expansion slot register

GetExpansionSlotRegister_End:
				; output
				;     +----------- expand or not
				;     |     ++---- primary slot
				; c = x00000xx
				;     ++---------- page3 exp slot
				;     ||++-------- page2 exp slot
				;     ||||++------ page1 exp slot
				;     ||||||++---- page0 exp slot
				; a = xxxxxxxx
	ret

_msxTimerSetCallBack:
	ld	(#_g_pTimerCallback),hl

	ld	hl,#H_TIMI	; save previous H_TIMI
	ld	de,#H_TIMI_Save
	ld	bc,#0x0005
	ldir

	ld	c,a		; c = save callback address high

	call	RSLREG          ; read primary slot register
				;     ++---------- page3
				;     ||++-------- page2
				;     ||||++------ page1
				;     ||||||++---- page0
				; a = xxxxxxxx

	ld	b,a		; b = save primary slot
	ld	a,#>TimerEntry	; a = TimerEntry address high

	rlca			; cy = callback address bit7
	jp	c,_msxTimerSetCallBack_Page23

_msxTimerSetCallBack_Page01:
	rlca			; cy = callback address bit6
	jp	c,_msxTimerSetCallBack_Page1

_msxTimerSetCallBack_Page0:
	ld	a,b		; a = ******xx ---- page0 slot
	call	GetExpansionSlotRegister
	rlca
	rlca			; a = ****xx** ---- page0 exp slot
	jp	_msxTimerSetCallBack_MakeHook

_msxTimerSetCallBack_Page1:
	ld	a,b
	rrca
	rrca			; a = ******xx ---- page1 slot
	call	GetExpansionSlotRegister
				; a = ****xx** ---- page1 exp slot
	jp	_msxTimerSetCallBack_MakeHook

_msxTimerSetCallBack_Page23:
	rlca			; cy = callback address bit 6
	jp	c,_msxTimerSetCallBack_Page3

_msxTimerSetCallBack_Page2:
	ld	a,b
	rrca
	rrca
	rrca
	rrca			; a = ******xx ---- page2 slot
	call	GetExpansionSlotRegister
	rrca
	rrca			; a = ****xx** ---- page2 exp slot
	jp	_msxTimerSetCallBack_MakeHook

_msxTimerSetCallBack_Page3:
	ld	a,b
	rlca
	rlca			; a = ******xx ---- page3 slot
	call	GetExpansionSlotRegister
	rrca
	rrca
	rrca
	rrca			; a = ****xx** ---- page3 exp slot

_msxTimerSetCallBack_MakeHook:
	and	#0x0c       	; mask exp slot
	or	c		;     +----------- is expanded
				;     |   ++------ exp slot
				;     |   ||++---- primary slot
				; a = x000xxxx

	di
	ld	hl,#H_TIMI_HookSource
	ld	de,#H_TIMI
	ld	bc,#0x0005
	ldir

	ld	(#H_TIMI+#1),a
	ei
	ret

H_TIMI_HookSource:
	rst	0x30		; CALLF
	.db	0		; slot
	.dw	TimerEntry	; caller address
	ret

CallHL:
	jp	(hl)

TimerEntry:
	di
	ld	hl,(_g_pTimerCallback)
	call	CallHL
	call	H_TIMI_Save
	ret
