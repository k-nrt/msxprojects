#ifndef MTK_ENEMY_H
#define MTK_ENEMY_H

#include "vec_math.h"

typedef struct tMtkEnemy
{
	u8 m_status;
	s16x3 m_position;
	s16x3 m_velocity;
	s16x3 m_target;
} SMtkEnemy;

enum EMtkEnemyStatus
{
	kMtkEnemyStatus_Idle,
	kMtkEnemyStatus_Move,
};

#define MTK_ENEMY_MAX (3)
extern SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

extern void MtkEnemyInit();
extern void MtkEnemyUpdate();

#endif //MTK_ENEMY_H
