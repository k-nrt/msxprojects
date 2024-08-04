#include "bios_wrapper.h"
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "clip.h"
#include "test.h"

#pragma codeseg CODE2

void Test_Line(const char* pszTitle)
{
	u16 i;
	msxRandInit(45, 36722);

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);

	VDPSetForegroundColor(0x44);
	for (i=0; i<256; i += 16)
	{
		u8 x = i;
		VDPWaitLine(x,0,x,211);
	}
	for (i=0; i<212; i += 16)
	{
		u8 y = i;
		VDPWaitLine(0,y,255,y);
	}

	Clip_SetRect(64,255-64,64,191+16-64);
	for(i = 0; i < 200; i++)
	{
		s16 sx,sy,ex,ey;
		sx = 0xff & ((s16) msxRandGet16()>>8);
		sy = 0xff & ((s16) msxRandGet16()>>4);
		ex = 0xff & ((s16) msxRandGet16()>>6);
		ey = 0xff & ((s16) msxRandGet16()>>8);
		Clip_SetLine(sx, MIN(sy,211), ex, MIN(ey,211));
		VDPSetForegroundColor(0x44);
		Clip_VDPWaitLine();
		VDPSetForegroundColor(0xff);
		//ClipRect();
		//Clip_VDPWaitLine();
		ClipRect_VDPWaitLine();
	}
	VDPSetForegroundColor(0xff);
	//VDPWaitLine(0,0,64,64);
	Test_WaitForTrigger(pszTitle);
}

