//. MSX-DOS PSG BIOS wrapper.
#pragma once
#include <msx-types.h>

extern void psgWrite(u8 register, u8 data);
extern u8 psgRead(u8 register);

