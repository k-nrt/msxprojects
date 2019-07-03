#include "macros.h"
#include "bg.h"
#include "pers.h"
#include "model_data.h"

#include "view.h"
#include "player.h"
#include "macros.h"

#define VIEW_ZROT_MAX VIEW_ROTATION_Z_MAX
#define CUBE_ZROT_MAX VIEW_ROTATION_Z_MAX

UgxBuffer g_bufferCubeIndices;
UgxBuffer g_bufferCubeVertices[CUBE_ZROT_MAX*2+1];

SBGObject g_bgObjects[BGOBJECT_MAX];

void BGInitialize()
{
	register u8 i;
	register s8 r;
	g_bufferCubeIndices = UgxCreateIndexBuffer(g_modelCube.pIndices, g_modelCube.nbLines * 2);
	for (i = 0, r = -VIEW_ZROT_MAX; i < (CUBE_ZROT_MAX*2 + 1); i++, r += VIEW_ZROT_MAX / CUBE_ZROT_MAX)
	{
		//register u8 u8RotY = (i * (VIEW_ZROT_MAX / CUBE_ZROT_MAX)) - VIEW_ZROT_MAX/2;
		g_bufferCubeVertices[i] = UgxCreateVertexBuffer(g_modelCube.pVertices, g_modelCube.nbVertices, kRotationOrder_XYZ, 0, 0, r, 0x100);
	}

	Vec3Set(g_bgObjects[0].m_v3Position, -128, -64, 512);
	Vec3Set(g_bgObjects[1].m_v3Position, 128, -64, 512);
	Vec3Set(g_bgObjects[2].m_v3Position, -128, 64, 512);
	Vec3Set(g_bgObjects[3].m_v3Position, 128, 64, 512);
	Vec3Set(g_bgObjects[4].m_v3Position, -64, 0, 256);
	Vec3Set(g_bgObjects[5].m_v3Position, 64, 0, 256);
	Vec3Set(g_bgObjects[6].m_v3Position, 0, -32, 768);
	Vec3Set(g_bgObjects[7].m_v3Position, 0, 32, 768);
}

void BGUpdateAndRender()
{
	register u8 i;
	register SBGObject *pBGObject;
	for (i = 0, pBGObject = g_bgObjects; i < BGOBJECT_MAX; i++, pBGObject++)
	{
		register s8 modelRotY = CLAMP(((s8)g_view.m_u8RotationZ) + 8, 0, 17);

		pBGObject->m_v3Position.z -= g_player.m_v3Velocity.z >> 2;

		if (pBGObject->m_v3Position.z < 0)
		{
			pBGObject->m_v3Position.z = 768;
		}

		ViewTransform(&pBGObject->m_v3Position);

		UgxColor(0xff);
		UgxSetModelPosition(VX, VY, VZ);
		UgxDrawModel
		(
//			VX, VY, VZ,
			g_bufferCubeVertices[modelRotY],
			g_modelCube.nbVertices,
			g_bufferCubeIndices,
			g_modelCube.nbLines
		);
	}
}

