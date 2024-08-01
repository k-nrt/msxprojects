;------------------------------------------------------------------------------
; System work area
;------------------------------------------------------------------------------
EXPTBL	= 0xFCC1	; Main ROM slot / ext slot [4].
SLTTBL	= 0xFCC5	; ext slot registers [4]
SLTATR	= 0xFCC9	; slot attributes for page [4*16 = 64]
SLTWRK	= 0xFD09	; slot work area for page [2*4*16 = 128]
EXBRSA	= 0xFAF8	; SUB ROM slot

DPPAGE	= 0xFAF5	; Display page.
ACPAGE	= 0xFAF6	; Active page.

FORCLR	= 0xF3E9	; Foreground Color.
BAKCLR	= 0xF3EA	; Background Color.
BDRCLR	= 0xF3EB	; Border Color.

LOGOPR	= 0xFB02	; Logical Operation.

GRPACX	= 0xFCB7	; Graphic Accumlator X (16bit)
GRPACY	= 0xFCB9	; Graphic Accumlator Y (16bit)

RG0SAV	= 0xF3DF
RG1SAV	= 0xF3E0
RG2SAV	= 0xF3E1
RG3SAV	= 0xF3E2
RG4SAV	= 0xF3E3
RG5SAV	= 0xF3E4
RG6SAV	= 0xF3E5
RG7SAV	= 0xF3E6

STATFL	= 0xF3E7

RG8SAV	= 0xFFE7
RG9SAV	= 0xFFE8
RG10SAV	= 0xFFE9
RG11SAV	= 0xFFEA
RG12SAV	= 0xFFEB
RG13SAV	= 0xFFEC
RG14SAV	= 0xFFED
RG15SAV	= 0xFFEE
RG16SAV	= 0xFFEF
RG17SAV	= 0xFFF0
RG18SAV	= 0xFFF1
RG19SAV	= 0xFFF2
RG20SAV	= 0xFFF3
RG21SAV	= 0xFFF4
RG22SAV	= 0xFFF5
RG23SAV	= 0xFFF6

RG25SAV	= 0xFFFA
RG26SAV	= 0xFFFB
RG27SAV	= 0xFFFC
