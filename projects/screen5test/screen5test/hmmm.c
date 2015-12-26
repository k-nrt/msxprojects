#include "hmmm.h"

void HMMM(u8 sx, u16 sy, u8 w, u8 h, u8 dx, u16 dy)
{
	sx;
	sy;
	w;
	h;
	dx;
	dy;
	//. ‚±‚±‚É
	//. push ix
	//. add ix,sp
	//. ‚ª“ü‚é.
	__asm
		RDVDP = 0x0006
		WRVDP = 0x0007

		push	ix

		ld		ix,#0
		add		ix,sp

		ld		a, (WRVDP)
		ld		c, a
		inc		c			//. c: = PORT's address
		ld      a, #32

		di
		out(c), a
		ld      a, #0x80 + #17
		out(c), a			// R#17 : = 32

		inc		c
		inc		c			// C: = PORT#3's address
		xor		a

		ld		l, 4(ix)
		out		(c), l		// SX
		out		(c), a

		ld		l, 5(ix)
		out		(c), l		// SY
		ld		h, 6(ix)
		out		(c), h

		ld		l, 9(ix)
		out		(c), l		// DX
		out		(c), a

		ld		l, 10(ix)
		out		(c), l		// DY
		ld		h, 11(ix)
		out		(c), h

		ld		l, 7(ix)
		out		(c), l		// NX
		out		(c), a

		ld		l, 8(ix)
		out		(c), l		// NX
		out		(c), a

		out		(c), a // dummy
		out		(c), a // MX, DI

		ld      a, #0b11010000 //. HMMM command
		out(C), a

		ei

		pop ix	
	__endasm;
}

void WaitCommand()
{
	__asm
		ld		a, (WRVDP)
		inc		a
		ld		d, a

		ld		a, (RDVDP)
		inc		a
		ld		e, a

		ld      h, #2
		ld		l, #0x8f
		ld		b, #0

	WAIT.VDP:
		di

		// Read S2
		// R15 = 2
		ld		c,d
		out		(c), h
		out		(c), l

		ld		c,e
		in      a, (c)

		// Crear R15
		// R15 = 0
		ld		c,d
		out		(c), b
		out		(c), l

		ei

		and     #1
		jr      nz, WAIT.VDP
		
	__endasm;
}
