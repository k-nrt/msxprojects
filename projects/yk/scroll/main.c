#include <msx-types.h>
#include "msx-dos.h"
#include "msx-dos-vdp.h"
#include "msx-dos-input.h"
#include "yk-bg.h"

typedef struct BloadHeader_
{
	u8 id;
	u16 start;
	u16 end;
	u16 run;
} BloadHeader;

BloadHeader g_bloadHeader;

bool Bload(msxDosFCB* fcb, BloadHeader* bloadHeader)
{
	u32 size = 0;
	u8 result = 0;
	result = msxDosOpen(fcb);
	if (result)
	{
		msxDosPrint("file not found.\r\n");
		return FALSE;
	}

	size = fcb->fileSize;
	msxDosPrint("size=");
	msxDosPrintHexU32(size);
	msxDosPrint("\r\n");

	msxDosSetReadAddress(bloadHeader);

	fcb->recordSize = 1;
	fcb->randomRecord = 0;

	result = msxDosReadRandomRecord(fcb, sizeof(BloadHeader));
	if (result)
	{
		msxDosPrint("read error\r\n");
		msxDosClose(fcb);
		return FALSE;
	}

	msxDosPrint("start=");
	msxDosPrintHexU16(bloadHeader->start);
	msxDosPrint("\r\n");
	msxDosPrint("end=");
	msxDosPrintHexU16(bloadHeader->end);
	msxDosPrint("\r\n");

	msxDosPrint("Loading ...\r\n");

	msxDosSetReadAddress((u16*) bloadHeader->start);

	result = msxDosReadRandomRecord(fcb, bloadHeader->end - bloadHeader->start);
	if (result)
	{
		msxDosPrint("read error\r\n");
		msxDosClose(fcb);
		return FALSE;
	}

	msxDosClose(fcb);

	msxDosPrint("Done\r\n");

	return TRUE;
}

void WaitForHitSpace()
{
	for(;;)
	{
		if (inputGetTrigger(0))
		{
			return;
		}
	}
}

u8 buffer[16];

const u8 spritePattern[32] =
{
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
};

void main(const char* args)
{
	UNUSED(args);
	msxDosPrint("Hello World!!\r\n");
	msxDosPrint(args);
	msxDosPrint("\r\n");

	{
		bool result = Bload(&msxDosDefaultFCB0, &g_bloadHeader);
		if (!result)
		{
			return;
		}
	}

	//WaitForHitSpace();

	vdpWriteVram(0x0000, 0xff);
	vdpFillVram(0x0010, 0x10, 0xcd);
	vdpCopyToVram("0123456789ABCDEF", 0x0020, 0x10);
	vdpCopyFromVram(0x0020, buffer, 0x10);

	vdpSetScreenColor(15, 1, 7);
	vdpSetScreenMode(5);
	vdpClearSprite();
	vdpSetSpriteMode(kVdpSpriteMode_16x16);

	vdpSetLogicalOperation(kVdpLogicalOperationCode_IMP);
	vdpSetForegroundColor(0x0f);
	vdpSetGraphicAccumlator(16, 128);
	vdpPrintChar('*');

	vdpSetGraphicAccumlator(16, 136);
	vdpPrintHexU32(0xdeadbeef);

	vdpSetGraphicAccumlator(16, 144);
	vdpPrint("Hello World!!");

	{
		vdpSetPalette(2, 13, (u8*)(g_bloadHeader.start + 0x0012));
	}

	{
		u16 i, j;
		vdpSetActivePage(0);
		vdpSetDisplayPage(0);
		
		for(i = 0; i < 12*14; i++)
		{
			u8* src = (u8*)(g_bloadHeader.start + 0x006c + (i << 5));
			u16 x = i%32;
			u16 y = i/32;
			u16 dst = x*4 + y*128*8;

			for (j = 0; j < 8; j++)
			{
				vdpCopyToVram((void*) src, dst, 4);
				src += 4;
				dst += 128;
			}
		}
	
		WaitForHitSpace();
	}

	{
		u16 i, j;
		vdpSetActivePage(1);
		vdpSetDisplayPage(1);

		for(i = 0; i < 12*14; i++)
		{
			u8* src = (u8*)(g_bloadHeader.start + 0x006c + (i << 5));
			u16 x = i%12;
			u16 y = i/12;
			u16 dst = x*4 + y*128*8;

			for (j = 0; j < 8; j++)
			{
				vdpCopyToVram((void*) src, dst, 4);
				src += 4;
				dst += 128;
			}
		}
	}

	{
		u8 i;
		u16 pattern = 0x7800;
		u16 attribute = 0x7600;
		u16 color = 0x7400;
		u8* src = (u8*)(g_bloadHeader.start + 0x3404);

		for(i = 0; i < 30; i++)
		{
			u8 x = 128 + (i%3)*16;
			u8 y = (i/3)*16;
			vdpCopyToVram(src, pattern, 32);
			src += 32;
			pattern += 32;

			vdpCopyToVram(src, color, 16);
			src += 16;
			color += 16;

			vdpWriteVram(attribute, y);
			attribute++;
			vdpWriteVram(attribute, x);
			attribute++;
			vdpWriteVram(attribute, i << 2);
			attribute++;
			vdpWriteVram(attribute, 0);
			attribute++;
		}
		WaitForHitSpace();
	}

	{
		u16 i, j;
		u8* pos = (u8*)(g_bloadHeader.start + 0x156c);
		for (i = 0; i < (24*9+4 - 23) ; i++)
		{
			u8* pos = (u8*)(g_bloadHeader.start + 0x156c);
			pos += i*32;

			vdpSetDisplayPage(1 + (~i & 0x01));

			for (j = 0; j < 24 ; j++)
			{
				u16 dy = ((i & 0x01) << 8) + 256 + 192 - 8 - (j << 3);
				ykDrawBg(pos, dy);
				pos += 32;
			}
		}
		
		WaitForHitSpace();
	}

	vdpSetActivePage(0);
	vdpSetDisplayPage(0);

	vdpSetScreenMode(0);
	msxClearKeyBuffer();
}
