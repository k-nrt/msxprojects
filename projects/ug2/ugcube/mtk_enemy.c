#include "mtk_player.h"
#include "mtk_enemy.h"
#include "mtk_world.h"
#include "msx-rand.h"

#include "mtk_mesh_enemy1.inc"
#include "mtk_mesh_enemy2_move.inc"
#include "mtk_mesh_enemy2_defence.inc"
#include "mtk_mesh_enemy2_attack.inc"

#pragma codeseg CODE2

SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

SMtkModel g_modelEnemy1;
SMtkModel g_modelEnemy2Move;
SMtkModel g_modelEnemy2Defence;
SMtkModel g_modelEnemy2Attack;

void MtkEnemyInit(void)
{
	u8 i;
	SMtkEnemy *pEnemy = g_mtkEnemies;
	for (i = 0; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		s16x3Set(pEnemy->m_position, 0, 0, 0);
		s16x3Set(pEnemy->m_velocity, 0, 0, 0);
		s16x3Set(pEnemy->m_target, 0, 0, 0);
		pEnemy->m_shield = 0;
		pEnemy->m_state = MtkEnemyStateIdle;
		pEnemy->m_visibility = FALSE;
		pEnemy->m_model = &g_modelEnemy1;
	}

	g_mtkEnemies[0].m_model = &g_modelEnemy2Move;
	g_mtkEnemies[1].m_model = &g_modelEnemy2Attack;
}

void MtkEnemyCreateModels(void)
{
	MtkModelCreate(&g_modelEnemy1, &g_meshEnemy1, 0, 128, 0);
	MtkModelCreate(&g_modelEnemy2Move, &g_meshEnemy2Move, 0, 128, 0);
	MtkModelCreate(&g_modelEnemy2Defence, &g_meshEnemy2Defence, 0, 128, 0);
	MtkModelCreate(&g_modelEnemy2Attack, &g_meshEnemy2Attack, 0, 128, 0);
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

void MtkEnemyRender(void)
{
	u8 i;
	SMtkEnemy *pEnemy = g_mtkEnemies;
	for (i = 0; i < MTK_ENEMY_MAX; i++, pEnemy++)
	{
		if (pEnemy->m_visibility)
		{
			MtkModelDrawBBoxClip(pEnemy->m_model, &pEnemy->m_position);
		}
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

	switch (msxRandGet8() & 0x7)
	{
	case 0:
	default:
		enemy->m_model = &g_modelEnemy1;
		break;

	case 1:
		enemy->m_model = &g_modelEnemy2Move;
		break;

	case 2:
		enemy->m_model = &g_modelEnemy2Defence;
		break;

	case 3:
		enemy->m_model = &g_modelEnemy2Attack;
		break;
	}
}

void MtkEnemyStateMove(SMtkEnemy *enemy)
{
	static s16 px, py, pz;
	static s16 vx, vy, vz;
	static s16 tx, ty, tz;

	px = enemy->m_position.x;
	py = enemy->m_position.y;
	pz = enemy->m_position.z;

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

	s16x3Set(enemy->m_velocity, vx, vy, vz);

	enemy->m_position.x = px + vx;
	enemy->m_position.y = py + vy;
	enemy->m_position.z = pz + vz;

	MtkWorldMovePosition(&enemy->m_position);
	//s16x3Op(pEnemy->m_target, pEnemy->m_target, -, g_mtkPlayer.m_velocity);
}
