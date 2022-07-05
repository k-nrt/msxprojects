#ifndef SLOT_H
#define SLOT_H

#include <msx-types.h>

#ifndef __INTELLISENSE__
#define INT_DISABLE() { __asm di __endasm; }
#define INT_ENABLE() { __asm ei __endasm; }
#else
#define INT_DISABLE()
#define INT_ENABLE()
#endif //__INTELLISENSE__

extern u8 SLOTReadDI(u8 iSlot, u16 nAddress) SDCCCALL(0);
extern void SLOTWriteDI(u8 iSlot, u16 nAddress, u8 nParam) SDCCCALL(0);
extern void SLOTEnableDI(u8 iSlot, u16 nAddress)  SDCCCALL(0);


typedef u8 SlotInfo;

extern SlotInfo SLOTGetSlotInfo(u16 address) SDCCCALL(0);
#define SlotInfo_HasExtension(_info)  (_info & 0x80)
#define SlotInfo_PrimarySlot(_info)   (_info & 0x03)
#define SlotInfo_ExtensionSlot(_info) ((_info >> 2)& 0x03)

extern u8 SLOTTestMemory(u8 slot, u16 baseAddress);
extern u8 SLOTFindRAMSlot(u16 address);

#endif //SLOT_H
