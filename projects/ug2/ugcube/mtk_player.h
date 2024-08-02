#ifndef MTK_PLAYER_H
#define MTK_PLAYER_H

#include "vec_math.h"

typedef struct tMtkPlayer
{
	s16x3 m_velocity;
} SMtkPlayer;
extern SMtkPlayer g_mtkPlayer;

extern void MtkPlayerInit(void);
extern void MtkPlayerUpdate(void);

#endif //MTK_PLAYER_H
