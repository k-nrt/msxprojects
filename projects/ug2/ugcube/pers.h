#ifndef PERS_H
#define PERS_H

#include <msx-types.h>
#include "vec_math.h"
#include "vec2.h"
#include "bbox.h"


enum EClipBit
{
    kClipBit_Near   = 0x01,
    kClipBit_Far    = 0x02,
    kClipBit_Left   = 0x04,
    kClipBit_Right  = 0x08,
    kClipBit_Top    = 0x10,
    kClipBit_Bottom = 0x20,
    kClipBit_16     = 0x80,
};


enum EBBoxClip
{
    kBBoxClip_Out        = 0,
    kBBoxClip_In         = 1,
    kBBoxClip_ScissorXY  = 2,
    kBBoxClip_ScissorXYZ = 3,
};


typedef struct tPersScreenPos
{
    u8 m_clipBits;      //< combination of EClipBit
    u8 m_x;             //< unsigned low 2D X or signed low 2D X for XY clipping
    u8 m_y;             //< unsigned low 2D Y or signed low 2D Y for XY clipping
    u8 m_xHigh;         //< signed high 2D X for XY clipping
    u8 m_yHigh;         //< signed high 2D Y for XY clippimg
    s8 m_3dx;           //< 3D X for near/far clipping
    s8 m_3dy;           //< 3D Y for near/far clipping
    s8 m_3dz;           //< 3D Z for near/far clipping
} SPersScreenPos;

typedef struct tPersViewport
{
    s16 m_left;
    s16 m_right;
    s16 m_top;
    s16 m_bottom;
} SPersViewport;

typedef struct tPersContext
{
    s8x3 m_v3Position;
    s8   m_s8ClipNear;
    s16  m_s16ScreenZ;
    s16  m_s16ScreenX;
    s16  m_s16ScreenY;
    SPersViewport m_viewPort;
    u8   m_vramHigh1;
    u16  m_vramLow16;
    
    void (*m_pProgress)(u16 address);
} SPersContext;

extern SPersContext g_persContext;

extern void PersInit
(
    s8 s8ClipNear,
    s16 s16ScreenZ,
    s16 s16ScreenCenterX,
    s16 s16ScreenCenterY,
    s16 s16Left,
    s16 s16Right,
    s16 s16Top,
    s16 s16Bottom,
    void (*pProgress)(u16)
);


extern u16 PersRegisterVertices(s8x3 *pVertices, u8 nbVertices, u8 rx, u8 ry, u8 rz, u8 shift);
extern u8 PersTransformNoClipVram(u16 vramOffset, u8 nbVertices) SDCCCALL(0);
extern void PersTransformClipXYVram(u16 vramOffset, u8 nbVertices) SDCCCALL(0);
extern void PersTransformClipXYZVram(u16 vramOffset, u8 nbVertices) SDCCCALL(0);
extern u8 PersSetVertices(s8x3 *pVertices, u8 nbVertices);

extern u8 PersTransformViewPosition(s8x3 *pViewPosition, s16x2 *pScreenPosition) SDCCCALL(0);

extern void PersDrawLines(const u16* pLines, u8 nbLines) SDCCCALL(0);
extern void PersDrawLinesClipXY(const u16* pLines, u8 nbLines) SDCCCALL(0);
extern void PersDrawLinesClipXYZ(const u16* pLines, u8 nbLines);

extern u8 PersClipPoint(s8 x, s8 y, s8 z, s8 near) SDCCCALL(0);
extern u8 PersClipBBox(const SBBox *pBBox, s8 x, s8 y, s8 z, s8 near) SDCCCALL(0);

extern u16 PersCreateBBox(u16 vramOffset, u8 nbVertices) SDCCCALL(0);
extern u8 PersClipBBoxVram(u16 vramOffset, s8 x, s8 y, s8 z, s8 near) SDCCCALL(0);

extern const SPersScreenPos* PersGetPostions(void);

#define PersTransform_Pointers     (0xC000) //. 2*128 entries.
#define PersTransform_Left         (0xC100) //. 1*128 entries.
#define PersTransform_Right        (0xC180) //. 1*128 entries.
#define PersTransform_Top          (0xC200) //. 1*128 entries.
#define PersTransform_Bottom       (0xC280) //. 1*128 entries.

#define PersTransform_Positions         (0xC300) //. 0xC300 - 0xDDFF
#define PersTransform_Positions_End     (0xDD00)
#define PersTransform_RcpZ              (0xDD00) // 2*128 entries
#define PersScreenPositionsAddress      (0xDE00) //. internal vertex buffer 8*64 entries.


#define PersSetPosition(inX,inY,inZ) s8x3Set(g_persContext.m_v3Position,inX,inY,inZ)
#define PersSetVertexBuffer(inHigh1,inLow16) g_persContext.m_vramHigh1=inHigh1;g_persContext.m_vramLow16=inLow16 
#endif //PERS_H
