#include "player_beam.h"
#include "player.h"

#include "model_data.h"

#include "view.h"

SPlayerBeam g_playerBeams[PLAYER_BEAM_MAX];

UgxBuffer g_bufferPlayerBeamIndices;
UgxBuffer g_bufferPlayerBeamVertices[9][9];

enum EPlayerBeamStatus
{
	kPlayerBeamStatus_Idle = 0,
	kPlayerBeamStatus_Busy = 1,
};

void PlayerBeamInitialize()
{
	register u8 i,x,y;
	register SPlayerBeam *pPlayerBeam;
	for (i = 0, pPlayerBeam = g_playerBeams; i < PLAYER_BEAM_MAX; i++,pPlayerBeam++)
	{
		Vec3Set(pPlayerBeam->m_v3Position, 0, 0, 0);
		Vec3Set(pPlayerBeam->m_v3Velocity, 0, 0, 0);
		pPlayerBeam->m_bufferVertices = 0;
		pPlayerBeam->m_liveCount = 0;
	}

	g_bufferPlayerBeamIndices = UgxCreateIndexBuffer(g_modelBeam.pIndices, g_modelBeam.nbLines * 2);
	for (x = 0; x < 9; x++)
	{
		u8 u8RotX = (x - 4) * 4;
		for (y = 0; y < 9; y++)
		{
			u8 u8RotY = (y - 4) * 4;
			g_bufferPlayerBeamVertices[x][y] = UgxCreateVertexBuffer(g_modelBeam.pVertices, g_modelBeam.nbVertices, kRotationOrder_ZXY, u8RotX, u8RotY, u8RotY * 3, 0x200);
		}
	}

}

void PlayerBeamSpawn()
{
	register u8 i;
	register SPlayerBeam *pPlayerBeam;
	for (i = 0, pPlayerBeam = g_playerBeams; i < PLAYER_BEAM_MAX; i++, pPlayerBeam++)
	{
		if ( 0 < pPlayerBeam->m_liveCount )
		{
			continue;
		}

		pPlayerBeam->m_liveCount = 8;
		Vec3Copy(pPlayerBeam->m_v3Position, g_player.m_v3Position);
		Vec3Copy(pPlayerBeam->m_v3Velocity, g_player.m_v3Velocity);
		pPlayerBeam->m_bufferVertices = g_bufferPlayerBeamVertices[g_player.m_u8ModelRotX][g_player.m_u8ModelRotY];

		return;
	}
}

void PlayerBeamUpdateAndRender()
{
	register u8 i;
	register SPlayerBeam *pPlayerBeam;

	UgxColor(0xaa);
	for (i = 0,pPlayerBeam = g_playerBeams; i < PLAYER_BEAM_MAX; i++,pPlayerBeam++)
	{
		if (pPlayerBeam->m_liveCount == 0)
		{
			continue;
		}
		pPlayerBeam->m_liveCount--;

		Vec3Op(pPlayerBeam->m_v3Position, pPlayerBeam->m_v3Position, +, pPlayerBeam->m_v3Velocity);

		ViewTransform(&pPlayerBeam->m_v3Position);

		UgxSetModelPosition(VX, VY, VZ);
		UgxDrawModel(//VX, VY, VZ,
			pPlayerBeam->m_bufferVertices,
			g_modelBeam.nbVertices,
			g_bufferPlayerBeamIndices,
			g_modelBeam.nbLines);
	}
}


