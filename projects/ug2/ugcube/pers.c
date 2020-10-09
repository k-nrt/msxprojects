#include "macros.h"
#include "pers.h"
#include "vdp_command.h"
#include "clip.h"

SPersContext g_persContext;

//. transformed internal vertex buffer.
#define PERS_MAX_VERTICES (64)
SDCC_FIXED_ADDRESS(PersScreenPositionsAddress) SPersScreenPos s_screenPositions[PERS_MAX_VERTICES];

//. transform position array pointers per z.
SDCC_FIXED_ADDRESS(PersTransform_Pointers)   u16 s_u16TransformPointers[128];

//. transform position array element count per z.
SDCC_FIXED_ADDRESS(PersTransform_Left)     u8  s_u8TransformLeft[128];
SDCC_FIXED_ADDRESS(PersTransform_Right)    u8  s_u8TransformRight[128];
SDCC_FIXED_ADDRESS(PersTransform_Top)      u8  s_u8TransformTop[128];
SDCC_FIXED_ADDRESS(PersTransform_Bottom)   u8  s_u8TransformBottom[128];

//. pre multiprecated screenz/z
SDCC_FIXED_ADDRESS(PersTransform_RcpZ)     u16  s_u16TransformRcpZ[128];

//. transform positions.
SDCC_FIXED_ADDRESS(PersTransform_Positions)  u8  s_u8TransformPositions[];

static void PersMakeTransformTable();
static void PersMakeTransformRcpZTable();

void PersInit
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
)
{
    s8x3Set(g_persContext.m_v3Position,0,0,64);
    g_persContext.m_s8ClipNear = s8ClipNear;//32;
    g_persContext.m_s16ScreenZ = s16ScreenZ; //80;
    g_persContext.m_s16ScreenX = s16ScreenCenterX; //128;
    g_persContext.m_s16ScreenY = s16ScreenCenterY; //104;
    g_persContext.m_viewPort.m_left   = s16Left; //0;
    g_persContext.m_viewPort.m_right  = s16Right; //255;
    g_persContext.m_viewPort.m_top    = s16Top; //0;
    g_persContext.m_viewPort.m_bottom = s16Bottom; //208;
    g_persContext.m_pProgress = pProgress;

    g_persContext.m_vramHigh1 = 1;
    g_persContext.m_vramLow16 = 0;

    PersMakeTransformTable();
    PersMakeTransformRcpZTable();
}

static void PersMakeTransformRcpZTable()
{
    u8 z;
    u16 u16ScreenZ = ((u16)g_persContext.m_s16ScreenZ) << 8;
    u16 *pRcpZ = s_u16TransformRcpZ;
    u8 clipNear = (u8) g_persContext.m_s8ClipNear;

    for (z = 0; z < 128; z++)
    {
        u16 preRcpZ = 0;
        if (clipNear <= z)
        {
            preRcpZ = u16ScreenZ/((u16)z);
        }
        *pRcpZ = preRcpZ;
        pRcpZ++;
    }
}

void PersMakeTransformTable()
{
    const s8 s8ClipNear = g_persContext.m_s8ClipNear;
    const s16 s16ScreenX = g_persContext.m_s16ScreenX;
    const s16 s16ScreenY = g_persContext.m_s16ScreenY;
    const s16 s16ScreenZ = g_persContext.m_s16ScreenZ;
    const s16 s16Left = g_persContext.m_viewPort.m_left;
    const s16 s16Right = g_persContext.m_viewPort.m_right;
    const s16 s16Top = g_persContext.m_viewPort.m_top;
    const s16 s16Bottom = g_persContext.m_viewPort.m_bottom;
    u8 x, z;
    u8 *pValues     = s_u8TransformPositions;
    u8 *pLeft       = s_u8TransformLeft;
    u8 *pRight      = s_u8TransformRight;
    u8 *pTop        = s_u8TransformTop;
    u8 *pBottom     = s_u8TransformBottom;
    u16 *pPositions = s_u16TransformPointers;
    const s16 kLeft   = s16ScreenX - s16Left - 0;
    const s16 kRight  = s16Right - s16ScreenX - 0;
    const s16 kTop    = s16ScreenY - s16Top - 0;
    const s16 kBottom = s16Bottom - s16ScreenY - 0;
    const s16 kMax    = MAX(MAX(kLeft,kRight),MAX(kTop,kBottom));

    for (z = 0; z < 128; z++)
    {
        if (g_persContext.m_pProgress)
        {
            (*g_persContext.m_pProgress)((u16)pValues);
        }

        if (z < 1)
        {
            //. near clip.
            *pLeft = 0;
            *pRight = 0;
            *pTop = 0;
            *pBottom = 0;
            *pPositions = 0;
        }
        else if (PersTransform_Positions_End <= (u16)pValues)
        {
            //. position buffer over run.
            *pRight = 0;
            *pTop = 0;
            *pBottom = 0;
            *pPositions = 0;
        }
        else
        {
            s16 zz = (s16) z;
            u8 left = 0;
            u8 right = 0;
            u8 top = 0;
            u8 bottom = 0;
            *pPositions = (u16) pValues;

            for(x = 0; x < 128; x++)
            {
                s16 xx = (s16)x;
                xx = (s16ScreenZ*xx)/zz;
                if(xx < kLeft)
                {
                    left++;
                }

                if (xx < kRight)
                {
                    right++;
                }

                if (xx < kTop)
                {
                    top++;
                }

                if (xx < kBottom)
                {
                    bottom++;
                }

                if (xx < kMax)
                {
                    *pValues = (u8) xx;
                    pValues++;
                    if(PersTransform_Positions_End <= (u16)pValues)
                    {
                        //. buffer over run.
                        break;
                    }
                }
                else
                {
                    //. clip. 
                    break;
                }
            }
            *pLeft   = left ? left - 1 : left;
            *pRight  = right ? right - 1 : right;
            *pTop    = top ? top - 1 : top;
            *pBottom = bottom ? bottom - 1 : bottom;
        }
        pLeft++;
        pRight++;
        pTop++;
        pBottom++;
        pPositions++;
    }
}


u16 PersRegisterVertices(s8x3 *pVertices, u8 nbVertices, u8 rx, u8 ry, u8 rz, u8 shift)
{
    u8 i;
    s8x3 vertex;
    s8x3 *pVertex = pVertices;
    u16 result = g_persContext.m_vramLow16;

    for (i = 0; i < nbVertices; i++)
    {
        s8 x = pVertex->x;
        s8 y = pVertex->y;
        s8 z = pVertex->z;

        if (rx)
        {
            s8XY yz = SinCos_RotateXYS8Lp(y,z,rx);
            y = s8XY_GetX(yz);
            z = s8XY_GetY(yz);
        }

        if (ry)
        {
            s8XY zx = SinCos_RotateXYS8Lp(z,x,ry);
            z = s8XY_GetX(zx);
            x = s8XY_GetY(zx);
        }

        if (rz)
        {
            s8XY xy = SinCos_RotateXYS8Lp(x,y,rz);
            x = s8XY_GetX(xy);
            y = s8XY_GetY(xy);
        }

        if (shift)
        {
            x >>= shift;
            y >>= shift;
            z >>= shift;
        }

        vertex.x = x;
        vertex.y = y;
        vertex.z = z;

        VDPWriteBytes(g_persContext.m_vramHigh1,g_persContext.m_vramLow16, &vertex, sizeof(s8x3));
        g_persContext.m_vramLow16 += sizeof(s8x3);
        pVertex++;
    }

    return result;
}


u8 PersSetVertices(s8x3 *pVertices, u8 nbVertices)
{
    SPersScreenPos *pScreen = s_screenPositions;
    s8x3 *pVertex = pVertices;
    u8 modelClipBits = 0;
    u8 i;

    for(i = 0; i < nbVertices; i++)
    {
        u8 clipBit = 0;
        s16 z = (s16) pVertex->z + (s16) g_persContext.m_v3Position.z;
        if (z < g_persContext.m_s8ClipNear)
        {
            clipBit = kClipBit_Near;
        }
        else if(128 <= z)
        {
            clipBit = kClipBit_Far;
        }
        else
        {
            s16 x = (s16) pVertex->x + (s16) g_persContext.m_v3Position.x;
            s16 y = (s16) pVertex->y + (s16) g_persContext.m_v3Position.y;
            const s16 kLeft   = - (s16) s_u8TransformLeft[z];
            const s16 kRight  =   (s16) s_u8TransformRight[z];
            const s16 kTop    = - (s16) s_u8TransformTop[z];
            const s16 kBottom =   (s16) s_u8TransformBottom[z];
            if (x < kLeft)
            {
                clipBit = kClipBit_Left;
            }
            else if (kRight < x)
            {
                clipBit = kClipBit_Right;
            }
            
            if (y < kTop)
            {   
                clipBit |= kClipBit_Top;

            }
            else if (kBottom < y)
            {
                clipBit |= kClipBit_Bottom;
            }

            if (!clipBit)
            {
                u8 *pTransforms = (u8*) (s_u16TransformPointers[z]);
                if (x < 0)
                {
                    x = -((s16)(pTransforms[-x]));
                }
                else if (0 < x)
                {
                    x = ((s16)(pTransforms[x]));
                }
                else
                {
                    x = 0;
                }
                x += g_persContext.m_s16ScreenX;
                pScreen->m_x = x;
                         
                if (y < 0)
                {
                    y = -((s16)(pTransforms[-y]));
                }
                else if (0 < y)
                {
                    y = ((s16)(pTransforms[y]));
                }
                else
                {
                    y = 0;
                }

                y += g_persContext.m_s16ScreenY;
                pScreen->m_y = (u8)y;
            }
        }
        pScreen->m_clipBits = clipBit;
        modelClipBits |= clipBit;

        pVertex++;
        pScreen++;
    }

    return modelClipBits;
}

void PersSetViewport(s16 left, s16 top, s16 right, s16 bottom)
{
    g_persContext.m_viewPort.m_left = left;
    g_persContext.m_viewPort.m_top  = top;
    g_persContext.m_viewPort.m_right = right;
    g_persContext.m_viewPort.m_bottom = bottom;
    Clip_SetRect(left,right,top,bottom);
}

const SPersScreenPos* PersGetPostions()
{
    return s_screenPositions;
}

#if 0
void PersDrawLinesClipXY(const u16* pLines, u8 nbLines)
{
    const u16 *pLine = pLines;
    u8 i;
    for (i = 0; i < nbLines; i++)
    {
        const SPersScreenPos *pPos0 = (SPersScreenPos*)(*pLine);
        const SPersScreenPos *pPos1 = (SPersScreenPos*)(*(pLine+1));
        const u8 clipBitsOr  = pPos0->m_clipBits | pPos1->m_clipBits;
        const u8 clipBitsAnd = pPos0->m_clipBits & pPos1->m_clipBits;

        if(clipBitsAnd | (clipBitsOr & (kClipBit_Near|kClipBit_Far)))
        {

        }
        else if(!clipBitsOr)
        {
            VDPWaitLine(pPos0->m_x, pPos0->m_y, pPos1->m_x, pPos1->m_y);
        }
        else
        {
            s16 sx,sy,ex,ey;
            if(pPos0->m_clipBits & (kClipBit_Left|kClipBit_Right))
            {
                sx = (s16)((((u16)pPos0->m_xHigh)<<8) | ((u16)pPos0->m_x));
            }
            else
            {
                sx = (s16)((u16)pPos0->m_x);
            }

            if(pPos0->m_clipBits & (kClipBit_Top|kClipBit_Bottom))
            {
                sy = (s16)((((u16)pPos0->m_yHigh)<<8) | ((u16)pPos0->m_y));
            }
            else
            {
                sy = (s16)((u16)pPos0->m_y);
            }

            if(pPos1->m_clipBits & (kClipBit_Left|kClipBit_Right))
            {
                ex = (s16)((((u16)pPos1->m_xHigh)<<8) | ((u16)pPos1->m_x));
            }
            else
            {
                ex = (s16)((u16)pPos1->m_x);
            }
            
            if(pPos1->m_clipBits & (kClipBit_Top|kClipBit_Bottom))
            {
                ey = (s16)((((u16)pPos1->m_yHigh)<<8) | ((u16)pPos1->m_y));
            }
            else
            {
                ey = (s16)((u16)pPos1->m_y);
            }

            Clip_SetLine(sx,sy,ex,ey);
            ClipRect_VDPWaitLine();
        }

        pLine += 2;
    }
}
#endif

void PersDrawLinesClipXYZ(const u16* pLines, u8 nbLines)
{
    const u16 *pLine = pLines;
    u8 i;
    for (i = 0; i < nbLines; i++)
    {
        const SPersScreenPos *pPos0 = (SPersScreenPos*)(*pLine);
        const SPersScreenPos *pPos1 = (SPersScreenPos*)(*(pLine+1));
        const u8 clipBitsOr  = pPos0->m_clipBits | pPos1->m_clipBits;
        const u8 clipBitsAnd = pPos0->m_clipBits & pPos1->m_clipBits;

        if(clipBitsAnd)
        {

        }
        else if(!clipBitsOr)
        {
            VDPWaitLine(pPos0->m_x, pPos0->m_y, pPos1->m_x, pPos1->m_y);
        }
        else if(clipBitsOr & (kClipBit_Near|kClipBit_Far))
        {
            s8 clipLine;
            s8x3Set(g_clipLineS8x3.m_v3Start, pPos0->m_3dx, pPos0->m_3dy, pPos0->m_3dz);
            s8x3Set(g_clipLineS8x3.m_v3End,   pPos1->m_3dx, pPos1->m_3dy, pPos1->m_3dz);

            clipLine = ClipLineS8x3(g_persContext.m_s8ClipNear);

            if (0 <= clipLine)
            {
                u8 clip0 = PersTransformViewPosition(&g_clipLineS8x3.m_v3Start,(s16x2*)&g_clipLineInOut.sx);
                u8 clip1 = PersTransformViewPosition(&g_clipLineS8x3.m_v3End,  (s16x2*)&g_clipLineInOut.ex);
                u8 clipOr = clip0 | clip1;
                u8 clipAnd = clip0 & clip1;

                if (!clipOr)
                {
                    Clip_VDPWaitLine();
                }
                else if(!clipAnd)
                {
                    ClipRect_VDPWaitLine();
                }
            }
        }
        else
        {
            s16 sx,sy,ex,ey;
            if(pPos0->m_clipBits & (kClipBit_Left|kClipBit_Right))
            {
                sx = (s16)((((u16)pPos0->m_xHigh)<<8) | ((u16)pPos0->m_x));
            }
            else
            {
                sx = (s16)((u16)pPos0->m_x);
            }

            if(pPos0->m_clipBits & (kClipBit_Top|kClipBit_Bottom))
            {
                sy = (s16)((((u16)pPos0->m_yHigh)<<8) | ((u16)pPos0->m_y));
            }
            else
            {
                sy = (s16)((u16)pPos0->m_y);
            }

            if(pPos1->m_clipBits & (kClipBit_Left|kClipBit_Right))
            {
                ex = (s16)((((u16)pPos1->m_xHigh)<<8) | ((u16)pPos1->m_x));
            }
            else
            {
                ex = (s16)((u16)pPos1->m_x);
            }
            
            if(pPos1->m_clipBits & (kClipBit_Top|kClipBit_Bottom))
            {
                ey = (s16)((((u16)pPos1->m_yHigh)<<8) | ((u16)pPos1->m_y));
            }
            else
            {
                ey = (s16)((u16)pPos1->m_y);
            }

            Clip_SetLine(sx,sy,ex,ey);
            ClipRect_VDPWaitLine();
        }

        pLine += 2;
    }
}