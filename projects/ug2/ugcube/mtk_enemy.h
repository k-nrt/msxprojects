#ifndef MTK_ENEMY_H
#define MTK_ENEMY_H

#include "vec_math.h"
#include "mtk_model.h"

typedef struct tMtkEnemy
{
	s16x3 m_position;
	s16x3 m_velocity;
	s16x3 m_target;
	s8 m_shield;
	u8 m_timer;
	void (*m_state)(struct tMtkEnemy*);
	u8 m_visibility;
	SMtkModel* m_model;
} SMtkEnemy;

#define MTK_ENEMY_MAX (3)
extern SMtkEnemy g_mtkEnemies[MTK_ENEMY_MAX];

extern void MtkEnemyInit(void);
extern void MtkEnemyCreateModels(void);

extern void MtkEnemyUpdate(void);
extern void MtkEnemyRender(void);

extern void MtkEnemySetEntryPosition(SMtkEnemy *enemy);
extern void MtkEnemyMoveToTarget(SMtkEnemy *enemy);
extern s8 MtkEnemyAddDamage(s8 damage, SMtkEnemy *enemy);

extern void MtkEnemyStateIdle(SMtkEnemy *enemy);

extern void MtkEnemy1StateEntry(SMtkEnemy *enemy);
extern void MtkEnemy1StateMove(SMtkEnemy *enemy);

extern void MtkEnemy2StateEntry(SMtkEnemy *enemy);
extern void MtkEnemy2StateMove(SMtkEnemy *enemy);
extern void MtkEnemy2StateAttack(SMtkEnemy *enemy);

#endif //MTK_ENEMY_H
