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
		s16x3Set(pEnemy->m_position, 0, 0, 0);
		s16x3Set(pEnemy->m_velocity, 0, 0, 0);
		s16x3Set(pEnemy->m_target, 0, 0, 0);
		pEnemy->m_visibility = FALSE;
		pEnemy->m_shield = 0;
		pEnemy->m_state = MtkEnemyStateIdle;
	}
}

void MtkEnemyUpdate(void)
{
	u8 i;
	SMtkEnemy *pEnemy;
	for (i = 0, pEnemy = g_mtkEnemies; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		pEnemy->m_state(pEnemy);
	}
}

s8 MtkEnemyAddDamage(s8 damage, SMtkEnemy *enemy)
{
	if (0 < enemy->m_shield)
	{
		enemy->m_shield -= damage;
		if (enemy->m_shield <= 0)
		{
			enemy->m_shield = 0; 
			enemy->m_visibility = FALSE;
			enemy->m_state = MtkEnemyStateIdle;
		}
		else
		{
			enemy->m_velocity.z += 8;
		}
	}

	return enemy->m_shield;
}

void MtkEnemyStateIdle(SMtkEnemy *enemy)
{
	enemy->m_state = MtkEnemyStateEntry;
}

void MtkEnemyStateEntry(SMtkEnemy *enemy)
{
	s16 px = (s16)(msxRandGet16() >> 8) - 128;
	s16 py = (s16)(msxRandGet16() >> 8) - 128;
	s16 tx = (s16)(msxRandGet16() >> 9) - 64;
	s16 ty = (s16)(msxRandGet16() >> 9) - 64;

	s16x3Set(enemy->m_position, px, py, 720);
	s16x3Set(enemy->m_target, tx, ty, 200);

	enemy->m_visibility = TRUE;
	enemy->m_state = MtkEnemyStateMove;
	enemy->m_shield = 3;
}

void MtkEnemyStateMove(SMtkEnemy *enemy)
{
	static s16 px, py, pz;
	static s16 vx, vy, vz;
	static s16 tx, ty, tz;

	px = enemy->m_position.x;
	py = enemy->m_position.y;
	pz = enemy->m_position.z;

	if (g_mtkPlayer.m_angularVelocity.x)
	{
		SinCos_SetS16XY(py, pz);
		SinCos_RotateXYS16Hp(-g_mtkPlayer.m_angularVelocity.x);
		py = SinCos_GetS16X();
		pz = SinCos_GetS16Y();
	}

	if (g_mtkPlayer.m_angularVelocity.y)
	{
		SinCos_SetS16XY(pz, px);
		SinCos_RotateXYS16Hp(-g_mtkPlayer.m_angularVelocity.y);
		pz = SinCos_GetS16X();
		px = SinCos_GetS16Y();
	}

	vx = enemy->m_velocity.x;
	vy = enemy->m_velocity.y;
	vz = enemy->m_velocity.z;

	tx = enemy->m_target.x;
	ty = enemy->m_target.y;
	tz = enemy->m_target.z;

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

	enemy->m_position.x = px + vx - g_mtkPlayer.m_velocity.x;
	enemy->m_position.y = py + vy - g_mtkPlayer.m_velocity.y;
	enemy->m_position.z = pz + vz - g_mtkPlayer.m_velocity.z;

	s16x3Set(enemy->m_velocity, vx, vy, vz);
	//s16x3Op(pEnemy->m_target, pEnemy->m_target, -, g_mtkPlayer.m_velocity);
}
