#ifndef INPUT_H
#define INPUT_H

#include <msx-types.h>

typedef struct tagInput
{
	u8 m_u8Buttons;
	u8 m_u8PrevButtons;
} SInput;

extern SInput g_input;

#define INPUT_BUTTON g_input.m_u8Buttons
#define INPUT_PREV_BUTTON g_input.m_u8PrevButtons

#define INPUT_UP		(0x01)
#define INPUT_DOWN		(0x02)
#define INPUT_LEFT		(0x04)
#define INPUT_RIGHT		(0x08)
#define INPUT_TRIGGER1	(0x10)
#define INPUT_TRIGGER2	(0x20)

void InputInitialize();
void InputUpdate();

#define InputIsPush(_button)	(!((_button)&INPUT_PREV_BUTTON) && ((_button)&INPUT_BUTTON))
#define InputIsDown(_button)	(_button&INPUT_BUTTON)
#define InputIsRelease(_button) (((_button)&INPUT_PREV_BUTTON) && !((_button)&INPUT_BUTTON))
#define InputIsUp(_button)		!(_button&INPUT_BUTTON)

#endif //INPUT_H
