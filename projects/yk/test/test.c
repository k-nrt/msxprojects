#define UNUSED(var) ((void)var)
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

struct FCB
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
};
struct FCB *defaultFCB = (struct FCB*)(0x005C);
struct FCB *defaultSecondaryFCB = (struct FCB*)(0x006C);

void PrintChar(char c)
{
	UNUSED(c);
	__asm
		ld		e,a
		ld		c, #0x02
		call	#0x0005
	__endasm;
}

void Print(const char *str)
{
	for(;*str != '\0'; str++)
	{
		PrintChar(*str);
	}
}


void PrintHexU8(u8 hex)
{
	const static char toHex[] = "0123456789ABCDEF";
	PrintChar(toHex[(hex >> 4) & 0xf]);
	PrintChar(toHex[hex & 0xf]);
}

void PrintHexU16(u16 hex)
{
	PrintHexU8((u8)(hex >> 8));
	PrintHexU8((u8)(hex));
}

void PrintHexU32(u32 hex)
{
	PrintHexU8((u8)(hex >> 24));
	PrintHexU8((u8)(hex >> 16));
	PrintHexU8((u8)(hex >> 8));
	PrintHexU8((u8)(hex));
}

u8 Open(struct FCB *fcb)
{
	UNUSED(fcb);
	__asm
		ex		de,hl
		ld		c,#0x0f
		call	#0x0005
	__endasm;
}

u8 SetReadAddress(void* address)
{
	__asm
		ex		de,hl
		ld		c,#0x1a
		call	#0x0005
	__endasm;
}
u8 ReadRandomRecord(struct FCB *fcb, u16 recordSize)
{
	__asm
		ex		de,hl
		ld		c,#0x27
		call	#0x0005
	__endasm;
}

u8 Close(struct FCB *fcb)
{
	UNUSED(fcb);
	__asm
		ex		de,hl
		ld		c,#0x10
		call	#0x0005
	__endasm;
}

struct BloadHeader
{
	u8 id;
	u16 start;
	u16 end;
	u16 run;
};

u8 BloadHeaderGetId(struct BloadHeader* bloadHeaader)
{

}
struct BloadHeader g_header;

void Exec(void* run)
{
	__asm
		exx
		ld		hl,(0x0006)
		dec		hl
		ld		de,#JumpHLEnd
		ex		de,hl
		ld		bc,#JumpHLEnd-#JumpHL+#1
		lddr
		inc		de
		push	de
		exx
		ret
	JumpHL:
		push	hl
		ld		a,(0xfcc1)
		ld		h,#0x00
		call	#0x0024
		ret
		nop
	JumpHLEnd:
		nop

	__endasm;
}

u8* EXPTBL = 0xFCC1; 

void main(const char* args)
{
	UNUSED(args);
	Print("Hello World!!\r\n");
	Print(args);
	Print("\r\n");

	{
		u32 size = 0;
		u8 result = 0;
		result = Open(defaultFCB);
		if (result)
		{
			Print("file not found.\r\n");
			return;
		}
		size = defaultFCB->fileSize;;
		PrintHexU32(size);

		SetReadAddress(&g_header);

		defaultFCB->recordSize = 1;
		defaultFCB->randomRecord = 0;

		result = ReadRandomRecord(defaultFCB, sizeof(struct BloadHeader));
		if (result)
		{
			Print("read error\r\n");
		}

		Print("\r\n");
		PrintHexU8(g_header.id);
		PrintHexU16(g_header.start);
		PrintHexU16(g_header.end);
		PrintHexU16(g_header.run);

		SetReadAddress((u16*) g_header.start);

		ReadRandomRecord(defaultFCB, g_header.end - g_header.start);

		PrintHexU8(*(u8*) g_header.start);

		Exec((void*)g_header.run);

		Close(defaultFCB);
	}


}
