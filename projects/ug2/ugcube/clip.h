#ifndef CLIP_H
#define CLIP_H

#include <msx-types.h>
#include "vec2.h"

typedef struct
{
    s16 m_s16Left;
    s16 m_s16Right;
    s16 m_s16Top;
    s16 m_s16Bottom;
} SClipRect;

extern SClipRect g_clipRect;

typedef struct
{
    s16 sx, sy, ex, ey;
} SLine16;


extern SLine16 g_clipLineInOut;

#define Clip_SetRect(left,right,top,bottom) \
    g_clipRect.m_s16Left   = left;\
    g_clipRect.m_s16Right  = right;\
    g_clipRect.m_s16Top    = top;\
    g_clipRect.m_s16Bottom = bottom

#define Clip_SetLine(SX,SY,EX,EY) \
    g_clipLineInOut.sx = SX;\
    g_clipLineInOut.sy = SY;\
    g_clipLineInOut.ex = EX;\
    g_clipLineInOut.ey = EY

#define Clip_VDPWaitLine() VDPWaitLine(g_clipLineInOut.sx, g_clipLineInOut.sy, g_clipLineInOut.ex, g_clipLineInOut.ey)

s8 ClipLeft();
s8 ClipRight();
s8 ClipTop();
s8 ClipBottom();



#endif //CLIP_H