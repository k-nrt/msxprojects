#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "model_cube.h"
#include "model_zx_plane.h"
#include "pers.h"
#include "test.h"

#pragma codeseg CODE2

static u8 s_buffer[256];

struct SFlipper
{
    u8 m_u8Frame;
    u8 m_u8DisplayPage;
    u8 m_u8ActivePage;
    u16 m_u16ClearColor;
    u16 m_u16DrawColor;
} g_flipper;

#define FLIPPER_COLOR_START 2
#define FLIPPER_COLOR_COUNT 8
#define FLIPPER_COLOR_MASK  0x07
#define FLIPPER_CLEAR_WIDTH 32
#define FLIPPER_CLEAR_SHIFT 5

void Test_FlipperInit(const char *pszTitle)
{
    u8 i;
    g_flipper.m_u8Frame = 0;

    g_flipper.m_u8DisplayPage = 0;
    g_flipper.m_u8ActivePage = 1;

    g_flipper.m_u16ClearColor = 0x0000;
    g_flipper.m_u16DrawColor = 0x0777;

    msxBiosChangeColor(6,15,1,13);

    VDPSetActivePage(0);
    VDPSetForegroundColor(0x11);
    VDPFill(0,0,256,212);
    VDPWait();
    VDPSetForegroundColor(0xff);
    VDPPrint(0,0,pszTitle);

    VDPSetActivePage(1);
    VDPSetForegroundColor(0x11);
    VDPFill(0,0,256,212);
    VDPWait();
    VDPSetForegroundColor(0xff);
    VDPPrint(0,0,pszTitle);

    //VDPSetActivePage(0);

    for(i = 0; i < FLIPPER_COLOR_COUNT; i++)
    {
        VDPPaletteWrite(FLIPPER_COLOR_START + i, &g_flipper.m_u16ClearColor, 1);
    }
    //VDPPaletteWrite(FLIPPER_COLOR_START, &g_flipper.m_u16DrawColor, 1);
}

void Test_FlipperTerm()
{
    msx2BiosChangeModePalette(5);
    VDPSetActivePage(0);
    VDPSetDisplayPage(0);
    msxBiosChangeColor(6,15,1,7);
}

void Test_FlipperClear()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
    u8 u8X = (colorFrame & FLIPPER_COLOR_MASK) << FLIPPER_CLEAR_SHIFT;
    VDPSetForegroundColor(0x11);
	VDPFill(u8X, 32, FLIPPER_CLEAR_WIDTH, 160);
}

void Test_FlipperSetDrawColor()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
	u8 color = FLIPPER_COLOR_START + (colorFrame & FLIPPER_COLOR_MASK);
    VDPSetForegroundColor(color);
}

void Test_FlipperFlip()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
	u8 color = FLIPPER_COLOR_START + (colorFrame & FLIPPER_COLOR_MASK);
    u8 colorPrev = FLIPPER_COLOR_START + ((colorFrame - 1) & FLIPPER_COLOR_MASK);
	VDPPaletteWrite(colorPrev,&g_flipper.m_u16ClearColor,1);
	VDPPaletteWrite(color,&g_flipper.m_u16DrawColor,1);
  
    g_flipper.m_u8Frame++;

    g_flipper.m_u8DisplayPage = g_flipper.m_u8Frame & 0x01;
    g_flipper.m_u8ActivePage = g_flipper.m_u8DisplayPage ^ 0x01;

    VDPSetDisplayPage(g_flipper.m_u8DisplayPage);
    VDPSetActivePage(g_flipper.m_u8ActivePage);
}

void Test_DrawModel(s16x3 *pPosition, u16* pLodVertices, const SModel *pModel)
{
	s16x3 v3Temp;
	u8 lod = 0;

	s8x3Copy(v3Temp,(*pPosition));
	while (80 < v3Temp.z)
	{
		v3Temp.x >>= 1;
		v3Temp.y >>= 1;
		v3Temp.z >>= 1;
		lod++;
	}
	 
	PersSetPosition(v3Temp.x,v3Temp.y,v3Temp.z);
	PersSetVerticesVram(pLodVertices[lod], pModel->m_nbVertices);
	PersDrawLines(pModel->m_pIndices, pModel->m_nbLines);
}


void Test_Pers(const char* pszTitle)
{
	static u8 rz = 0;
	s8x3 *vertices = (s8x3*) s_buffer;
	u16 vramOffset = 0;
	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);
	VDPWait();

	PersSetVertexBuffer(0,0x0800);
	PersSetPosition(0,0,64);

	vramOffset = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0,rz,rz,1);
	rz++;
	VDPReadBytes(0,vramOffset,vertices,g_modelCube.m_nbVertices*3);

	//PersSetVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices);
	//PersSetVertices(vertices, g_modelCube.m_nbVertices);
	PersSetVerticesVram(vramOffset, g_modelCube.m_nbVertices);

	VDPSetForegroundColor(0xFF);
	PersDrawLines(g_modelCube.m_pIndices,g_modelCube.m_nbLines);
	Test_WaitForTrigger(pszTitle);
}

void Test_PersAnim(const char* pszTitle)
{
	static u8 rz = 0;
	u8 i;
	u16* vertices = (u16*) s_buffer;
	u8 u8TrigPrev = 0;
	u8 u8Frame = 0;
	
    Test_FlipperInit(pszTitle);

	PersSetVertexBuffer(0,0x0800);

	for (i = 0; i < 64; i++)
	{
		u8 rx = i << 2;
		vertices[i] = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, rx, rx, rz, 1);
	}
	rz += 8; 

	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			break;
		}
		u8TrigPrev = u8Trig;
        Test_FlipperClear();

		PersSetPosition(0,0,80);
		PersSetVerticesVram(vertices[u8Frame], g_modelCube.m_nbVertices);

        Test_FlipperSetDrawColor();
		PersDrawLines(g_modelCube.m_pIndices,g_modelCube.m_nbLines);

		u8Frame++;
		if (64 <= u8Frame)
		{
			u8Frame = 0;
		}

		Test_DrawTimerAndWait();
        Test_FlipperFlip();
	}

    Test_FlipperTerm();
}

void Test_PersScroll(const char* pszTitle)
{
	static u8 rz = 0;
	//u8 i;
	u16* cubeVertices = (u16*) s_buffer;
	u16* planeVertices = ((u16*) s_buffer)+4;
	u8 u8TrigPrev = 0;
	s16x3 v3Position0;
	s16x3 v3Position1;

    Test_FlipperInit(pszTitle);

	PersSetVertexBuffer(1,0x0000);
	cubeVertices[0] = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0, 0, rz, 0);
	cubeVertices[1] = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0, 0, rz, 1);
	cubeVertices[2] = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0, 0, rz, 2);
	cubeVertices[3] = PersRegisterVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices, 0, 0, rz, 3);
	planeVertices[0] = PersRegisterVertices(g_modelZXPlane.m_pVertices, g_modelZXPlane.m_nbVertices, 0, 0, 0, 0);
	planeVertices[1] = PersRegisterVertices(g_modelZXPlane.m_pVertices, g_modelZXPlane.m_nbVertices, 0, 0, 0, 1);
	planeVertices[2] = PersRegisterVertices(g_modelZXPlane.m_pVertices, g_modelZXPlane.m_nbVertices, 0, 0, 0, 2);
	planeVertices[3] = PersRegisterVertices(g_modelZXPlane.m_pVertices, g_modelZXPlane.m_nbVertices, 0, 0, 0, 3);
	rz += 8;
	s8x3Set(v3Position0, 64,0,512);
	s8x3Set(v3Position1, 0,32,512+64);

	 
	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			break;
		}
		u8TrigPrev = u8Trig;

        Test_FlipperClear();

		v3Position0.z -= 8;
		//v3Position.z = 49 ; 
		if (v3Position0.z < 48)
		{
			v3Position0.z = 640;
		}

		v3Position1.z -= 8;
		if (v3Position1.z < 48)
		{
			v3Position1.z = 640;
		} 

        Test_FlipperSetDrawColor();
		Test_DrawModel(&v3Position0, cubeVertices, &g_modelCube);
		Test_DrawModel(&v3Position1, planeVertices, &g_modelZXPlane);
 
		Test_DrawTimerAndWait();
        Test_FlipperFlip();
	}

    Test_FlipperTerm();
}

