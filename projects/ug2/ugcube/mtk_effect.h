#ifndef MTK_EFFECT_H
#define MTK_EFFECT_H

#include "vec_math.h"

typedef struct tMtkEffect
{
	s16x3 m_position;
	s16x3 m_velocity;
	u8 m_type;
	u8 m_timer;
} SMtkEffect;

enum EMtkEffectType
{
	kMtkEffectType_None,
	kMtkEffectType_Explosion,
};

#define MTK_EFFECT_MAX (3)
extern SMtkEffect g_mtkEfects[MTK_EFFECT_MAX];
extern void MtkEffectInit();
extern u8 MtkEffectSpawn(enum EMtkEffectType type, const s16x3 *pPosition, const s16x3 *pVelocity);
extern void MtkEffectUpdate();

#endif //MTK_EFFECT_H
