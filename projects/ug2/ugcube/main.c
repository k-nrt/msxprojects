#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "sincos.h"

#pragma codeseg CODE2

u8 g_u8Timer = 0;
void OnTimer()
{
	g_u8Timer++;
}

void Init(void)
{
	msx2BiosChangeModePalette(5);
	msxBiosChangeColor(6, 15, 4, 7);

	g_u8Timer = 0;
	msxTimerSetCallBack(OnTimer);
}

void Test_WaitForTrigger(const char *pszTitle)
{
	u8 u8Trig = msxBiosGetTrigger(0);
	u8 u8TrigPrev = u8Trig;

	VDPSetForegroundColor(0xff);
	VDPSetBackgroundColor(0x11);
	VDPPrint(0,0,pszTitle);
	
	for(;;)
	{
		u8 u8Timer = g_u8Timer;
		u8Trig = msxBiosGetTrigger(0);

		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			return;
		}

		u8TrigPrev = u8Trig;

		VDPPrintU8X(240, 0, g_u8Timer);
		while(u8Timer == g_u8Timer);
	}
}

void Test_SinCosLp(const char *pszTitle)
{
	u8 i;

	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);

	VDPSetForegroundColor(7);
	for (i = 0; i < 255; i++)
	{
		SinCosLp sincos0 = SinCos_GetSinCosLp(i);
		SinCosLp sincos1 = SinCos_GetSinCosLp(i+1);
		u8 y0 = sincos0 + 64;
		u8 y1 = sincos1 + 64;
		VDPWaitLine(i,y0,i+1,y1);
	}

	VDPSetForegroundColor(8);
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

void Test(const char *pszTitle)
{
	struct Item
	{
		const char *m_pszName;
 		void (*m_pFunc)(const char*);
	};

	static const struct Item items[] =
	{
		{"SinCosLp", Test_SinCosLp},
		{"SinLp CosLp", Test_SinLp_CosLp},
		{"SinHp CosHp", Test_SinHp},
		{NULL, NULL}
	};

	u8 nbItems = (sizeof(items)/sizeof(struct Item)) - 1;

	{
		u8 redraw = 1;
		u8 pos = 0;
		u8 posPrev = 1;
		u8 u8StickPrev = 0;
		u8 u8TrigPrev = 0;
		for(;;)
		{
			u8 u8Timer = g_u8Timer;
			u8 u8Stick = msxBiosGetStick(0);
			u8 u8Trig = msxBiosGetTrigger(0);

			if (u8StickPrev == 0)
			{
				if (u8Stick == 1 && 0 < pos)
				{
					pos--;
				}
				else if (u8Stick == 5  && pos < (nbItems-1))
				{
					pos++;
				}
			}

			if (u8TrigPrev == 0 && u8Trig != 0)
			{
				(*(items[pos].m_pFunc))(items[pos].m_pszName);
				redraw = 1; 
			}

			if (redraw)
			{
				u8 y = 8;
				const struct Item *pItem = items;

				VDPSetForegroundColor(0x11);   
				VDPFill(0,0,256,212);
				VDPSetForegroundColor(0xff);
				VDPSetBackgroundColor(0x11);
				VDPPrint(0, 0, pszTitle);
				while (pItem->m_pszName != NULL)
				{
					VDPPrint(16, y, pItem->m_pszName);
					y += 8;
					pItem++;
				}
			}

			if (pos != posPrev || redraw)
			{
				VDPPrint(8,8 + (posPrev << 3), " ");
				VDPPrint(8,8 + (pos << 3),     ">");
			}

			VDPPrintU8X(240, 0, g_u8Timer);
		
			u8TrigPrev = u8Trig;
			u8StickPrev = u8Stick;
			posPrev = pos;
			while(u8Timer == g_u8Timer);

			if (redraw)
			{
				redraw = 0;
			}
		}
	}
}

void main(void)
{
	Init();

	Test("UGCUBE");

	for(;;)
	{}
}