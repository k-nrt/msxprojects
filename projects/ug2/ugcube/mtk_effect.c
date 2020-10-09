#include "macros.h"
#include "mtk_effect.h"

#pragma codeseg CODE2

SMtkEffect g_mtkEfects[MTK_EFFECT_MAX];

void MtkEffectInit()
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		s16x3Set(pEffect->m_position, 0, 0, 0);
		s16x3Set(pEffect->m_velocity, 0, 0, 0);
		pEffect->m_type = kMtkEffectType_None;
		pEffect->m_timer = 0;
	}
}

u8 MtkEffectSpawn(enum EMtkEffectType type, const s16x3 *pPosition, const s16x3 *pVelocity)
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	UNUSED(type);
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		if (pEffect->m_type != kMtkEffectType_None)
		{
			continue;
		}
		s16x3Copy(pEffect->m_position, (*pPosition));
		s16x3Copy(pEffect->m_velocity, (*pVelocity));
		pEffect->m_type = kMtkEffectType_Explosion;
		pEffect->m_timer = 0;
		return TRUE;
	}

	return FALSE;
}

void MtkEffectUpdate()
{
	u8 i;
	SMtkEffect *pEffect = g_mtkEfects;
	for (i = 0; i < MTK_EFFECT_MAX; i++, pEffect++)
	{
		if (pEffect->m_type == kMtkEffectType_None)
		{
			continue;
		}

		s16x3Op(pEffect->m_position, pEffect->m_position, +, pEffect->m_velocity);
		pEffect->m_timer++;
		if (4 < pEffect->m_timer)
		{
			pEffect->m_type = kMtkEffectType_None;
		}
	}
}
