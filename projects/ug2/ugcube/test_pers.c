#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "mesh_cube.h"
#include "mesh_zx_plane.h"
#include "pers.h"
#include "clip.h"
#include "test.h"

#pragma codeseg CODE2

static u8 s_buffer[256];
static u8* s_pMemory = s_buffer;

static void* MemoryAllocate(u16 size)
{
	void *pResult = (void*) s_pMemory;
	s_pMemory += size;

	if(((u16)&s_buffer[255]) < ((u16)s_pMemory))
	{
		pResult = s_pMemory = s_buffer;
	}
	return pResult;
}

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
	VDPWait();
	VDPPaletteWrite(colorPrev,&g_flipper.m_u16ClearColor,1);
	VDPPaletteWrite(color,&g_flipper.m_u16DrawColor,1);
  
    g_flipper.m_u8Frame++;

    g_flipper.m_u8DisplayPage = g_flipper.m_u8Frame & 0x01;
    g_flipper.m_u8ActivePage = g_flipper.m_u8DisplayPage ^ 0x01;

    VDPSetDisplayPage(g_flipper.m_u8DisplayPage);
    VDPSetActivePage(g_flipper.m_u8ActivePage);
}

void Test_DrawModel(s16x3 *pPosition, u16* pLodVertices, const SMesh *pMesh)
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
	PersTransformNoClipVram(pLodVertices[lod], pMesh->m_nbVertices);
	PersDrawLines(pMesh->m_pIndices, pMesh->m_nbLines);
}

void Test_DrawModelClipXY(s16x3 *pPosition, u16* pLodVertices, const SMesh *pMesh)
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
	PersTransformClipXYVram(pLodVertices[lod], pMesh->m_nbVertices);
	PersDrawLinesClipXY(pMesh->m_pIndices, pMesh->m_nbLines);
}

void Test_DrawModelClipXYZ(s16x3 *pPosition, u16* pLodVertices, const SMesh *pMesh)
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
	PersTransformClipXYZVram(pLodVertices[lod], pMesh->m_nbVertices);
	PersDrawLinesClipXYZ(pMesh->m_pIndices, pMesh->m_nbLines);
}

void Test_Pers(const char* pszTitle)
{
	static u8 rz = 0;
	s8x3 *vertices = (s8x3*) MemoryAllocate(sizeof(s8x3)*32);
	u16 vramOffset = 0;
	VDPSetForegroundColor(0x11);
	VDPFill(0,0,256,212);
	VDPWait();

	PersSetVertexBuffer(0,0x0800);
	PersSetPosition(0,0,64);

	vramOffset = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0,rz,rz,1);
	rz++;
	VDPReadBytes(0,vramOffset,vertices,g_meshCube.m_nbVertices*3);

	//PersSetVertices(g_modelCube.m_pVertices, g_modelCube.m_nbVertices);
	//PersSetVertices(vertices, g_modelCube.m_nbVertices);
	PersTransformNoClipVram(vramOffset, g_meshCube.m_nbVertices);

	VDPSetForegroundColor(0xFF);
	PersDrawLines(g_meshCube.m_pIndices,g_meshCube.m_nbLines);
	Test_WaitForTrigger(pszTitle);
}

void Test_PersAnim(const char* pszTitle)
{
	static u8 rz = 0;
	u8 i;
	u16* vertices = (u16*) MemoryAllocate(sizeof(u16)*64);
	u8 u8TrigPrev = 0;
	u8 u8Frame = 0;
	
    Test_FlipperInit(pszTitle);

	PersSetVertexBuffer(0,0x0800);

	for (i = 0; i < 64; i++)
	{
		u8 rx = i << 2;
		vertices[i] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, rx, rx, rz, 1);
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
		PersTransformNoClipVram(vertices[u8Frame], g_meshCube.m_nbVertices);

        Test_FlipperSetDrawColor();
		PersDrawLines(g_meshCube.m_pIndices,g_meshCube.m_nbLines);

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
	u16* cubeVertices = (u16*) MemoryAllocate(sizeof(u16)*4);
	u16* planeVertices = (u16*) MemoryAllocate(sizeof(u16)*4);
	u8 u8TrigPrev = 0;
    u8 u8StickPrev = 0;
	s16x3 v3Position0;
	s16x3 v3Position1;

    Test_FlipperInit(pszTitle);

	PersSetVertexBuffer(1,0x0000);
	cubeVertices[0] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, rz, 0);
	cubeVertices[1] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, rz, 1);
	cubeVertices[2] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, rz, 2);
	cubeVertices[3] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, rz, 3);
	planeVertices[0] = PersRegisterVertices(g_meshZXPlane.m_pVertices, g_meshZXPlane.m_nbVertices, 0, 0, 0, 0);
	planeVertices[1] = PersRegisterVertices(g_meshZXPlane.m_pVertices, g_meshZXPlane.m_nbVertices, 0, 0, 0, 1);
	planeVertices[2] = PersRegisterVertices(g_meshZXPlane.m_pVertices, g_meshZXPlane.m_nbVertices, 0, 0, 0, 2);
	planeVertices[3] = PersRegisterVertices(g_meshZXPlane.m_pVertices, g_meshZXPlane.m_nbVertices, 0, 0, 0, 3);
	rz += 8;
	s8x3Set(v3Position0, 64,0,512);
	s8x3Set(v3Position1, 0,32,512+64);

	 
	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
        u8 u8Stick = msxBiosGetStick(0);
		s16 vx = 0;
		s16 vz = 0;
		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			break;
		}
		u8TrigPrev = u8Trig;

        if (u8Stick == 1)
        {
            vz = -8;
        }
        else if(u8Stick == 5)
        {
            vz = 8;
        }
        
        if (u8Stick == 3)
        {
            vx = -4;
        }
        else if(u8Stick == 7)
        {
            vx = 4;
        }
        u8StickPrev = u8Stick;

        Test_FlipperClear();

		v3Position0.z += vz - 4;
		v3Position0.x += vx;
		if (v3Position0.z < 48)
		{
			v3Position0.z += (640 -64);
		}
        else if (640 < v3Position0.z)
        {
            v3Position0.z -= (640-48);
        }

		v3Position1.z += vz - 4;
		v3Position1.x += vx;
		if (v3Position1.z < 48)
		{
			v3Position1.z += (640 -64);
		} 
        else if (640 < v3Position1.z)
        {
            v3Position1.z -= (640-48);
        }

        Test_FlipperSetDrawColor();
		Test_DrawModelClipXYZ(&v3Position0, cubeVertices, &g_meshCube);
		Test_DrawModelClipXYZ(&v3Position1, planeVertices, &g_meshZXPlane);
		
		Test_DrawTimerAndWait();
        Test_FlipperFlip();
	}

    Test_FlipperTerm();
}

void Test_BBoxClip(const char* pszTitle)
{
	u8 u8TrigPrev = 0;
    u8 u8StickPrev = 0;
	u16* cubeVertices = (u16*) MemoryAllocate(sizeof(u16)*4);

	s16x3 v3Position0;

    Test_FlipperInit(pszTitle);

	PersSetVertexBuffer(1,0x0000);
	cubeVertices[0] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 0);
	cubeVertices[1] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 1);
	cubeVertices[2] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 2);
	cubeVertices[3] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 3);
	s8x3Set(v3Position0, 64,0,512);
	 
	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
        u8 u8Stick = msxBiosGetStick(0);
		s16 vx = 0;
		s16 vz = 0;
		if(u8TrigPrev == 0 && u8Trig != 0)
		{
			break;
		}
		u8TrigPrev = u8Trig;

        if (u8Stick == 1)
        {
            vz = -8;
        }
        else if(u8Stick == 5)
        {
            vz = 8;
        }
        
        if (u8Stick == 3)
        {
            vx = -4;
        }
        else if(u8Stick == 7)
        {
            vx = 4;
        }
        u8StickPrev = u8Stick;

        Test_FlipperClear();

		v3Position0.z += vz;
		v3Position0.x += vx;
		if (v3Position0.z < 48)
		{
			v3Position0.z += (640 -64);
		}
        else if (640 < v3Position0.z)
        {
            v3Position0.z -= (640-48);
        }

        Test_FlipperSetDrawColor();
		Test_DrawModelClipXYZ(&v3Position0, cubeVertices, &g_meshCube);
		
		Test_DrawTimerAndWait();
        Test_FlipperFlip();
	}

    Test_FlipperTerm();
}
