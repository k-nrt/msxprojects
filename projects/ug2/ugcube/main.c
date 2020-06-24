#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "pers.h"
#include "test.h"

#pragma codeseg CODE2

u8 g_u8Timer = 0;
void OnTimer()
{
	g_u8Timer++;
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
	PersInit();
}

void main(void)
{
	Init();

	Test("UGCUBE");

	for(;;)
	{}
}