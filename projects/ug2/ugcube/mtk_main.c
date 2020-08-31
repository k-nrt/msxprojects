#include <msx-types.h>
#include <msx-bios-wrapper.h>

#include "pers.h"
#include "clip.h"
#include "flipper.h"
#include "vdp_command.h"
#include "mesh.h"
#include "e1.inc"

static u16 s_vertices[64];

extern u8 g_u8Timer;

void WaitVSync()
{
	volatile u8 u8Timer = g_u8Timer;
	VDPWait();
	while(u8Timer == g_u8Timer);
}

void Mtk_Main(const char* pszTitle)
{
	static u8 rz = 0;
	u8 i;
	u16* vertices = s_vertices;
	u8 u8TrigPrev = 0;
	u8 u8Frame = 0;
	
    FlipperInit();
	FlipperPrint(0,0,0xff,pszTitle);

	PersSetVertexBuffer(1,0x0000);

	for (i = 0; i < 64; i++)
	{
		u8 rx = i << 2;
		vertices[i] = PersRegisterVertices(g_meshe1.m_pVertices, g_meshe1.m_nbVertices, 0,rx,rz,1);
	}
	rz += 8; 

	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			//break;
		}
		u8TrigPrev = u8Trig;
        FlipperClear();

		PersSetPosition(0,0,80);
		PersTransformNoClipVram(vertices[u8Frame], g_meshe1.m_nbVertices);

        FlipperSetDrawColor();
		PersDrawLines(g_meshe1.m_pIndices,g_meshe1.m_nbLines);

		u8Frame++;
		if (64 <= u8Frame)
		{
			u8Frame = 0;
		}

		WaitVSync();
        FlipperFlip();
	}

    //FlipperTerm();
}
