#ifndef BIOS_WRAPPER_H
#define BIOS_WRAPPER_H

#if defined(SYSTEM_ROM32K)
#include <msx-bios-wrapper.h>
#define vdpSetScreenModePalette(screenMode) msx2BiosChangeModePalette(screenMode)
#define vdpSetScreenColor(screenMode, foregroundColor, backgroundColor, borderColor) msxBiosChangeColor(screenMode, foregroundColor, backgroundColor, borderColor)
#define vdpPrintChar(c) msx2BiosGraphicPrint(c)

#define inputGetStick(stick) msxBiosGetStick(stick)
#define inputGetTrigger(trigger) msxBiosGetTrigger(trigger)
#define inputGetKeyMatrix(line) msxBiosReadKeyMatrix(line)

#elif defined(SYSTEM_MSXDOS)
#include <msx-dos-vdp.h>
#include <msx-dos-input.h>

#else
#error "Unknown system."
#endif

#endif //BIOS_WRAPPER_H
