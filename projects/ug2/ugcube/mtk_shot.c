#include "mtk_input.h"
#include "mtk_player.h"
#include "mtk_shot.h"

SMtkShot g_mtkShots[MTK_SHOT_MAX];

void MtkShotInit()
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

void MtkShotUpdate()
{
	u8 i;
	SMtkShot *pShot = g_mtkShots;
	u8 spawn = MtkInputIsPush(kMtkInput_Fire) ? 1 : 0;
	for(i=0; i < MTK_SHOT_MAX; i++, pShot++)
	{
		if (pShot->m_status == kMtkShotStatus_Move)
		{
			s16x3 velocity;
			s16x3Op(velocity,pShot->m_velocity,-,g_mtkPlayer.m_velocity);
			s16x3Op(pShot->m_position, pShot->m_position, +, velocity);

			if (720 < pShot->m_position.z)
			{
				pShot->m_status = kMtkShotStatus_Idle;
			}
		}
		else if (pShot->m_status == kMtkShotStatus_Hit)
		{
			s16x3 velocity;
			s16x3Op(velocity,pShot->m_velocity,-,g_mtkPlayer.m_velocity);
			s16x3Op(pShot->m_position, pShot->m_position, +, velocity);

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
