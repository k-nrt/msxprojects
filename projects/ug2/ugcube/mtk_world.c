#include "mtk_world.h"

SMtkWorld g_mtkWorld;

void MtkWorldInit(void)
{
	s16x3Set(g_mtkWorld.m_velocity, 0, 0, 0);
	s8x3Set(g_mtkWorld.m_angularVelocity, 0, 0, 0);
}

void MtkWorldSetVelocity(const s16x3* velocity)
{
	s16x3Copy(g_mtkWorld.m_velocity, *velocity);
}

void MtkWorldSetAngularVelocity(const s8x3* angularVelocity)
{
	s8x3Copy(g_mtkWorld.m_angularVelocity, *angularVelocity);
}

void MtkWorldMovePosition(s16x3* position)
{
	static s16 px, py, pz;

	px = position->x;
	py = position->y;
	pz = position->z;

	if (g_mtkWorld.m_angularVelocity.x)
	{
		SinCos_SetS16XY(py, pz);
		SinCos_RotateXYS16Hp(-g_mtkWorld.m_angularVelocity.x);
		py = SinCos_GetS16X();
		pz = SinCos_GetS16Y();
	}

	if (g_mtkWorld.m_angularVelocity.y)
	{
		SinCos_SetS16XY(pz, px);
		SinCos_RotateXYS16Hp(-g_mtkWorld.m_angularVelocity.y);
		pz = SinCos_GetS16X();
		px = SinCos_GetS16Y();
	}

	position->x = px - g_mtkWorld.m_velocity.x;
	position->y = py - g_mtkWorld.m_velocity.y;
	position->z = pz - g_mtkWorld.m_velocity.z;
}
