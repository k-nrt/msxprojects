#include <msx-bios-wrapper.h>
#include "input.h"

SInput g_input;

void InputInitialize()
{
	g_input.m_u8Buttons = 0;
	g_input.m_u8PrevButtons = 0;
}


void InputUpdate()
{
	register u8 u8Stick = msxBiosGetStick(0);
	register u8 u8Trigger1 = msxBiosGetTrigger(0);

	g_input.m_u8PrevButtons = g_input.m_u8Buttons;
	g_input.m_u8Buttons = 0;
	if (u8Stick)
	{
		if (7 < u8Stick || u8Stick < 3)
		{
			g_input.m_u8Buttons = INPUT_UP;
		}
		else if (3 < u8Stick && u8Stick < 7)
		{
			g_input.m_u8Buttons = INPUT_DOWN;
		}

		if (1 < u8Stick && u8Stick < 5)
		{
			g_input.m_u8Buttons |= INPUT_RIGHT;
		}
		else if (5 < u8Stick)
		{
			g_input.m_u8Buttons |= INPUT_LEFT;
		}
	}

	if (u8Trigger1)
	{
		g_input.m_u8Buttons |= INPUT_TRIGGER1;
	}
}


