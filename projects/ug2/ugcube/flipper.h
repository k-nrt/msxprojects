#ifndef FLIPPER_H
#define FLIPPER_H

#include <msx-types.h>

enum EFlipperMode
{
    kFlipperMode_Default,
    kFlipperMode_Palette,
};

typedef struct tFlipperTile
{
    u8 x,y;
    u16 w,h;
} SFlipperTile;

typedef struct tFlipperConfig
{
    u8 m_clearValue;            //< clear value for HMMV.
    u8 m_startColor;            //< start color code to draw.
    u8 m_endColor;              //< end color code to draw.
    SFlipperTile m_screenRect;  //< screen rectangle
    SFlipperTile m_tiles[16];   //< clear tiles.
} SFlipperConfig;

#define FLIPPER_VERTICAL_TILE(_s,_e,_x,_y,_w,_h,_i) \
{(_x), (_y)+((_i-_s)*(_h))/((_e)-(_s)+1), _w, (((_i-_s+1)*(_h))/((_e)-(_s)+1))-(((_i-_s)*(_h))/((_e)-(_s)+1))}

typedef struct tFlipper
{
    u8 m_u8DisplayPage;
    u8 m_u8ActivePage;
    u8 m_u8Tile;
    const SFlipperTile *m_pTile;

    u16 m_u16Background0GRB;    //< background palette.
    u16 m_u16Foreground0GRB;    //< foreground palette.

    SFlipperConfig *m_pConfig;
} SFlipper;
extern SFlipper g_flipper;

extern void FlipperInit
(
    const SFlipperConfig *pConfig,
    u16 u16Background0GRB,          //< background palette.
    u16 u16Foreground0GRB           //< foreground palette.
);

extern void FlipperTerm(void);

extern void FlipperSetForegroundColor(u16 u160GRB);

extern void FlipperPrint(u16 x, u16 y, u8 color, const char *pszText);

//! clear screen.
extern void FlipperClear(void);

//! apply current foreground color code to FORCLR
extern void FlipperApplyForegroundColor(void);

//! flip page and palettes.
extern void FlipperFlip(void);

#endif //FLIPPER_H
