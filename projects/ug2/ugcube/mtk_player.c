#include "mtk_input.h"
#include "mtk_player.h"
#include "mtk_world.h"

#pragma codeseg CODE2

SMtkPlayer g_mtkPlayer;
void MtkPlayerInit(void)
{
	s16x3Set(g_mtkPlayer.m_velocity, 0, 0, 0);
	s8x3Set(g_mtkPlayer.m_angularVelocity, 0, 0, 0);
}

void MtkPlayerUpdate(void)
{
	s8 avx = g_mtkPlayer.m_angularVelocity.x;
	s8 avy = g_mtkPlayer.m_angularVelocity.y;
	s16 vz = g_mtkPlayer.m_velocity.z;
	
	if (MtkInputIsDown(kMtkInput_Right))
	{
		if (avy < 8)
		{
			avy += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Left))
	{
		if (-8 < avy)
		{
			avy -= 1;
		}
	}
	else if (avy < 0)
	{
		avy += 1;
	}
	else if (0 < avy)
	{
		avy -= 1;
	}

	if (MtkInputIsDown(kMtkInput_Down))
	{
		if (-8 < avx)
		{
			avx -= 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Up))
	{
		if (avx < 8)
		{
			avx += 1;
		}
	}
	else if (avx < 0)
	{
		avx += 1;
	}
	else if (0 < avx)
	{
		avx -= 1;
	}

	if (MtkInputIsDown(kMtkInput_Thrust))
	{
		if (vz < 8)
		{
			vz += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Fire))
	{
		if (-8 < vz)
		{
			vz -= 1;
		}
	}
	else if (vz < 1)
	{
		vz += 1;
	}
	else if (1 < vz)
	{
		vz -= 1;
	}

	g_mtkPlayer.m_angularVelocity.x = avx;
	g_mtkPlayer.m_angularVelocity.y = avy;
	g_mtkPlayer.m_velocity.z = vz;

	MtkWorldSetVelocity(&g_mtkPlayer.m_velocity);
	MtkWorldSetAngularVelocity(&g_mtkPlayer.m_angularVelocity);
}
