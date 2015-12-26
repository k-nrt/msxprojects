#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-sprite.h>
#include <msx-rand.h>

#include "sprite_blinker.h"

u16 g_nTimer;

void TimerCallBack(void);

const u8 g_aSpritePattern[] =
{
	0x07, 0x1f, 0x3f, 0x7f, 0x7f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x7f, 0x7f, 0x3f, 0x1f, 0x07,
	0xe0, 0xf8, 0xfc, 0xfe, 0xfe, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xe0,
};

struct SBall
{
	s16 x, y, vx, vy;
	u8 color;
	u8 pad[7];
};

#define BALL_COUNT (32)
struct SBall g_balls[BALL_COUNT];

u8 g_nbBalls = 4;
u8 g_nPrevStick = 0;

void main(void)
{
	u8 i;
	u16 nTimer;

	msxBiosInitializeText32Mode();
	msxBiosChangeColor( 1, 15, 1, 7 );
	msxTimerSetCallBack( TimerCallBack ); 

	msxSpriteSetMode( kSpriteMode_16x16x1 );
	msxSpriteSetPattern( 0, g_aSpritePattern );

	msxRandInit( 0x12, 0x3456 );

	SpbInit();


	g_nTimer = 0;
	nTimer = g_nTimer;

	for( i = 0 ; i < BALL_COUNT ; i++ )
	{
		struct SBall *pBall = &g_balls[i];
#if 0
		pBall->x = (0+ ((s16)i)*16)<<4;//(msxRandGet16() & 0x0f00);;
		pBall->y = 32;//(32 + i*2) << 4;//msxRandGet16() & 0x0ff0;
		pBall->vy = 0;//16 + (msxRandGet16() & 0x2f);
		pBall->vx = 0;//0 + (msxRandGet16() & 0x2f);
#else
		pBall->x = (msxRandGet16() & 0x0ff0);
		pBall->y = msxRandGet16() & 0x07f0;
		pBall->vy = 16 + (msxRandGet16() & 0x2f);
		pBall->vx = -31 + (msxRandGet16() & 0x2f);
#endif
		pBall->color = (msxRandGet16()%14)+2;
	}

	for(;;)
	{
		struct SBall *pBall = g_balls;
		u8 stick = msxBiosGetStick(0);

		if( g_nPrevStick == 0 )
		{
			if( stick == 3 && g_nbBalls < 32 )
			{
				g_nbBalls++;
			}

			if( stick == 7 && g_nbBalls > 0 )
			{
				g_nbBalls--;
			}
		}

		g_nPrevStick = stick;

		pBall = g_balls;
		for( i = 0 ; i < BALL_COUNT ; i++, pBall++ )
		{
			pBall->x += pBall->vx;
			pBall->y += pBall->vy;

			if( pBall->x < 0 )
			{
				pBall->x = 0;
				pBall->vx = -pBall->vx;
			}
			else if( ((256 - 16) << 4) < pBall->x )
			{
				pBall->x = (256 - 16) << 4;
				pBall->vx = -pBall->vx;
			}

			if( pBall->y < 0 )
			{
				pBall->y = 0;
				pBall->vy = -pBall->vy;
			}
			else if( ((192 - 16) << 4) < pBall->y )
			{
				pBall->y = (192 - 16) << 4;
				pBall->vy = -pBall->vy;
			}

			if( i < g_nbBalls )
			{
				SpbSetSprite( i, pBall->x >> 4, pBall->y >> 4, 0, pBall->color );
			}
			else
			{
				SpbSetSprite( i, 0, 209, 0, 0 );
			}
		}

//		while( g_nTimer <= (nTimer+1) )
		while( g_nTimer == nTimer )
		{
		}
		
		nTimer = g_nTimer;
	}
}


void TimerCallBack(void)
{
	SpbSendSprite(0x1b00);
	g_nTimer++;
}

