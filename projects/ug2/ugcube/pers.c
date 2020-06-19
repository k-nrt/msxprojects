#include "macros.h"
#include "pers.h"
#include "vdp_command.h"

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


void PersInit()
{
    s8x3Set(g_persContext.m_v3Position,0,0,64);
    g_persContext.m_s8ClipNear = 32;
    g_persContext.m_s16ScreenZ = 80;
    g_persContext.m_s16ScreenX = 128;
    g_persContext.m_s16ScreenY = 104;
    g_persContext.m_viewPort.m_left   = 0;
    g_persContext.m_viewPort.m_right  = 255;
    g_persContext.m_viewPort.m_top    = 0;
    g_persContext.m_viewPort.m_bottom = 208;

    g_persContext.m_vramHigh1 = 1;
    g_persContext.m_vramLow16 = 0;

    PersMakeTransformTable(160,32);
    PersMakeTrandformTable2();
}

static void PersMakeTrandformTable2()
{
    u8 z;
    s16 s16ScreenZ = (g_persContext.m_s16ScreenZ) << 8;
    u16 *pRcpZ = s_u16TransformRcpZ;
    u8 clipNear = (u8) g_persContext.m_s8ClipNear;

    for (z = 0; z < 128; z++)
    {
        s16 preRcpZ = 0;
        if (clipNear <= z)
        {
            preRcpZ = s16ScreenZ/((s16)z);
        }
        *pRcpZ = (u16) preRcpZ;
        pRcpZ++;
    }
}

void PersMakeTransformTable(s16 s16ScreenZ, s8 s8ClipNear)
{
    u8 x, z;
    u8 *pValues     = s_u8TransformPositions;
    u8 *pLeft       = s_u8TransformLeft;
    u8 *pRight      = s_u8TransformRight;
    u8 *pTop        = s_u8TransformTop;
    u8 *pBottom     = s_u8TransformBottom;
    u16 *pPositions = s_u16TransformPointers;
    const s16 kLeft   = g_persContext.m_s16ScreenX - g_persContext.m_viewPort.m_left - 0;
    const s16 kRight  = g_persContext.m_viewPort.m_right - g_persContext.m_s16ScreenX - 0;
    const s16 kTop    = g_persContext.m_s16ScreenY - g_persContext.m_viewPort.m_top - 0;
    const s16 kBottom = g_persContext.m_viewPort.m_bottom - g_persContext.m_s16ScreenY - 0;
    const s16 kMax    = MAX(MAX(kLeft,kRight),MAX(kTop,kBottom));

    g_persContext.m_s8ClipNear = s8ClipNear;
    g_persContext.m_s16ScreenZ = s16ScreenZ;

    for (z = 0; z < 128; z++)
    {
        VDPPrintU16X(0,8,(u16)pValues);
        if (z < (u8)s8ClipNear)
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

const SPersScreenPos* PersGetPostions()
{
    return s_screenPositions;
}