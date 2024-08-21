#include "bios_wrapper.h"
#include <msx-timer.h>
#include <msx-rand.h>

#ifdef SYSTEM_MSXDOS
#include <msx-dos.h>
#endif

#include "vdp_command.h"
#include "pers.h"
#include "test.h"

#pragma codeseg CODE2

u8 g_u8Timer = 0;
void OnTimer(void)
{
	g_u8Timer++;
}

void OnProgress(u16 address)
{
	VDPPrintU16X(0, 8, address);
}

void Init(void)
{
	vdpSetScreenModePalette(5);
	vdpSetScreenColor(5, 15, 1, 7);

	g_u8Timer = 0;
	msxTimerSetCallBack(OnTimer);

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);
	VDPSetForegroundColor(0xFF);
	VDPPrint(0,0,"CREATE TRANSFROM TABLE...");
	PersInit(32, 160, 128, 104, 0, 255, 104 - 64, 104 + 63, OnProgress);

	VDPPrint(0,16,"DONE");
}

#if defined(SYSTEM_ROM32K)
void main(void)
{
	Init();

	Test("UGCUBE");

	for(;;)
	{}
}
#elif defined(SYSTEM_MSXDOS)
SDCC_FIXED_ADDRESS(0x0006) u16 g_tpa;
SDCC_FIXED_ADDRESS(0xFC4A) u16 g_himem;
void main(const char* args)
{
	UNUSED(args);
	msxDosPrint("TPA:");
	msxDosPrintHexU16(g_tpa);
	msxDosPrint("\r\n");

	msxDosPrint("HIMEM:");
	msxDosPrintHexU16(g_himem);
	msxDosPrint("\r\n");

	VDPInit();
	Init();
	VDPPrint(0,24,"DONE1");

	Test("UGCUBE");

	msxTimerSetCallBack(NULL);
	VDPPrint(0,32,"DONE2");


	vdpSetActivePage(0);
	vdpSetDisplayPage(0);

	vdpSetScreenMode(0);
	msxClearKeyBuffer();
}
#endif