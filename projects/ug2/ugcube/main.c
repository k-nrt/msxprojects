#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

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
	msx2BiosChangeModePalette(5);
	msxBiosChangeColor(6, 15, 1, 7);

	g_u8Timer = 0;
	msxTimerSetCallBack(OnTimer);

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);
	VDPSetForegroundColor(0xFF);
	VDPPrint(0,0,"CREATE TRANSFROM TABLE...");
	PersInit(32, 160, 128, 104, 0, 255, 104 - 64, 104 + 63, OnProgress);
}

void main(void)
{
	Init();

	Test("UGCUBE");

	for(;;)
	{}
}