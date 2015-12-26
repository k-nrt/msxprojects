//. スプライト点滅.
#include "sprite_blinker.h"
#include <msx-bios-wrapper.h>

struct SSpritePlane
{
	u8 y,x,iPattern,nColor;
};

#define SPRITE_MAX (32)
static struct SSpritePlane s_spritePlanes[SPRITE_MAX];
static u8 s_u8Phase = 0;

//. 初期化.
void SpbInit(void)
{
	struct SSpritePlane *pPlane = s_spritePlanes;
	s8 i;

	for( i = 0 ; i < SPRITE_MAX ; i++ )
	{
		pPlane->y = 209;
		pPlane->x = 0;
		pPlane->iPattern = 0;
		pPlane->nColor = 0;
		pPlane++;
	}

	s_u8Phase = 0;
}

//. スプライト設定.
void SpbSetSprite(u8 iPlane, s16 x, s16 y, u8 iPattern, u8 nColor)
{
	register u8 *pVdp = (u8*) s_spritePlanes;
	pVdp += iPlane << 2;

	if( x < -16 || 255 < x || y < -16 || 191 < y )
	{
		//. クリップ.
		*pVdp = 209;
		return;
	}
	else if( x < 0 )
	{
		//. 左端から消える.
		*pVdp = y;
		pVdp++;

		*pVdp = (u8) (x + 32);
		pVdp++;

		*pVdp = iPattern;
		pVdp++;

		*pVdp = 0x80 | nColor;
	}
	else
	{
		*pVdp = y;
		pVdp++;

		*pVdp = (u8) x;
		pVdp++;

		*pVdp = iPattern;
		pVdp++;

		*pVdp = nColor;
	}
}

/*
01234567
76543210
23016745
54761032
67452301
10325476
45670123
32107654
*/

#define SP(a0,a1,a2,a3,a4,a5,a6,a7) &s_spritePlanes[a0*4], &s_spritePlanes[a1 * 4], &s_spritePlanes[a2 * 4], &s_spritePlanes[a3 * 4], &s_spritePlanes[a4 * 4], &s_spritePlanes[a5 * 4],&s_spritePlanes[a6 * 4],&s_spritePlanes[a7 * 4]

const static struct  SSpritePlane *s_pSrcAddress[] =
{
	SP(0, 1, 2, 3, 4, 5, 6, 7),
	SP(7, 6, 5, 4, 3, 2, 1, 0),
	SP(2, 3, 0, 1, 6, 7, 4, 5),
	SP(5, 4, 7, 6, 1, 0, 3, 2),
	SP(6, 7, 4, 5, 2, 3, 0, 1),
	SP(1, 0, 3, 2, 5, 4, 7, 6),
	SP(4, 5, 6, 7, 0, 1, 2, 3),
	SP(3, 2, 1, 0, 7, 6, 5, 4),
};

void SpbSendSprite(u16 pSpriteAttribute)
{
	static __sfr __at 0x98 vdpPort0;
	register u8 i;

	//. スプライトアトリビュートテーブル.
	msxBiosSetWrite(pSpriteAttribute);

	for (i = 0; i < 8; i++, s_u8Phase++)
	{
		register u8 *pVdp = (u8*) s_pSrcAddress[s_u8Phase];

		//. 0
		vdpPort0 = *pVdp; pVdp++; //. y.
		vdpPort0 = *pVdp; pVdp++; //. x.
		vdpPort0 = *pVdp; pVdp++; //. pattern.
		vdpPort0 = *pVdp; pVdp++; //. color.

		//. 1
		vdpPort0 = *pVdp; pVdp++; //. y.
		vdpPort0 = *pVdp; pVdp++; //. x.
		vdpPort0 = *pVdp; pVdp++; //. pattern.
		vdpPort0 = *pVdp; pVdp++; //. color.

		//. 2
		vdpPort0 = *pVdp; pVdp++; //. y.
		vdpPort0 = *pVdp; pVdp++; //. x.
		vdpPort0 = *pVdp; pVdp++; //. pattern.
		vdpPort0 = *pVdp; pVdp++; //. color.

		//. 3
		vdpPort0 = *pVdp; pVdp++; //. y.
		vdpPort0 = *pVdp; pVdp++; //. x.
		vdpPort0 = *pVdp; pVdp++; //. pattern.
		vdpPort0 = *pVdp;         //. color.
	}

	s_u8Phase &= 0x3f;
}
