#include <msx-bios-wrapper.h>
#include "vdp_command.h"
#include "flipper.h"

SFlipper g_flipper;

void FlipperInit()
{
    u8 i;
    g_flipper.m_u8Frame = 0;

    g_flipper.m_u8DisplayPage = 0;
    g_flipper.m_u8ActivePage = 1;

    g_flipper.m_u16ClearColor = 0x0000;
    g_flipper.m_u16DrawColor = 0x0777;

    msxBiosChangeColor(6,15,1,13);

    VDPSetActivePage(0);
    VDPSetForegroundColor(0x11);
    VDPFill(0,0,256,212);
    VDPWait();

    VDPSetActivePage(1);
    VDPSetForegroundColor(0x11);
    VDPFill(0,0,256,212);
    VDPWait();

    //VDPSetActivePage(0);

    for(i = 0; i < FLIPPER_COLOR_COUNT; i++)
    {
        VDPPaletteWrite(FLIPPER_COLOR_START + i, &g_flipper.m_u16ClearColor, 1);
    }
    //VDPPaletteWrite(FLIPPER_COLOR_START, &g_flipper.m_u16DrawColor, 1);
}

void FlipperTerm()
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

void FlipperClear()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
    u8 u8X = (colorFrame & FLIPPER_COLOR_MASK) << FLIPPER_CLEAR_SHIFT;
    VDPSetForegroundColor(0x11);
	VDPFill(u8X, 32, FLIPPER_CLEAR_WIDTH, 160);
}

void FlipperSetDrawColor()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
	u8 color = FLIPPER_COLOR_START + (colorFrame & FLIPPER_COLOR_MASK);
    VDPSetForegroundColor(color);
}

void FlipperFlip()
{
    u8 colorFrame = g_flipper.m_u8Frame >> 1;
	u8 color = FLIPPER_COLOR_START + (colorFrame & FLIPPER_COLOR_MASK);
    u8 colorPrev = FLIPPER_COLOR_START + ((colorFrame - 1) & FLIPPER_COLOR_MASK);
	VDPWait();
	VDPPaletteWrite(colorPrev,&g_flipper.m_u16ClearColor,1);
	VDPPaletteWrite(color,&g_flipper.m_u16DrawColor,1);
  
    g_flipper.m_u8Frame++;

    g_flipper.m_u8DisplayPage = g_flipper.m_u8Frame & 0x01;
    g_flipper.m_u8ActivePage = g_flipper.m_u8DisplayPage ^ 0x01;

    VDPSetDisplayPage(g_flipper.m_u8DisplayPage);
    VDPSetActivePage(g_flipper.m_u8ActivePage);
}
