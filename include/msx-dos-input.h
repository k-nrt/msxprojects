//. MSX-DOS Input BIOS wrapper. 
#pragma once
#include <msx-types.h>

//. Game input.
extern u8 inputGetStick(u8 stick);
extern u8 inputGetTrigger(u8 trigger);

//. Keybord.
extern u8 inputGetKeyMatrix(u8 line);
