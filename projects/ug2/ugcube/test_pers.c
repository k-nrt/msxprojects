#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>

#include "vdp_command.h"
#include "mesh_cube.h"
#include "mesh_zx_plane.h"
#include "pers.h"
#include "clip.h"
#include "test.h"
#include "flipper.h"

#pragma codeseg CODE2

static const SFlipperConfig s_testFlipperConfig =
{
	0x11,
	2, 13, 
	{0, 32, 256, 160},
	{
		{0,0,0,0},
		{0,0,0,0},
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,2),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,3),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,4),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,5),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,6),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,7),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,8),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,9),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,10),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,11),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,12),
		FLIPPER_VERTICAL_TILE(2,13,0,32,256,160,13),
		{0,0,0,0},
		{0,0,0,0},
	}
};


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


s8 Test_GetLod(const s16x3 *pPosition, s8x3 *pLodPosition)
{
	s16 x = pPosition->x;
	s16 y = pPosition->y;
	s16 z = pPosition->z;
	s8 lod = 0;
	s8 maxZ = 32;

	while ((127 - maxZ) < z)
	{
		x >>= 1;
		y >>= 1;
		z >>= 1;
		maxZ >>= 1;
		lod++;
	}

	if (x < -128 || 127 < x || y < -128 || 127 < y)
	{
		return -1;
	}

	pLodPosition->x = (s8) x;
	pLodPosition->y = (s8) y;
	pLodPosition->z = (s8) z;
	return lod;
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
	
    FlipperInit(&s_testFlipperConfig,0x0001,0x0537);
	FlipperPrint(0,0,0xff,pszTitle);

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
        FlipperClear();

		PersSetPosition(0,0,80);
		PersTransformNoClipVram(vertices[u8Frame], g_meshCube.m_nbVertices);

        FlipperApplyForegroundColor();
		PersDrawLines(g_meshCube.m_pIndices,g_meshCube.m_nbLines);

		u8Frame++;
		if (64 <= u8Frame)
		{
			u8Frame = 0;
		}

		Test_DrawTimerAndWait();
        FlipperFlip();
	}

    FlipperTerm();
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

    FlipperInit(&s_testFlipperConfig,0x0001,0x0537);
	FlipperPrint(0,0,0xff,pszTitle);
	Clip_SetRect
	(
		g_persContext.m_viewPort.m_left,
		g_persContext.m_viewPort.m_right,
		g_persContext.m_viewPort.m_top,
		g_persContext.m_viewPort.m_bottom
	);

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
            vx = -8;
        }
        else if(u8Stick == 7)
        {
            vx = 8;
        }
        u8StickPrev = u8Stick;

        FlipperClear();

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

        FlipperApplyForegroundColor();
		Test_DrawModelClipXYZ(&v3Position0, cubeVertices, &g_meshCube);
		Test_DrawModelClipXYZ(&v3Position1, planeVertices, &g_meshZXPlane);
		
		Test_DrawTimerAndWait();
        FlipperFlip();
	}

    FlipperTerm();
}

void Test_BBoxClip(const char* pszTitle)
{
	u8 u8TrigPrev = 0;
    u8 u8StickPrev = 0;
	u16* cubeVertices = (u16*) MemoryAllocate(sizeof(u16)*4);
	u16* boxes = (u16*) MemoryAllocate(sizeof(u16)*4);

	s16x3 v3Position0;

    FlipperInit(&s_testFlipperConfig,0x0001,0x0537);
	FlipperPrint(0,0,0xff,pszTitle);
	Clip_SetRect
	(
		g_persContext.m_viewPort.m_left,
		g_persContext.m_viewPort.m_right,
		g_persContext.m_viewPort.m_top,
		g_persContext.m_viewPort.m_bottom
	);

	PersSetVertexBuffer(1,0x0000);
	cubeVertices[0] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 0);
	cubeVertices[1] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 1);
	cubeVertices[2] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 2);
	cubeVertices[3] = PersRegisterVertices(g_meshCube.m_pVertices, g_meshCube.m_nbVertices, 0, 0, 0, 3);

	boxes[0] = PersCreateBBox(cubeVertices[0],g_meshCube.m_nbVertices);
	boxes[1] = PersCreateBBox(cubeVertices[1],g_meshCube.m_nbVertices);
	boxes[2] = PersCreateBBox(cubeVertices[2],g_meshCube.m_nbVertices);
	boxes[3] = PersCreateBBox(cubeVertices[3],g_meshCube.m_nbVertices);
	
	s8x3Set(v3Position0, 64,0,512);
	 
	for(;;)
	{
		u8 u8Trig = msxBiosGetTrigger(0);
        u8 u8Stick = msxBiosGetStick(0);
		s16 vx = 0;
		s16 vz = 0;
        FlipperClear();

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
            vx = -8;
        }
        else if(u8Stick == 7)
        {
            vx = 8;
        }
        u8StickPrev = u8Stick;

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

        FlipperApplyForegroundColor();
		{
			s8x3 v3Pos;
			s8 lod = -1;
			lod = Test_GetLod(&v3Position0, &v3Pos);
			VDPPrintU8X(0,16,lod);
			VDPPrintU8X(24,16,v3Pos.x);
			VDPPrintU8X(48,16,v3Pos.y);
			VDPPrintU8X(72,16,v3Pos.z);
			if (0 <= lod && lod < 4)
			{
				enum EBBoxClip clip = PersClipBBoxVram(boxes[lod], v3Pos.x, v3Pos.y, v3Pos.z, g_persContext.m_s8ClipNear >> lod);
				VDPPrintU8X(96,16,clip);

				if (clip == kBBoxClip_In)
				{
					PersSetPosition(v3Pos.x, v3Pos.y, v3Pos.z);
					PersTransformNoClipVram(cubeVertices[lod], g_meshCube.m_nbVertices);
					PersDrawLines(g_meshCube.m_pIndices, g_meshCube.m_nbLines);
				}
				else if(clip == kBBoxClip_ScissorXY)
				{
					PersSetPosition(v3Pos.x, v3Pos.y, v3Pos.z);
					PersTransformClipXYVram(cubeVertices[lod], g_meshCube.m_nbVertices);
					PersDrawLinesClipXY(g_meshCube.m_pIndices, g_meshCube.m_nbLines);
				}
				else if (clip == kBBoxClip_ScissorXYZ)
				{
					PersSetPosition(v3Pos.x, v3Pos.y, v3Pos.z);
 					PersTransformClipXYZVram(cubeVertices[lod], g_meshCube.m_nbVertices);
					PersDrawLinesClipXYZ(g_meshCube.m_pIndices, g_meshCube.m_nbLines);
				}
				
			}
		}
		//Test_DrawModelClipXYZ(&v3Position0, cubeVertices, &g_meshCube);
		
		Test_DrawTimerAndWait();
        FlipperFlip();
	}

    FlipperTerm();
}
