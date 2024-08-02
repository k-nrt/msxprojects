#include "mtk_input.h"
#include "mtk_player.h"

#pragma codeseg CODE2

SMtkPlayer g_mtkPlayer;
void MtkPlayerInit(void)
{
	s16x3Set(g_mtkPlayer.m_velocity,0,0,0);
}

void MtkPlayerUpdate(void)
{
	s16 vx = g_mtkPlayer.m_velocity.x;
	s16 vy = g_mtkPlayer.m_velocity.y;
	s16 vz = g_mtkPlayer.m_velocity.z;
	
	if (MtkInputIsDown(kMtkInput_Right))
	{
		if (vx < 8)
		{
			vx += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Left))
	{
		if (-8 < vx)
		{
			vx -= 1;
		}
	}
	else if (vx < 0)
	{
		vx += 1;
	}
	else if (0 < vx)
	{
		vx -= 1;
	}

	if (MtkInputIsDown(kMtkInput_Down))
	{
		if (vy < 8)
		{
			vy += 1;
		}
	}
	else if (MtkInputIsDown(kMtkInput_Up))
	{
		if (-8 < vy)
		{
			vy -= 1;
		}
	}
	else if (vy < 0)
	{
		vy += 1;
	}
	else if (0 < vy)
	{
		vy -= 1;
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
	else if (vz < 0)
	{
		vz += 1;
	}
	else if (0 < vz)
	{
		vz -= 1;
	}

	g_mtkPlayer.m_velocity.x = vx;
	g_mtkPlayer.m_velocity.y = vy;
	g_mtkPlayer.m_velocity.z = vz;
}
