#ifndef MTK_ENEMY_SHOT_H
#define MTK_ENEMY_SHOT_H

#include "vec_math.h"
#include "mtk_model.h"

enum EMtkEnemyShotState
{
	kMtkEnemyShotState_Idle,
	kMtkEnemyShotState_Busy,
};

typedef struct tMtkEnemyShot
{
	s16x3 m_position;
	s16x3 m_velocity;
	u8 m_state;
	SMtkModel* m_model;
} SMtkEnemyShot;

#define MTK_ENEMY_SHOT_COUNT (4)
extern SMtkEnemyShot g_mtkEnemyShots[MTK_ENEMY_SHOT_COUNT];

extern void MtkEnemyShotInit(void);
extern void MtkEnemyShotCreateModels(void);
extern void MtkEnemyShotUpdate(void);
extern void MtkEnemyShotRender(void);

extern void MtkEnemyShotLaunch(s16x3* position, s16x3* velocity);

#endif //MTK_ENEMY_SHOT_H
