#include <msx-bios-wrapper.h>
#include "vdp_command.h"
#include "flipper.h"

SFlipper g_flipper;
static const SFlipperConfig *s_pConfig = NULL;

void FlipperInit
(
    const SFlipperConfig *pConfig,
    u16 u16Background0GRB,          //< background palette.
    u16 u16Foreground0GRB           //< foreground palette.
)
{
    u8 i;

    s_pConfig = pConfig;
    g_flipper.m_u16Background0GRB = u16Background0GRB;
    g_flipper.m_u16Foreground0GRB = u16Foreground0GRB;

    g_flipper.m_u8DisplayPage = 0;
    g_flipper.m_u8ActivePage = 1;

    g_flipper.m_u8Tile = s_pConfig->m_startColor;
    g_flipper.m_pTile = &s_pConfig->m_tiles[s_pConfig->m_startColor];

    msxBiosChangeColor(6,15,1,1);

    VDPSetActivePage(0);
    VDPSetForegroundColor(s_pConfig->m_clearValue);
    VDPFill(0,0,256,212);
    VDPWait();

    VDPSetActivePage(1);
    VDPSetForegroundColor(s_pConfig->m_clearValue);
    VDPFill(0,0,256,212);
    VDPWait();

    //VDPSetActivePage(0);

    for(i = s_pConfig->m_startColor; i <= s_pConfig->m_endColor; i++)
    {
        VDPPaletteWrite(i, &g_flipper.m_u16Background0GRB, 1);
    }
    VDPPaletteWrite(s_pConfig->m_clearValue & 0x0f, &g_flipper.m_u16Background0GRB, 1);
}

void FlipperTerm(void)
{
    msx2BiosChangeModePalette(5);
    VDPSetActivePage(0);
    VDPSetDisplayPage(0);
    msxBiosChangeColor(6,15,1,7);
}

void FlipperPrint(u16 x, u16 y, u8 color, const char *pszText)
{
    VDPSetForegroundColor(color);
    VDPSetActivePage(0);
    VDPPrint(x,y,pszText);

    VDPSetActivePage(1);
    VDPPrint(x,y,pszText);

    VDPSetActivePage(g_flipper.m_u8ActivePage);
}

void FlipperClear(void)
{
    const SFlipperTile *pTile = g_flipper.m_pTile;
    VDPSetForegroundColor(s_pConfig->m_clearValue);
	VDPFill(pTile->x, pTile->y, pTile->w, pTile->h);
}

void FlipperApplyForegroundColor(void)
{
    VDPSetForegroundColor(g_flipper.m_u8Tile);
}

void FlipperFlip(void)
{
    if (!g_flipper.m_u8ActivePage)
    {
        //. 0 -> 1.
        u8 u8PrevTile = g_flipper.m_u8Tile - 1;
        if (u8PrevTile < s_pConfig->m_startColor)
        {
           u8PrevTile = s_pConfig->m_endColor;
        }

        g_flipper.m_u8ActivePage = 1;
        g_flipper.m_u8DisplayPage = 0;

        VDPWait();
        VDPPaletteWrite(u8PrevTile,&g_flipper.m_u16Background0GRB,1);
        VDPPaletteWrite(g_flipper.m_u8Tile,&g_flipper.m_u16Foreground0GRB,1);
    }
    else
    {
        //. 1 -> 0.
        if (g_flipper.m_u8Tile < s_pConfig->m_endColor)
        {
            g_flipper.m_u8Tile++;
            g_flipper.m_pTile++;
        }
        else
        {
            g_flipper.m_u8Tile = s_pConfig->m_startColor;
            g_flipper.m_pTile = &s_pConfig->m_tiles[s_pConfig->m_startColor];
        }

        g_flipper.m_u8ActivePage = 0;
        g_flipper.m_u8DisplayPage = 1;

    }

    VDPSetDisplayPage(g_flipper.m_u8DisplayPage);
    VDPSetActivePage(g_flipper.m_u8ActivePage);
}
