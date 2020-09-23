#include "mtk_player.h"
#include "mtk_enemy.h"

SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];
void MtkEnemyInit()
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
		pEnemy->m_status = kMtkEnemyStatus_Move;
	}
}

void MtkEnemyUpdate()
{
	u8 i;
	SMtkEnemy *pEnemy;
	for (i = 0, pEnemy = g_mtkEnemies; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		if (pEnemy->m_status == kMtkEnemyStatus_Move)
		{
			s16x3Op(pEnemy->m_position, pEnemy->m_position, -, g_mtkPlayer.m_velocity);
		}
	}
}
