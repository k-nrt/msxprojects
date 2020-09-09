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
	while(u8Timer == g_u8Timer);
}

static const SFlipperConfig s_mtkFlipperConfig =
{
	0x11,
	2, 13, 
	{0, 16, 256, 192},
	{
		{0,0,0,0},
		{0,0,0,0},
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,2),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,3),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,4),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,5),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,6),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,7),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,8),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,9),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,10),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,11),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,12),
		FLIPPER_VERTICAL_TILE(2,13,0,16,256,192,13),
		{0,0,0,0},
		{0,0,0,0},
	}
};

void Mtk_Main(const char* pszTitle)
{
	static u8 rz = 0;
	u8 i;
	u16* vertices = s_vertices;
	u8 u8TrigPrev = 0;
	u8 u8Frame = 0;
	
    FlipperInit(&s_mtkFlipperConfig,0x0001,0x0657);
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

		PersSetPosition(0,0,60);
		PersTransformNoClipVram(vertices[u8Frame], g_meshe1.m_nbVertices);

        FlipperApplyForegroundColor();
		VDPWait();
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
