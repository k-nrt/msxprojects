#include "player.h"
#include "macros.h"
#include "input.h"
#include "sincos.h"

#pragma codeseg CODE2

SPlayer g_player;

void PlayerInitialize()
{
	Vec3Set(g_player.m_v3Position, 0, 0, 0);
	Vec3Set(g_player.m_v3Rotation, 0, 0, 0);
	Vec3Set(g_player.m_v3Velocity, 0, 0, 0);
}

void PlayerUpdate()
{
	register u8 urdl = g_input.m_u8Buttons;

	if (urdl & INPUT_UP)
	{
		g_player.m_v3Position.y -= 1;
		g_player.m_v3Rotation.x += 4;
	}
	else if (urdl & INPUT_DOWN)
	{
		g_player.m_v3Position.y += 1;
		g_player.m_v3Rotation.x -= 4;
	}
	else
	{
		g_player.m_v3Rotation.x += (g_player.m_v3Rotation.x < 0) ? 4 : 0;
		g_player.m_v3Rotation.x -= (0 < g_player.m_v3Rotation.x) ? 4 : 0;
	}

	if (urdl & INPUT_RIGHT)
	{
		g_player.m_v3Position.x += 1;
		g_player.m_v3Rotation.y += 4;
		g_player.m_v3Rotation.z += 2;
	}
	else if (urdl & INPUT_LEFT)
	{
		g_player.m_v3Position.x -= 1;
		g_player.m_v3Rotation.y -= 4;
		g_player.m_v3Rotation.z -= 2;
	}
	else
	{
		g_player.m_v3Rotation.z += (g_player.m_v3Rotation.z < 0) ? 2 : 0;
		g_player.m_v3Rotation.z -= (0 < g_player.m_v3Rotation.z) ? 2 : 0;

		g_player.m_v3Rotation.y += (g_player.m_v3Rotation.y < 0) ? 4 : 0;
		g_player.m_v3Rotation.y -= (0 < g_player.m_v3Rotation.y) ? 4 : 0;
	}

	g_player.m_v3Rotation.z = CLAMP(g_player.m_v3Rotation.z, -VIEW_ROTATION_Z_MAX, VIEW_ROTATION_Z_MAX);

	g_player.m_v3Rotation.x = CLAMP(g_player.m_v3Rotation.x, -16, 16);
	g_player.m_v3Rotation.y = CLAMP(g_player.m_v3Rotation.y, -16, 16);

	{
		const s8x3 velocity = { 0, 0, 64 };
		register s16 sinX = SinCos_GetSin(g_player.m_v3Rotation.x);
		register s16 cosX = SinCos_GetCos(g_player.m_v3Rotation.x);
		register s16 sinY = SinCos_GetSin(g_player.m_v3Rotation.y);
		register s16 cosY = SinCos_GetCos(g_player.m_v3Rotation.y);
		register s16 xx, yy, zz;
		
		xx = velocity.x;
		yy = SinCos_RotateXS8x256(velocity.y, velocity.z, sinX, cosX) >> 8;
		zz = SinCos_RotateYS8x256(velocity.y, velocity.z, sinX, cosX) >> 8;

		g_player.m_v3Velocity.x = SinCos_RotateYS8x256((s8)zz, (s8)xx, sinY, cosY) >> 8;
		g_player.m_v3Velocity.y = yy;
		g_player.m_v3Velocity.z = SinCos_RotateXS8x256((s8)zz, (s8)xx, sinY, cosY) >> 8;
	}

	g_player.m_v3Position.x += g_player.m_v3Velocity.x >> 2;
	g_player.m_v3Position.y += g_player.m_v3Velocity.y >> 2;

	g_player.m_v3Position.x = CLAMP(g_player.m_v3Position.x, -128, 128);
	g_player.m_v3Position.y = CLAMP(g_player.m_v3Position.y, -64, 64);

	//. モデルパターン選択.
	g_player.m_u8ModelRotX = (u8)CLAMP((((s8)g_player.m_v3Rotation.x) >> 2) + 4, 0, 9);
	g_player.m_u8ModelRotY = (u8)CLAMP((((s8)g_player.m_v3Rotation.y) >> 2) + 4, 0, 9);
}
