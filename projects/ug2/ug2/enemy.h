#ifndef ENEMY_H
#define ENEMY_H

#include "vec_math.h"
typedef struct tagEnemy
{
	u8 m_eType;
	u8 m_eStatus;
	u8 m_liveCount;
	u8 m_rotation;
	s16x3 m_v3Position;
} SEnemy;

#define ENEMY_MAX (8)
extern SEnemy g_enemies[ENEMY_MAX];

enum EEnemyType
{
	kEnemyType_None,
	kEnemyType_Type0,
	kEnemyType_Type1,
	kEnemyType_Type2,
};

enum EEnemyStatus
{
	kEnemyStatus_Idle,
	kEnemyStatus_Active,
	kEnemyStatus_Explosion,
};

void EnemyInitialize();
void EnemyUpdateAndRender();

void EnemyBeginExplosion(SEnemy *pEnemy);

#endif //ENEMY_H