#include <msx-bios-wrapper.h>

#include "vdp_command.h"
#include "macros.h"

#include <stdarg.h>


//! FORCLR(F3E9H, 1).
//! �����l	15.
//! ���e	�O�i�F�BCOLOR���Őݒ肳���.
SDCC_FIXED_ADDRESS(0xF3E9) u8 FORCLR;

//! BAKCLR(F3EAH, 1)
//! �����l	4
//! ���e	�w�i�F�BCOLOR���Őݒ肳���
SDCC_FIXED_ADDRESS(0xF3EA) u8 BAKCLR;

//! BDRCLR(F3EBH, 1).
//! �����l	7.
//! ���e	���ӐF�BCOLOR���Őݒ肳���.
SDCC_FIXED_ADDRESS(0xF3EB) u8 BDRCLR;

//! DFPAGE(FAF5H, 1)
//! ���e	�f�B�X�v���C�y�[�W�ԍ�
SDCC_FIXED_ADDRESS(0xFAF5) u8 DFPAGE;

//! ACPAGE(FAF6H, 1)
//! ���e	�A�N�e�B�u�y�[�W�ԍ�
SDCC_FIXED_ADDRESS(0xFAF6) u8 ACPAGE;

//! GRPACX(FCB7H, 2).
//! ���e	�O���t�B�b�N�A�L�������[�^(X���W).
SDCC_FIXED_ADDRESS(0xFCB7) u16 GRPACX;

//! GRPACY(FCB9H, 2)
//! ���e	�O���t�B�b�N�A�L�������[�^(Y���W).
SDCC_FIXED_ADDRESS(0xFCB9) u16 GRPACY;

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
		msx2BiosGraphicPrint(*pszString);
	}
}

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
				msx2BiosGraphicPrint(c);
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
			msx2BiosGraphicPrint(c);
		}
	}

	va_end(vaList);
}

