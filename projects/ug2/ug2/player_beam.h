#ifndef PLAYER_BEAM_H
#define PLAYER_BEAM_H

#include <msx-types.h>
#include "vec_math.h"
#include "pers.h"

typedef struct tagPlayerBeam
{
	s16x3 m_v3Position;
	s16x3 m_v3Velocity;
	UgxBuffer m_bufferVertices;
	u8 m_liveCount;
	u8 m_pad0;
} SPlayerBeam;

#define PLAYER_BEAM_MAX (3)
extern SPlayerBeam g_playerBeams[PLAYER_BEAM_MAX];

void PlayerBeamInitialize();
void PlayerBeamSpawn();
void PlayerBeamUpdateAndRender();






#endif //PLAYER_BEAM_H
