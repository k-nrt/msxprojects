#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "sincos.h"
#include "clip.h"
#include "model_cube.h"
#include "pers.h"

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

void Test_Line(const char* pszTitle)
{
	u8 c;
	u16 i;
	UNUSED(c);

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
#if 0
	for(c = 0; c < 2; c++)
	{
		for (i=16; i < 192; i += 8)
		{
			if (i&8)
			{
				VDPSetForegroundColor(0x88);
				Clip_SetLine(16, i, 128, 104);
			}
			else
			{
				VDPSetForegroundColor(0x22);
				Clip_SetLine(128,104,16,i);
			}
			if(c)
			{
				ClipRect_VDPWaitLine();
			}
			else
			{
				VDPSetForegroundColor(0x55);
				Clip_VDPWaitLine();
			}

			if (i&8)
			{
				VDPSetForegroundColor(0x88);
				Clip_SetLine(128, 104, 239, i);
			}
			else
			{
				VDPSetForegroundColor(0x22);
				Clip_SetLine(239, i, 128, 104);
			}

			if(c)
			{
				ClipRect_VDPWaitLine();
			}
			else
			{
				VDPSetForegroundColor(0x55);
			}
			Clip_VDPWaitLine();
		}

		for (i=16; i < 240; i += 8)
		{
			if (i&8)
			{
				VDPSetForegroundColor(0x88);
				Clip_SetLine(i, 16, 128, 104);
			}
			else
			{
				VDPSetForegroundColor(0x22);
				Clip_SetLine(128, 104, i, 16);
			}

			if(c)
			{
				ClipRect_VDPWaitLine();
			}
			else
			{
				VDPSetForegroundColor(0x55);
				Clip_VDPWaitLine();
			}

			if (i&8)
			{
				VDPSetForegroundColor(0x88);
				Clip_SetLine(i, 191, 128, 104);
			}
			else
			{
				VDPSetForegroundColor(0x22);
				Clip_SetLine(128, 104, i, 191);
			}

			if(c)
			{
				ClipRect_VDPWaitLine();
			}
			else
			{
				VDPSetForegroundColor(0x55);
				Clip_VDPWaitLine();
			}
	
		}
	}
#endif
	VDPSetForegroundColor(0xff);
	//VDPWaitLine(0,0,64,64);
	Test_WaitForTrigger(pszTitle);
}

void Test_Pers(const char* pszTitle)
{
	static s8x3 vertices[32];
	static u8 rz = 0;
	u16 vramOffset = 0;
	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);
	VDPWait();
	vramOffset = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0,0,rz,0);
	rz++;
	VDPReadBytes(0,vramOffset,vertices,g_modelCube.m_nbVertices*3);

	//PersSetVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices);
	//PersSetVertices(vertices, g_modelCube.m_nbVertices);
	PersSetVerticesVram(vramOffset, g_modelCube.m_nbVertices);

	VDPSetForegroundColor(0xFF);
	{
		u8 i;
		SPersScreenPos **ppPosition = (SPersScreenPos**) g_modelCube.m_pIndices;
		for(i = 0; i < g_modelCube.m_nbLines ; i++)
		{
			u8 x0,y0,x1,y1;
			
			if ((*ppPosition)->m_clipBits)
			{
				ppPosition += 2;
				continue;
			}
			ppPosition++;
			if ((*ppPosition)->m_clipBits)
			{
				ppPosition++;
				continue;
			}
			x0 = (*ppPosition)->m_x;
			y0 = (*ppPosition)->m_y;
			ppPosition--;
			x1 = (*ppPosition)->m_x;
			y1 = (*ppPosition)->m_y;
			ppPosition += 2;
			VDPWaitLine(x0,y0,x1,y1);
		}
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
		{"SINCOS 2:6",        Test_SinCosLp},
		{"SIN 2:6  COS 2:6",  Test_SinLp_CosLp},
		{"SIN 2:14 COS 2:14", Test_SinHp},
		{"CLIP RECT",         Test_Line},
		{"BASIC CUBE",        Test_Pers},
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