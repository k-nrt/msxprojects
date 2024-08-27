#ifndef MTK_ENEMY_H
#define MTK_ENEMY_H

#include "vec_math.h"

typedef struct tMtkEnemy
{
	s16x3 m_position;
	s16x3 m_velocity;
	s16x3 m_target;
	u8 m_visibility;
	s8 m_shield;
	void (*m_state)(struct tMtkEnemy*);
} SMtkEnemy;

#define MTK_ENEMY_MAX (3)
extern SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

extern void MtkEnemyInit(void);
extern void MtkEnemyUpdate(void);

extern s8 MtkEnemyAddDamage(s8 damage, SMtkEnemy *enemy);

extern void MtkEnemyStateIdle(SMtkEnemy *enemy);
extern void MtkEnemyStateEntry(SMtkEnemy *enemy);
extern void MtkEnemyStateMove(SMtkEnemy *enemy);


#endif //MTK_ENEMY_H
