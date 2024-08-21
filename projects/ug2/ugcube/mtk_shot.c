#include "mtk_input.h"
#include "mtk_player.h"
#include "mtk_shot.h"

#pragma codeseg CODE2

SMtkShot g_mtkShots[MTK_SHOT_MAX];

void MtkShotInit(void)
{
	u8 i;
	SMtkShot *pShot = g_mtkShots;
	for(i=0; i < MTK_SHOT_MAX; i++, pShot++)
	{
		pShot->m_status = kMtkShotStatus_Idle;
		s16x3Set(pShot->m_position, 0, 0, 0);
		s16x3Set(pShot->m_velocity, 0, 0, 0);
	}
}

void MtkShotMove(SMtkShot *pShot)
{
	static s16 px, py, pz;
	static s16 vx, vy, vz;

	px = pShot->m_position.x;
	py = pShot->m_position.y;
	pz = pShot->m_position.z;

	vx = pShot->m_velocity.x - g_mtkPlayer.m_velocity.x;
	vy = pShot->m_velocity.y - g_mtkPlayer.m_velocity.y;
	vz = pShot->m_velocity.z - g_mtkPlayer.m_velocity.z;

	pShot->m_position.x = px + vx;
	pShot->m_position.y = py + vy;
	pShot->m_position.z = pz + vz;
}

void MtkShotUpdate(void)
{
	static u8 i;
	static SMtkShot *pShot;
	static u8 spawn;

	pShot = g_mtkShots;
	spawn = MtkInputIsPush(kMtkInput_Fire) ? 1 : 0;

	for(i=0; i < MTK_SHOT_MAX; i++, pShot++)
	{
		if (pShot->m_status == kMtkShotStatus_Move)
		{
			MtkShotMove(pShot);
			if (720 < pShot->m_position.z)
			{
				pShot->m_status = kMtkShotStatus_Idle;
			}
		}
		else if (pShot->m_status == kMtkShotStatus_Hit)
		{
			MtkShotMove(pShot);
			pShot->m_status = kMtkShotStatus_Idle;
		}
		else if (spawn)
		{
			spawn = 0;
			pShot->m_status = kMtkShotStatus_Move;
			s16x3Set(pShot->m_position,  16, 8, 65);
			pShot->m_velocity.x = g_mtkPlayer.m_velocity.x;
			pShot->m_velocity.y = g_mtkPlayer.m_velocity.y;
			pShot->m_velocity.z = 64;
		}
	}
}
