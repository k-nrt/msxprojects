#ifndef MTK_WORLD_H
#define MTK_WORLD_H

#include "vec_math.h"

typedef struct tMtkWorld
{
	s16x3 m_velocity;
	s8x3 m_angularVelocity;
} SMtkWorld;
extern SMtkWorld g_mtkWorld;

extern void MtkWorldInit(void);
extern void MtkWorldSetVelocity(const s16x3* velocity);
extern void MtkWorldSetAngularVelocity(const s8x3* angularVelocity);

extern void MtkWorldMovePosition(s16x3* position);
extern void MtkWorldRotatePositionsS8Lp(u8 count, s8x3* positions);
#endif //MTK_WORLD_H


