#ifndef MTK_INPUT_H
#define MTK_INPUT_H

#include <msx-types.h>

typedef struct tMtkInput
{
	u8 m_down;
	u8 m_up;
	u8 m_push;
	u8 m_release;
} SMtkInput;

enum EMtkInput
{
	kMtkInput_Up     = 0x01,
	kMtkInput_Down   = 0x02,
	kMtkInput_Left   = 0x04,
	kMtkInput_Right  = 0x08,
	kMtkInput_Fire   = 0x10,
	kMtkInput_Thrust = 0x20,
};

extern SMtkInput g_mtkInput;
extern void MtkInputInit(void);
extern void MtkInputScan(void);

#define MtkInputIsDown(input)    (g_mtkInput.m_down    & (input))
#define MtkInputIsUp(input)      (g_mtkInput.m_up      & (input))
#define MtkInputIsPush(input)    (g_mtkInput.m_push    & (input))
#define MtkInputIsRelease(input) (g_mtkInput.m_release & (input))

#endif //MTK_INPUT_H
