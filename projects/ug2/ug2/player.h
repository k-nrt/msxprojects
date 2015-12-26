#ifndef PLAYER_H
#define PLAYER_H

#include "vec_math.h"

typedef struct tagPlayer
{
	s16x3 m_v3Position;
	s16x3 m_v3Velocity;
	s8x3 m_v3Rotation;
	u8 m_u8ModelRotX;
	u8 m_u8ModelRotY;
} SPlayer;

extern SPlayer g_player;

void PlayerInitialize();
void PlayerUpdate();

#endif //PLAYER_H
