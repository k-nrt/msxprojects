#include "mtk_player.h"
#include "mtk_enemy.h"
#include "mtk_world.h"
#include "msx-rand.h"
#include "mtk_enemy_shot.h"

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
		pEnemy->m_timer = 0;
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

void MtkEnemySetEntryPosition(SMtkEnemy *enemy)
{
	s16 px = (s16)(msxRandGet16() >> 8) - 128;
	s16 py = (s16)(msxRandGet16() >> 8) - 128;
	s16 tx = (s16)(msxRandGet16() >> 9) - 64;
	s16 ty = (s16)(msxRandGet16() >> 9) - 64;

	s16x3Set(enemy->m_position, px, py, 720);
	s16x3Set(enemy->m_target, tx, ty, 200);
}

void MtkEnemyMoveToTarget(SMtkEnemy *enemy)
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
	switch (msxRandGet8() & 0x7)
	{
	case 0:
	default:
		enemy->m_state = MtkEnemy1StateEntry;
		break;

	case 1:
		enemy->m_state = MtkEnemy2StateEntry;
		break;
	}
}

void MtkEnemy1StateEntry(SMtkEnemy *enemy)
{
	MtkEnemySetEntryPosition(enemy);

	enemy->m_visibility = TRUE;
	enemy->m_state = MtkEnemy1StateMove;
	enemy->m_shield = 1;
	enemy->m_model = &g_modelEnemy1;
}

void MtkEnemy1StateMove(SMtkEnemy *enemy)
{
	MtkEnemyMoveToTarget(enemy);
}

void MtkEnemy2StateEntry(SMtkEnemy *enemy)
{
	MtkEnemySetEntryPosition(enemy);

	enemy->m_visibility = TRUE;
	enemy->m_state = MtkEnemy2StateMove;
	enemy->m_shield = 3;
	enemy->m_timer = 20;
	enemy->m_model = &g_modelEnemy2Move;
}

void MtkEnemy2StateMove(SMtkEnemy *enemy)
{
	MtkEnemyMoveToTarget(enemy);

	if (!enemy->m_timer)
	{
		switch (msxRandGet8() & 0x3)
		{
		default:
		case 0: //. continue.
			{
				s16 tx = (s16)(msxRandGet16() >> 9) - 64;
				s16 ty = (s16)(msxRandGet16() >> 9) - 64;
				s16 tz = (s16)(msxRandGet16() >> 9) - 64;
				enemy->m_target.x += tx;
				enemy->m_target.y += ty;
				enemy->m_target.z += tz;
				enemy->m_timer = 20;

				if (enemy->m_model == &g_modelEnemy2Move)
				{
					enemy->m_model = &g_modelEnemy2Defence;
				}
				else
				{
					enemy->m_model = &g_modelEnemy2Move;
				}
			}
			break;

		case 1: //. move far.
			{
				s16 tx = (s16)(msxRandGet16() >> 9) - 64;
				s16 ty = (s16)(msxRandGet16() >> 9) - 64;
				s16 tz = (s16)(msxRandGet16() >> 9) - 64;
				enemy->m_target.x += tx*4;
				enemy->m_target.y += ty*4;
				enemy->m_target.z = 700 + tz;
				enemy->m_timer = 60;
			}
			break;

		case 2: //. move near.
			{
				s16 tx = (s16)(msxRandGet16() >> 9) - 64;
				s16 ty = (s16)(msxRandGet16() >> 9) - 64;
				s16 tz = (s16)(msxRandGet16() >> 9) - 64;
				enemy->m_target.x += tx;
				enemy->m_target.y += ty;
				enemy->m_target.z = 300 + tz;
				enemy->m_timer = 60;
			}
			break;

		case 3: //. attack.
			enemy->m_timer = 60;
			enemy->m_target.x = 0;
			enemy->m_target.y = 0;
			enemy->m_state = MtkEnemy2StateAttack;
			enemy->m_model = &g_modelEnemy2Attack;
			break;
		}
	}
	else
	{
		enemy->m_timer--;
	}
}

void MtkEnemy2StateAttack(SMtkEnemy *enemy)
{
	MtkEnemyMoveToTarget(enemy);

	if (!enemy->m_timer)
	{
		s16 tx = (s16)(msxRandGet16() >> 9) - 64;
		s16 ty = (s16)(msxRandGet16() >> 9) - 64;
		s16 tz = (s16)(msxRandGet16() >> 9) - 64;
		s16x3 velocity;
		
		enemy->m_target.x += tx;
		enemy->m_target.y += ty;
		enemy->m_target.z += tz;
		enemy->m_timer = 20;
		enemy->m_model = &g_modelEnemy2Move;

		s16x3Set(velocity, 0, 0, -32);
		MtkEnemyShotLaunch(&enemy->m_position, &velocity);

		if (!(msxRandGet8() & 3))
		{
			enemy->m_state = MtkEnemy2StateMove;
		}
	}
	else
	{
		enemy->m_timer--;
	}
}
