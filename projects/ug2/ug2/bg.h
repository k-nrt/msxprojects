#ifndef BG_H
#define BG_H

#include "vec_math.h"

typedef struct tagBGObject
{
	s16x3 m_v3Position;
} SBGObject;

#define BGOBJECT_MAX (8)
extern SBGObject g_bgObjects[BGOBJECT_MAX];

void BGInitialize();
void BGUpdateAndRender();

#endif //BG_H
