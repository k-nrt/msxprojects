#include "mtk_enemy_shot.h"
#include "mtk_world.h"

#include "mtk_enemy_shot.inc"

SMtkEnemyShot g_mtkEnemyShots[MTK_ENEMY_SHOT_COUNT];

#define MTK_ENEMY_SHOT_ROTATION_COUNT (8)
SMtkModel g_modelEnemyShots[MTK_ENEMY_SHOT_ROTATION_COUNT];

void MtkEnemyShotInit(void)
{
	u8 i;
	SMtkEnemyShot* shot = g_mtkEnemyShots;
	for(i = 0; i < MTK_ENEMY_SHOT_COUNT; i++, shot++)
	{
		s16x3Set(shot->m_position, 0, 0, 0);
		s16x3Set(shot->m_velocity, 0, 0, 0);
		shot->m_state = kMtkEnemyShotState_Idle;
		shot->m_rotation = 0;
		shot->m_model = &g_modelEnemyShots[0];
	}
}

void MtkEnemyShotCreateModels(void)
{
	u8 i;
	for(i = 0; i < MTK_ENEMY_SHOT_ROTATION_COUNT; i++)
	{
		MtkModelCreate(&g_modelEnemyShots[i], &g_meshEnemyShot, 0, 0, i << 4);
	}
}

void MtkEnemyShotUpdate(void)
{
	u8 i;
	SMtkEnemyShot* shot = g_mtkEnemyShots;
	for(i = 0; i < MTK_ENEMY_SHOT_COUNT; i++, shot++)
	{
		if (shot->m_state == kMtkEnemyShotState_Idle)
		{
			continue;
		}

		if (shot->m_position.z < 0)
		{
			shot->m_state = kMtkEnemyShotState_Idle;
		}
		else
		{
			MtkWorldMovePosition(&shot->m_position);
			shot->m_position.z += shot->m_velocity.z;
			shot->m_model = &g_modelEnemyShots[shot->m_rotation & 0x7];
			shot->m_rotation++;
		}
	}
}

void MtkEnemyShotRender(void)
{
	u8 i;
	SMtkEnemyShot* shot = g_mtkEnemyShots;
	for(i = 0; i < MTK_ENEMY_SHOT_COUNT; i++, shot++)
	{
		if (shot->m_state == kMtkEnemyShotState_Idle)
		{
			continue;
		}

		MtkModelDrawBBoxClip(shot->m_model, &shot->m_position);
	}
}

void MtkEnemyShotLaunch(s16x3* position, s16x3* velocity)
{
	u8 i;
	SMtkEnemyShot* shot = g_mtkEnemyShots;
	for(i = 0; i < MTK_ENEMY_SHOT_COUNT; i++, shot++)
	{
		if (shot->m_state == kMtkEnemyShotState_Idle)
		{
			s16x3Copy(shot->m_position, (*position));
			s16x3Copy(shot->m_velocity, (*velocity));
			shot->m_state = kMtkEnemyShotState_Busy;
			return;
		}
	}
}
