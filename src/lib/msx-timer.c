//-----------------------------------------------------------------------------
//. É^ÉCÉ}äÑÇËçûÇ›.
//-----------------------------------------------------------------------------
#include "msx-timer.h"

static u8 s_u8HTIMIBackUp[5+3];

void msxTimerSetCallBack( void (pCallBack(void)) )
{
	u8 i;
	u8 *pH_TIMI = (u8*) 0xFD9F;
	u8 *pS = pH_TIMI;
	u8 *pD = &s_u8HTIMIBackUp[3];
	
	for(i=0;i<5;i++,pS++,pD++)
	{
		*pD = *pS;
	}

	s_u8HTIMIBackUp[0] = 0xcd;
	*((u16*)&(s_u8HTIMIBackUp[1])) = (u16) pCallBack;

	__asm
		di
	__endasm;
	
	*pH_TIMI = 0xc3; //. jp.
	*((u16*) (pH_TIMI+1)) = (u16) s_u8HTIMIBackUp;

	__asm
		ei
	__endasm;
}
