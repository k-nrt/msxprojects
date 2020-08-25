#ifndef FLIPPER_H
#define FLIPPER_H

#include <msx-types.h>

enum EFlipperMode
{
    kFlipperMode_Default,
    kFlipperMode_Palette,
};

typedef struct tFlipper
{
    u8 m_u8Frame;
    u8 m_u8DisplayPage;
    u8 m_u8ActivePage;
    u16 m_u16ClearColor;
    u16 m_u16DrawColor;

} SFlipper;
extern SFlipper g_flipper;

#define FLIPPER_COLOR_START 2
#define FLIPPER_COLOR_COUNT 8
#define FLIPPER_COLOR_MASK  0x07
#define FLIPPER_CLEAR_WIDTH 32
#define FLIPPER_CLEAR_SHIFT 5

extern void FlipperInit();
extern void FlipperTerm();
extern void FlipperPrint(u16 x, u16 y, u8 color, const char *pszText);

extern void FlipperClear();
extern void FlipperSetDrawColor();
extern void FlipperFlip();

#endif //FLIPPER_H
