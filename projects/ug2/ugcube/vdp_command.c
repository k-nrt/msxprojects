#include "bios_wrapper.h"

#include "vdp_command.h"
#include "macros.h"

#include <stdarg.h>


//! FORCLR(F3E9H, 1).
//! 初期値	15.
//! 内容	前景色。COLOR文で設定される.
SDCC_FIXED_ADDRESS(0xF3E9) u8 FORCLR;

//! BAKCLR(F3EAH, 1)
//! 初期値	4
//! 内容	背景色。COLOR文で設定される
SDCC_FIXED_ADDRESS(0xF3EA) u8 BAKCLR;

//! BDRCLR(F3EBH, 1).
//! 初期値	7.
//! 内容	周辺色。COLOR文で設定される.
SDCC_FIXED_ADDRESS(0xF3EB) u8 BDRCLR;

//! DFPAGE(FAF5H, 1)
//! 内容	ディスプレイページ番号
SDCC_FIXED_ADDRESS(0xFAF5) u8 DFPAGE;

//! ACPAGE(FAF6H, 1)
//! 内容	アクティブページ番号
SDCC_FIXED_ADDRESS(0xFAF6) u8 ACPAGE;

//! GRPACX(FCB7H, 2).
//! 内容	グラフィックアキュムレータ(X座標).
SDCC_FIXED_ADDRESS(0xFCB7) u16 GRPACX;

//! GRPACY(FCB9H, 2)
//! 内容	グラフィックアキュムレータ(Y座標).
SDCC_FIXED_ADDRESS(0xFCB9) u16 GRPACY;

//! LOGOPR(FB02H, 1)
//! 内容	ロジカル・オペレーション・コード.
SDCC_FIXED_ADDRESS(0xFB02) u8 LOGOPR;

#if defined(SYSTEM_ROM32K)
SDCC_FIXED_ADDRESS(0x0006) u8 VDPRead;
SDCC_FIXED_ADDRESS(0x0007) u8 VDPWrite;

#elif defined(SYSTEM_MSXDOS)
u8 VDPRead = 0;
u8 VDPWrite = 0;

#else
#error "Unknown system"
#endif

#if defined(SYSTEM_ROM32K)
void VDPPrintCore(register const char *pszString)
{
	for (; *pszString != 0; pszString++)
	{
		if (*pszString == '\n')
		{
			GRPACX = 0;
			GRPACY += 8;
			if ((212 - 8) <= GRPACY)
			{
				GRPACY = 0;
			}

			continue;
		}
		vdpPrintChar(*pszString);
	}
}
#endif

void VDPPrintF(const char *pszFormat, ...)
{
	va_list vaList;
	va_start(vaList,pszFormat);

	for (; *pszFormat != '\0'; pszFormat++)
	{
		register char c = *pszFormat;
		if (c == '%')
		{
			register char f;
			pszFormat++;
			f = *pszFormat;

			if (f == 'd')
			{
				u16 v = (u16)va_arg(vaList, int);
				VDPPrintU16DCore(v);
			}
			else if (f == 'x')
			{
				u16 v = (u16)va_arg(vaList, int);
				VDPPrintU16XCore(v);
			}
			else if (f == 's')
			{
				const char *s = (const char *)va_arg(vaList, const char *);
				VDPPrintCore(s);
			}
			else if (f == '\0')
			{
				break;
			}
			else
			{
				vdpPrintChar(c);
			}
		}
		else if (c == '\n')
		{
			GRPACX = 0;
			GRPACY += 8;
			if ((212 - 8) <= GRPACY)
			{
				GRPACY = 0;
			}

			continue;
		}
		else
		{
			vdpPrintChar(c);
		}
	}

	va_end(vaList);
}
