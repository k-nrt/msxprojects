//. MSX-DOS Function call wrapper.
#pragma once
#include <msx-types.h>

typedef struct msxDosFCB_
{
	u8 drive;
	char fileName[8];
	char extension[3];
	u16 currentBlock;
	u16 recordSize;
	u32 fileSize;
	u16 day;
	u16 time;
	u8 deviceId;
	u8 directoryLocation;
	u16 startCluster;
	u16 lastCluster;
	u16 clusterLocation;
	u8 currentRecord;
	u32 randomRecord;
} msxDosFCB;

extern msxDosFCB msxDosDefaultFCB0;
extern msxDosFCB msxDosDefaultFCB1;

//. Function call.
extern void msxDosReset();

extern void msxDosPrintChar(char c);
extern void msxDosPrint(const char *str);
extern void msxDosPrintHexU8(u8 hex);
extern void msxDosPrintHexU16(u16 hex);
extern void msxDosPrintHexU32(u32 hex);

extern u8 msxDosOpen(msxDosFCB *fcb);
extern u8 msxDosSetReadAddress(void* address);
extern u8 msxDosReadRandomRecord(msxDosFCB *fcb, u16 recordSize);
extern u8 msxDosClose(msxDosFCB *fcb);

//. set page0 to Main ROM and execute.
extern void msxDosRun(void* run);

//. copy.
extern void msxMemCopy(void* to, const void* from, u16 size);

//. clear key buffer.
extern void msxClearKeyBuffer();
