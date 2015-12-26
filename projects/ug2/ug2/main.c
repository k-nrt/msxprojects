#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-rand.h>
#include "ux_render.h"
#include "vdp_command.h"
#include "sincos.h"
#include "pers.h"
#include "math.h"
#include "key_matrix.h"
#include "vec_math.h"

#include "model_data.h"
#include "player.h"
#include "view.h"

#include "input.h"

#include "player_beam.h"
#include "explosion.h"

#include "bg.h"

#include "enemy.h"

#pragma codeseg CODE2




UgxBuffer g_bufferPlayerIndices;
UgxBuffer g_bufferPlayerVertices[9][9];


UgxBuffer g_bufferGridXYIndices;
UgxBuffer g_bufferGridXYVertices[17];

u8 g_screenFlash = 0;

u8 g_debug = FALSE;
void UgxCopyToVRAM8Internal(void *pDst, const void* pSrc, u8 sizeInBytes);

void Test()
{
	u8 values[8] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef };
	VDPSetDisplayPage(2);
	UgxCopyToVRAM8Internal((void *)0x000, values, sizeof(values));
	for (;;)
	{

	}
}

void DetectIntersectionPlayerBeamsAndEnemies()
{
	register u8 i, j;
	register SPlayerBeam *pPlayerBeam;
	register SEnemy *pEnemy;
	for (i = 0, pPlayerBeam = g_playerBeams; i < PLAYER_BEAM_MAX; i++,pPlayerBeam++)
	{
		if (pPlayerBeam->m_liveCount == 0)
		{
			continue;
		}

		for (j = 0, pEnemy = g_enemies; j < ENEMY_MAX; j++, pEnemy++)
		{
			if (pEnemy->m_eStatus != kEnemyStatus_Active)
			{
				continue;
			}

			if ((pPlayerBeam->m_v3Position.x - 32) < pEnemy->m_v3Position.x && pEnemy->m_v3Position.x < (pPlayerBeam->m_v3Position.x +32) &&
				(pPlayerBeam->m_v3Position.y - 32) < pEnemy->m_v3Position.y && pEnemy->m_v3Position.y < (pPlayerBeam->m_v3Position.y + 32) &&
				(pPlayerBeam->m_v3Position.z - 32) < pEnemy->m_v3Position.z && pEnemy->m_v3Position.z < (pPlayerBeam->m_v3Position.z + 32))
			{
				EnemyBeginExplosion(pEnemy);
				g_screenFlash = 2;
				pPlayerBeam->m_liveCount = 0;
			}
		}
	}
}

void main(void)
{
	u8 x,y;
	u8 nColor;
	u8 page;

	msx2BiosChangeModePalette(5);
	msxBiosChangeColor(6, 15, 4, 7);
	VDPSetColor(15,4,7);

	//Test();

	VDPSetActivePage(0);

	VDPWait();
	VDPSetForegroundColor(0x44);
	VDPFill(0, 0, 256, 212);

	VDPSetForegroundColor(0xff);
	VDPPrint(0, 0, "PROJECT UG2");

	VDPSetForegroundColor(0xff);
	for (y = 0; y < 255; y++)
	{
		s16 s0 = SinCos_GetSin(y);
		s16 c0 = SinCos_GetCos(y);
		s16 yy;

		VDPSetForegroundColor(0x88);
		VDPWaitLine(y, 106, y, 106 + (s0 >> 2));
		VDPSetForegroundColor(0x22);
		VDPWaitLine(y, 106, y, 106 + (c0 >> 2));

		yy = Math_MulU16xU16_U32(20, s0) >> 8;
		VDPSetForegroundColor(0xdd);
		VDPWaitLine(y, 106, y, 106 + yy);
	}

	VDPSetForegroundColor(0xff);
	for (y = 0; y < 8; y++)
	{
		VDPPrintU8D(y * 32, 8, Math_MulU8xU8_U8(3, y));
		VDPPrintU8D(y * 32, 16, Math_MulU8xU8_U8(5, y + 16));
		VDPPrintU8D(y * 32, 24, Math_MulU8xU8_U8(7, y + 32));
		VDPPrintU8D(y * 32, 32, Math_MulU8xU8_U8(10, y));
		VDPPrintU8D(y * 32, 40, Math_MulU8xU8_U8(20, y));
		VDPPrintU8D(y * 32, 48, Math_MulU8xU8_U8(30, y));
	}

	for (y = 0; y < 4; y++)
	{
		VDPPrintU16D(y * 64, 56, Math_MulU8xU8_U16(100, y));
		VDPPrintU16D(y * 64, 64, Math_MulU8xU8_U16(202, y + 16));
	}
	VDPPrintU16D(0, 72, Math_MulU8xU8_U16(255, 255));

	VDPPrintU8D(0,  80, sizeof(char) );
	VDPPrintU8D(4 * 8, 80, sizeof(short));
	VDPPrintU8D(8 * 8, 80, sizeof(int));
	VDPPrintU8D(12 * 8, 80, sizeof(long));

	{
		const u32 a = 0xffff;
		const u32 b = 0x1234;
		const u32 r0 = a*b;
		u32 r1 = Math_MulU16xU16_U32(a, b);
		VDPPrintU16X(0,  88, r0 >> 16);
		VDPPrintU16X(32, 88, r0 & 0xffff);

		VDPPrintU16X(32*2, 88, r1 >> 16);
		VDPPrintU16X(32*3, 88, r1 & 0xffff);

		VDPPrintU16X(32*4, 88, 0xdead);
	}


	VDPSetActivePage(1);
	VDPSetForegroundColor(0x44);
	VDPWait();
	VDPFill(0, 0, 256, 212);
	VDPPrint(0, 0, "PROJECT UG2");

	UgxInitialize();
	//UxRenderInit();

	g_bufferGridXYIndices = UgxCreateIndexBuffer(g_modelGridXY.pIndices, g_modelGridXY.nbLines * 2);
	for (y = 0; y < 17; y++)
	{
		u8 u8RotY = (y*2) - 16;
		g_bufferGridXYVertices[y] = UgxCreateVertexBuffer(g_modelGridXY.pVertices, g_modelGridXY.nbVertices, kRotationOrder_XYZ, 0, 0, u8RotY,0x100);
	}

	g_bufferPlayerIndices = UgxCreateIndexBuffer(g_modelPlayer.pIndices, g_modelPlayer.nbLines * 2);
	for (x = 0; x < 9; x++)
	{
		u8 u8RotX = (x - 4) * 4;
		for (y = 0; y < 9; y++)
		{
			u8 u8RotY = (y - 4)*4;
			g_bufferPlayerVertices[x][y] = UgxCreateVertexBuffer(g_modelPlayer.pVertices,g_modelPlayer.nbVertices, kRotationOrder_ZXY, u8RotX, u8RotY, u8RotY*3, 0x100);
		}
	}

	page = 0;
	VDPSetDisplayPage(page);
	page ^= 1;

	g_debug = FALSE;

	ViewInitialize();

	InputInitialize();

	PlayerInitialize();
	Vec3Set(g_player.m_v3Position, 0, 0, 160);

	PlayerBeamInitialize();

	ExplosionInitialize();

	BGInitialize();

	EnemyInitialize();

	for (;;)
	{
		VDPSetActivePage(page);

		if (1 < g_screenFlash)
		{
			VDPSetForegroundColor(0xff);
			g_screenFlash = 1;
		}
		else if (0 < g_screenFlash)
		{
			VDPSetForegroundColor(0x44);
			g_screenFlash = 0;
		}
		else
		{
			VDPSetForegroundColor(0x11);
		}
		UgxClearScreen();

		InputUpdate();

		PlayerUpdate();
		ViewUpdate( g_player.m_v3Position.x >> 1, g_player.m_v3Position.y >> 1, g_player.m_v3Rotation.z );



		UgxBeginFrame();

		if (g_debug)
		{
			s16x3 v3Position = { 0, 0, 0 };
			s8 modelRotY = (((s8)g_view.m_u8RotationZ) >> 1) + 8;
			if (modelRotY < 0)
			{
				modelRotY = 0;
			}
			if (16 < modelRotY)
			{
				modelRotY = 16;
			}

			ViewTransform(&v3Position);

			UgxColor(0x44);
			UgxSetModelPosition(VX, VY, 320);
			UgxDrawModel(//VX, VY, 320,
				g_bufferGridXYVertices[modelRotY], g_modelGridXY.nbVertices,
				g_bufferGridXYIndices, g_modelGridXY.nbLines);
		}

		BGUpdateAndRender();


		{
			ViewTransform(&g_player.m_v3Position);

			UgxColor(0x77);
			UgxSetModelPosition(VX, VY, VZ);
			UgxDrawModel(//VX, VY, VZ,
				g_bufferPlayerVertices[g_player.m_u8ModelRotX][g_player.m_u8ModelRotY],
				g_modelPlayer.nbVertices,
				g_bufferPlayerIndices,
				g_modelPlayer.nbLines);

			if (InputIsDown(INPUT_TRIGGER1))
			{
				PlayerBeamSpawn();
			}

			PlayerBeamUpdateAndRender();
		}

		EnemyUpdateAndRender();

		DetectIntersectionPlayerBeamsAndEnemies();

		//ExplosionUpdateAndRender();
		{
			const u8 x = UGX_CENTER_X;
			const u8 y = UGX_CENTER_Y;
			const u8 i = 4;
			const u8 o = 16;

			/*
			UgxColor(0x33);
			UgxBegin(kUgxPrimitive_Lines);
			UgxVertex2u8(x - i, y); UgxVertex2u8(x - o, y);
			UgxVertex2u8(x + i, y); UgxVertex2u8(x + o, y);
			UgxVertex2u8(x, y - i); UgxVertex2u8(x, y - o);
			UgxVertex2u8(x, y + i); UgxVertex2u8(x, y + o);
			UgxEnd();
			*/
		}

		UgxEndFrame();
		if (KeyMatrix_Press(KM_1))
		{
			g_debug ^= 0x1;
		}

		if (KeyMatrix_Press(KM_0))
		{
			UgxDumpContext();
		}

		if (g_debug)
		{
			VDPSetPrintPosition(0, 0);
			VDPPrintF("%d %d", g_player.m_v3Position.x, g_player.m_v3Position.y);
		}
		UgxFlushCommands();

		VDPWait();
		VDPSetDisplayPage(page);
		page ^= 1;
	}
}

