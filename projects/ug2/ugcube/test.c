#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "test.h"

#pragma codeseg CODE2

extern u8 g_u8Timer; //. main.c

void Test_DrawTitle(const char *pszTitle)
{
	VDPSetForegroundColor(0xff);
	VDPSetBackgroundColor(0x11);
	VDPPrint(0,0,pszTitle);
}

void Test_DrawTimerAndWait()
{
	volatile u8 u8Timer = g_u8Timer;
	VDPSetForegroundColor(0xff);
	VDPSetBackgroundColor(0x11);
	VDPWait();
	VDPPrintU8X(240, 0, g_u8Timer);
	while(u8Timer == g_u8Timer);  
}


void Test_WaitForTrigger(const char *pszTitle)
{
	u8 u8Trig = msxBiosGetTrigger(0);
	u8 u8TrigPrev = u8Trig;

	Test_DrawTitle(pszTitle);
	
	for(;;)
	{
		u8Trig = msxBiosGetTrigger(0);

		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			return;
		}

		u8TrigPrev = u8Trig;

		Test_DrawTimerAndWait();
	}
}

extern void Mtk_Main(const char *);

void Test(const char *pszTitle)
{
	struct Item
	{
		const char *m_pszName;
 		void (*m_pFunc)(const char*);
	};

	static const struct Item items[] =
	{
		{"SINCOS 2:6",        Test_SinCosLp},
		{"SIN 2:6  COS 2:6",  Test_SinLp_CosLp},
		{"SIN 2:14 COS 2:14", Test_SinHp},
		{"CLIP RECT",         Test_Line},
		{"BASIC CUBE",        Test_Pers},
		{"ROTATE CUBE",       Test_PersAnim},
		{"SCROLL CUBE",       Test_PersScroll},
		{"CLIP BBOX",         Test_BBoxClip},
		{"MTK",               Mtk_Main},
		{NULL, NULL}
	};

	u8 nbItems = (sizeof(items)/sizeof(struct Item)) - 1;

	s8 autoRun = -1;
	if (0 <= autoRun)
	{
		(*(items[autoRun].m_pFunc))(items[autoRun].m_pszName);
	}

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

