#include "slot.h"

/*
void IntDisable()
{
	__asm
	di
	__endasm;
}

void IntEnable()
{
	__asm
	ei
	__endasm;
}
*/

u8 SLOTTestMemory(u8 slot, u16 baseAddress)
{
	u16 offset = 0;
	for (offset = 0; offset < 16384; offset += 1024)
	{
		u8 value, invValue;
		u16 address = baseAddress + offset;
		value = SLOTReadDI(slot, address);
		invValue = ~value;
		SLOTWriteDI(slot, address, invValue);
		value = SLOTReadDI(slot, address);
		if (value != invValue)
		{
			return FALSE;
		}
	}

	return TRUE;
}

u8 SLOTFindRAMSlot(u16 address)
{
	u8 *EXPTBL = (u8*)0xFCC1;
	u8 b, e;
	for (b = 0; b < 4; b++)
	{
		if (EXPTBL[b] & 0x80)
		{
			for (e = 0; e < 4; e++)
			{
				u8 slot = b | (e << 2);
				if (SLOTTestMemory(slot, address))
				{
					return slot;
				}
			}
		}
		else
		{
			if (SLOTTestMemory(b, address))
			{
				return b;
			}
		}
	}

	INT_ENABLE();

	return 0xff;
}

