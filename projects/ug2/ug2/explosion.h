#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <msx-types.h>
#include "vec_math.h"

typedef struct tagExplosion
{
	s16x3 m_v3Position;
	s16x3 m_v3Velocity;
	u8 m_liveCount;
} SExplosion;

#define EXPLOSION_MAX (3)
extern SExplosion g_explosion[EXPLOSION_MAX];

void ExplosionInitialize();
void ExplosionUpdateAndRender();

void ExplosionRender(const s16x3 *pPosition, u8 liveCount);

#endif //EXPLOSION_H
