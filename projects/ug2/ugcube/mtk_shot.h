#ifndef MTK_SHOT_H
#define MTK_SHOT_H

#include "vec_math.h"

typedef struct tMtkShot
{
	u8    m_status;
	s16x3 m_position;
	s16x3 m_velocity;
} SMtkShot;

enum EMtkShotStatus
{
	kMtkShotStatus_Idle,
	kMtkShotStatus_Move,
	kMtkShotStatus_Hit,
};

#define MTK_SHOT_MAX (4)
extern SMtkShot g_mtkShots[MTK_SHOT_MAX];

extern void MtkShotInit(void);
extern void MtkShotUpdate(void);

#endif //MTK_SHOT_H
