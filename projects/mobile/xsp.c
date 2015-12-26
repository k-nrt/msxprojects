//. スプライト点滅.
#include "xsp.h"
#include <msx-bios-wrapper.h>

struct SSpritePlane
{
	u8 blink;
	u8 y,x,iPattern,nColor;
	u8 index;
	u8 blocks;
	u8 yb;
};

//. u8* アクセス用.
#define SP_BLINK(p) (*p)
#define SP_Y(p) (*(p+1))
#define SP_X(p) (*(p+2))
#define SP_PATTERN(p) (*(p+3))
#define SP_COLOR(p) (*(p+4))
#define SP_INDEX(p) (*(p+5))
#define SP_BLOCKS(p) (*(p+6))
#define SP_YB(p) (*(p+7))

#define SPRITE_MAX (32)
static struct SSpritePlane s_spritePlanes[SPRITE_MAX];
static u8 s_nBlink = 0;
//struct SSpritePlane s_spritePlanes[SPRITE_MAX];
//u8 s_nBlink = 0;

//. 8 ピクセル毎にスプライトが何枚あるか格納.
static u8 s_nCounter[32];

//. 初期化.
void SpbInit(void)
{
	s8 i;

	for( i = 0 ; i < SPRITE_MAX ; i++ )
	{
		struct SSpritePlane *pPlane = &s_spritePlanes[i];
		pPlane->blink = 0;
		pPlane->y = 209;
		pPlane->x = 0;
		pPlane->iPattern = 0;
		pPlane->nColor = 0;
		pPlane->index = 0;
		pPlane->blocks = 0;
		pPlane->yb = 0xff;
		pPlane++;
	}

	s_nBlink = 0;
}

//. スプライト設定.
void SpbSetSprite(u8 iPlane, s16 x, s16 y, u8 iPattern, u8 nColor)
{
	register u8 *pVdp = (u8*) s_spritePlanes;
	register u8 m;
	pVdp += iPlane << 3;
	pVdp++;

	if( x < -16 || 255 < x || y < -16 || 191 < y )
	{
		//. クリップ.
		*pVdp = 209;
		pVdp += 5;

		*pVdp = 0;
		pVdp += 2;

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

	pVdp+=2;

	y += 16;
	m = y & 0x7;
	y >>= 3;

	*pVdp = ( m ? 3 : 2 );
	pVdp++;

	*pVdp = y;
}
#if 1
//. 横並びチェック.
void SpbUpdate(void)
{
	//. 重いので処理を 2 フレームに分ける.
	static u8 phase = 0;
	s8 i;
	register u8 *pVdp;

	if( phase == 0 )
	{
		//. まず縦 8 ピクセル毎に何枚スプライトがあるか調べる.
		pVdp = s_nCounter;
		for( i = 0 ; i < 32 ; i++, pVdp++ )
		{
			*pVdp = 0;
		}
		pVdp = (u8*) s_spritePlanes;
		for( i = 0 ; i < SPRITE_MAX ; i++, pVdp+=sizeof(struct SSpritePlane) )
		{
			register u8 y, m;
			register u8 max;
			register u8 *pCounter;

			if( SP_BLOCKS(pVdp) == 0 )
			{
				//. 非表示.
				continue;
			}
			y = SP_YB(pVdp);

			pCounter = &s_nCounter[y];

			//. 1.
			max = *pCounter;
			(*pCounter)++;
			pCounter++;

			//. 2.
			if( max < (*pCounter) ){ max = (*pCounter); }
			(*pCounter)++;

			//. 余ってたら連続する 3 ブロックに登録.
			if( SP_BLOCKS(pVdp) == 3 )
			{
				pCounter++;
				if( max < (*pCounter) ){ max = (*pCounter); }
				(*pCounter)++;
			}

			//. 一番多い枚数を保存.
			SP_INDEX(pVdp) = max;
		}
	}
	else if( phase == 1 )
	{
		//. 自分より優先度の低いものがいればブリンクする.
		pVdp = (u8*) s_spritePlanes;
		for( i = 0 ; i < SPRITE_MAX ; i++ )
		{
			register u8 y, m;
			register u8 max;
			register u8 *pCounter;

			*pVdp = 0; //. blink フラグクリア.

			//pVdp++;
			//y = *pVdp; //y;

			if( SP_BLOCKS(pVdp) == 0 )
			{
				pVdp += sizeof( struct SSpritePlane );
				continue;
			}
			y = SP_YB(pVdp);
			//y += 16; //. -16 ～ 0 があると面倒なのでシフト.
			//m = y & 0x7;
			//y >>= 3;

			pCounter = &s_nCounter[y];
			max = *pCounter;
			pCounter++;

			if( max < (*pCounter) ) { max = (*pCounter); }

			if( SP_BLOCKS(pVdp) == 3 )
			{
				pCounter++;
				if( max < (*pCounter) ) { max = (*pCounter); }
			}

			pVdp++;
			if( max > 4 )
			{
				pVdp += 4; // index.
				if( (max-12) > *pVdp )
				{
					pVdp -= 5;
					*pVdp = 7;
					pVdp += sizeof( struct SSpritePlane );
				}
				else if( (max-8) > *pVdp )
				{
					pVdp -= 5;
					*pVdp = 3;
					pVdp += sizeof( struct SSpritePlane );
				}
				else if( (max - 4) > *pVdp )
				{
					//.　自分が登録したときとの差が４以上で点滅する.
					pVdp -= 5;
					*pVdp = 1;//((1 << (max>>2)) - 1);//max >> 2; //. blink.
					pVdp += sizeof( struct SSpritePlane );
				}
				else
				{
					pVdp += 3;
				}
			}
			else
			{
				pVdp += sizeof( struct SSpritePlane ) - 1;
			}
		}
	}

	phase = (phase + 1)&0x1;
//	phase = 
}
#else
//. 横並びチェック.
void XspUpdate(void)
{
	//. 重いので処理を 2 フレームに分ける.
	static u8 phase = 0;
	s8 i;
	register u8 *pVdp;

	//if( phase == 0 )
	{
		u8 *pSprite = (u8*) s_spritePlanes;
		for( i = 0 ; i < SPRITE_MAX - 4 ; i++, pSprite+=sizeof(struct SSpritePlane) )
		{
			u8 sy, syt, syb;
			s8 j;
			u8 nbCollisions;
			u8 *pTarget;
			pTarget = pSprite + sizeof(struct SSpritePlane);


			SP_BLINK(pSprite) = 0;
			//SP_COLOR(pSprite) = 4;

			sy = SP_Y(pSprite);
			if( 191 < sy && sy < 241 )
			{
				//. 非表示.
				continue;
			}

			syt = sy + 32 - 16;
			syb = sy + 32 + 16;

			nbCollisions = 0;
			
			for( j = i + 1 ; j < SPRITE_MAX ; j++, pTarget += sizeof(struct SSpritePlane) )
			{
				u8 ty;
				ty = SP_Y(pTarget);

				if( 191 < ty && ty < 241 )
				{
					//. 非表示.
					continue;
				}

				ty = ty + 32;

				if( ty < syt || syb <= ty )
				{
					//. 非接触.
					continue;
				}

				nbCollisions++;

				/*
				if( nbCollisions >= 15 )
				{
					break;
				}
				*/
			}

			SP_BLINK(pSprite) = ((1 << (nbCollisions>>2)) - 1);
			/*
			if( nbCollisions > 4 )
			{
				SP_COLOR(pSprite) = 15;
			}
			*/
		}
	}

	phase = (phase + 1)&0x3;
//	phase = 
}
#endif

//. 一気に転送.
//. ここだけ 60 fps で動かすと綺麗に見える.
void SpbSendSprite( u16 pSpriteAttribute )
{
	static __sfr __at 0x98 vdpPort0;
	register s8 i;
	register u8 *pVdp;

	//. スプライトアトリビュートテーブル.
	msxBiosSetWrite( pSpriteAttribute );

	pVdp = (u8*) s_spritePlanes;

	for( i = 0 ; i < SPRITE_MAX ; i++ )
	{
		u8 u8Blink = *pVdp;
		//if( ((1 << u8Blink) - 1) & s_nBlink )
		if( u8Blink & s_nBlink )
		{
			vdpPort0 = 209;
			vdpPort0 = 0;
			vdpPort0 = 0;
			vdpPort0 = 0;
			pVdp += sizeof(struct SSpritePlane);
		}
		else
		{
			pVdp++; // blink.

			//. y.
			vdpPort0 = *pVdp;
			pVdp++;
			
			//. x.
			vdpPort0 = *pVdp;
			pVdp++;

			//. pattern.
			vdpPort0 = *pVdp;
			pVdp++;

			//. color.
			vdpPort0 = *pVdp;
			pVdp++;

			pVdp+=3; // index.

		}
	}

	s_nBlink = (s_nBlink+1) & 0x7;
//	s_nBlink = (s_nBlink ? 0 : 1);
/*
; 00000000 0
; 01010101 1 4-7
; 00110011 2 8-15
; 00001111 3 16-31
*/
}
