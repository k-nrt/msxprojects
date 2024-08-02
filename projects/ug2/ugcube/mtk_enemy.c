#include "mtk_player.h"
#include "mtk_enemy.h"
#include "msx-rand.h"

#pragma codeseg CODE2

SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];
void MtkEnemyInit(void)
{
	u8 i;
	SMtkEnemy *pEnemy = g_mtkEnemies;
	for (i = 0; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		s16 x = i;
		s16 z = i;
		x = -48 + x*48; 
		z = 160 + 40*z;
		s16x3Set(pEnemy->m_position, x, 0, z);
		s16x3Set(pEnemy->m_velocity, 0, 0, 0);
		s16x3Set(pEnemy->m_target, x, 0, z);
		pEnemy->m_status = kMtkEnemyStatus_Move;
	}
}

void MtkEnemyUpdate(void)
{
	u8 i;
	SMtkEnemy *pEnemy;
	for (i = 0, pEnemy = g_mtkEnemies; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		if (pEnemy->m_status == kMtkEnemyStatus_Idle)
		{
			s16 px = (s16)(msxRandGet16() >> 8) - 128;
			s16 py = (s16)(msxRandGet16() >> 8) - 128;
			s16 tx = (s16)(msxRandGet16() >> 9) - 64;
			s16 ty = (s16)(msxRandGet16() >> 9) - 64;

			s16x3Set(pEnemy->m_position, px, py, 720);
			s16x3Set(pEnemy->m_target, tx, ty, 200);

			pEnemy->m_status = kMtkEnemyStatus_Move;
		}
		else if (pEnemy->m_status == kMtkEnemyStatus_Move)
		{
			static s16 px, py, pz;
			static s16 vx, vy, vz;
			static s16 tx, ty, tz;
			
			px = pEnemy->m_position.x;
			py = pEnemy->m_position.y;
			pz = pEnemy->m_position.z;

			vx = pEnemy->m_velocity.x;
			vy = pEnemy->m_velocity.y;
			vz = pEnemy->m_velocity.z;

			tx = pEnemy->m_target.x;
			ty = pEnemy->m_target.y;
			tz = pEnemy->m_target.z;

			if (px < tx && vx < 8)
				vx += 1;
			else if (tx < px && -8 < vx)
				vx -= 1;
			
			if (py < ty && vy < 8)
				vy += 1;
			else if (ty < py && -8 < vy)
				vy -= 1;

			if (pz < tz && vz < 8)
				vz += 1;
			else if (tz < pz && -8 < vz)
				vz -= 1;

			pEnemy->m_position.x = px + vx - g_mtkPlayer.m_velocity.x;
			pEnemy->m_position.y = py + vy - g_mtkPlayer.m_velocity.y;
			pEnemy->m_position.z = pz + vz - g_mtkPlayer.m_velocity.z;

			s16x3Set(pEnemy->m_velocity, vx, vy, vz);
			s16x3Op(pEnemy->m_target, pEnemy->m_target, -, g_mtkPlayer.m_velocity);
		}
	}
}
