#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "sincos.h"
#include "test.h"

#pragma codeseg CODE2

void Test_SinCosLp(const char *pszTitle)
{
	u8 i;

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);

	//. cos.
	VDPSetForegroundColor(8);
	for (i = 0; i < 255; i++)
	{
		SinCosLp sincos0 = SinCos_GetSinCosLp(i);
		SinCosLp sincos1 = SinCos_GetSinCosLp(i+1);
		u8 y0 = sincos0 + 64;
		u8 y1 = sincos1 + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	//. sin.
	VDPSetForegroundColor(7);
	for (i = 0; i < 255; i++)
	{
		SinCosLp sincos0 = SinCos_GetSinCosLp(i);
		SinCosLp sincos1 = SinCos_GetSinCosLp(i+1);
		u8 y0 = (sincos0 >> 8) + 64;
		u8 y1 = (sincos1 >> 8) + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	Test_WaitForTrigger(pszTitle); 
}

void Test_SinLp_CosLp(const char *pszTitle)
{
	u8 i;

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);

	VDPSetForegroundColor(7);
	for (i = 0; i < 255; i++)
	{
		fp2_6 sin0 = SinCos_GetSinLp(i);
		fp2_6 sin1 = SinCos_GetSinLp(i+1);
		u8 y0 = sin0 + 64;
		u8 y1 = sin1 + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	VDPSetForegroundColor(8);
	for (i = 0; i < 255; i++)
	{
		fp2_6 cos0 = SinCos_GetCosLp(i);
		fp2_6 cos1 = SinCos_GetCosLp(i+1);
		u8 y0 = cos0 + 64;
		u8 y1 = cos1 + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	Test_WaitForTrigger(pszTitle); 
}

void Test_SinHp(const char *pszTitle)
{
	u8 i;

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);

	VDPSetForegroundColor(7);
	for (i = 0; i < 255; i++)
	{
		fp2_14 sin0 = SinCos_GetSinHp(i);
		fp2_14 sin1 = SinCos_GetSinHp(i+1);
		u8 y0 = (sin0 >> 8) + 64;
		u8 y1 = (sin1 >> 8) + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	VDPSetForegroundColor(8);
	for (i = 0; i < 255; i++)
	{
		fp2_14 cos0 = SinCos_GetCosHp(i);
		fp2_14 cos1 = SinCos_GetCosHp(i+1);
		u8 y0 = (cos0 >> 8) + 64;
		u8 y1 = (cos1 >> 8) + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	Test_WaitForTrigger(pszTitle); 
}

