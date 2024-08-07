#include "bios_wrapper.h"
#include "mtk_input.h"

#pragma codeseg CODE2

SMtkInput g_mtkInput;
void MtkInputInit(void)
{
	g_mtkInput.m_down = 0;
	g_mtkInput.m_up = 0xff;
	g_mtkInput.m_push = 0;
	g_mtkInput.m_release = 0;
}

void MtkInputScan(void)
{
	u8 down = 0;
	u8 i;

	for (i = 0; i < 2; i++)
	{
		u8 stick = inputGetStick(i);

		if (!stick)
		{
			continue;
		}

		if (stick < 3 || 7 < stick)
		{
			down |= kMtkInput_Up;
		}
		else if (3 < stick && stick < 7)
		{
			down |= kMtkInput_Down;
		}

		if (1 < stick && stick < 5)
		{
			down |= kMtkInput_Right;
		}
		else if (5 < stick)
		{
			down |= kMtkInput_Left;
		}
	}

	{
		u8 trigger = inputGetTrigger(0);
		trigger |= inputGetTrigger(1);
		if (trigger)
		{
			down |= kMtkInput_Fire;
		}
	}

	{
		u8 trigger = inputGetTrigger(3);
		//. shift.
		trigger |= ~inputGetKeyMatrix(6) & 0x01;
		if (trigger)
		{
			down |= kMtkInput_Thrust;
		}
	}

	{
		u8 up = ~down;
		//. down edge.
		g_mtkInput.m_push = (g_mtkInput.m_down ^ down) & down;

		//. up edge.
		g_mtkInput.m_release = (g_mtkInput.m_up ^ up) & up;

		g_mtkInput.m_down = down;
		g_mtkInput.m_up = up;
	}
}
