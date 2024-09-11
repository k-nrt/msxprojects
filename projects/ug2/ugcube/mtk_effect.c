#include <msx-rand.h>
#include "macros.h"
#include "mtk_effect.h"

#pragma codeseg CODE2

#include "mtk_mesh_exp0.inc"
#include "mtk_mesh_exp1.inc"
#include "mtk_mesh_exp2.inc"
#include "mtk_mesh_exp3.inc"

SMtkEffect g_mtkEfects[MTK_EFFECT_MAX];
SMtkModel g_modelExplosion[4*4];

static u8 counter;
static SMtkEffect* itEffect;

void MtkEffectInit(void)
{
	itEffect = g_mtkEfects;
	for (counter = 0; counter < MTK_EFFECT_MAX; counter++, itEffect++)
	{
		s16x3Set(itEffect->m_position, 0, 0, 0);
		s16x3Set(itEffect->m_velocity, 0, 0, 0);
		itEffect->m_type = kMtkEffectType_None;
		itEffect->m_timer = 0;
	}
}

void MtkEffectCreateModels(void)
{
	for (counter = 0; counter < 4; counter++)
	{
		MtkModelCreate(&g_modelExplosion[counter*4+0], &g_meshExp0, 0, 0, counter << 6);
		MtkModelCreate(&g_modelExplosion[counter*4+1], &g_meshExp1, 0, 0, counter << 6);
		MtkModelCreate(&g_modelExplosion[counter*4+2], &g_meshExp2, 0, 0, counter << 6);
		MtkModelCreate(&g_modelExplosion[counter*4+3], &g_meshExp3, 0, 0, counter << 6);
	}
}

u8 MtkEffectSpawn(enum EMtkEffectType type, const s16x3 *pPosition, const s16x3 *pVelocity)
{
	itEffect = g_mtkEfects;
	UNUSED(type);
	for (counter = 0; counter < MTK_EFFECT_MAX; counter++, itEffect++)
	{
		if (itEffect->m_type != kMtkEffectType_None)
		{
			continue;
		}
		s16x3Copy(itEffect->m_position, (*pPosition));
		s16x3Copy(itEffect->m_velocity, (*pVelocity));
		itEffect->m_type = kMtkEffectType_Explosion;
		itEffect->m_timer = 0;
		itEffect->m_model = &g_modelExplosion[msxRandGet8() & 0x0c];
		return TRUE;
	}

	return FALSE;
}

void MtkEffectUpdate(void)
{
	itEffect = g_mtkEfects;
	for (counter = 0; counter < MTK_EFFECT_MAX; counter++, itEffect++)
	{
		if (itEffect->m_type == kMtkEffectType_None)
		{
			continue;
		}

		s16x3Op(itEffect->m_position, itEffect->m_position, +, itEffect->m_velocity);
		itEffect->m_timer++;
		if (4 < itEffect->m_timer)
		{
			itEffect->m_type = kMtkEffectType_None;
		}
	}
}

void MtkEffectRender(void)
{
	itEffect = g_mtkEfects;
	for (counter = 0; counter < MTK_EFFECT_MAX; counter++, itEffect++)
	{
		if (itEffect->m_type == kMtkEffectType_None)
		{
			continue;
		}
		else
		{
			u8 index = itEffect->m_timer - 1;
			if (3 < index)
			{
				index = 3;
			}
			MtkModelDrawNoClip(itEffect->m_model + index, &itEffect->m_position);
		}
	}
}
