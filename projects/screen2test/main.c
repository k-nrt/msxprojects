#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-sprite.h>
#include <msx-rand.h>

#include "pat.h"
#include "obj.h"

const u8 g_patterns[] =
{
#include "pattern16x16.inc"
};

u16 g_nTimer;
void TimerCallBack(void);

struct SBall
{
	s8 x, y, z;
	s16 vx, vy;
	struct SObj *pObj;
};

#define MAX_BALLS (32)
struct SBall s_balls[MAX_BALLS];

struct SBg
{
	s8 x,y,z;
	s8 size;
	const void *pObj;
};

//BG_SET(pBg,x,y,z,size,pObj) pBg->x=x; pBg->y=y; pBg->z=z; pBg->size=size; pBg->pObj=pObj;
void BgSet( struct SBg *pBg, s8 x, s8 y, s8 z, s8 size, const Obj3d *pObj3d )
{
	pBg->x = x;
	pBg->y = y;
	pBg->z = z;
	pBg->size = size;
	pBg->pObj = pObj3d;
}

enum
{
	kBg_Road,
	kBg_Ceil,
	kBg_FarLeft,
	kBg_FarRight,
	kBg_FenceLeft,
	kBg_FenceRight,
	kBgCount,
};

struct SBgBlock
{
	struct SBg bg[kBgCount];
};


#define BG_DEPTH (8)
struct SBg s_bgLeft[BG_DEPTH];
struct SBg s_bgLeftFence[BG_DEPTH];
struct SBg s_bgRight[BG_DEPTH];
struct SBg s_bgCenter[BG_DEPTH];

struct SBgBlock s_bgBlock[BG_DEPTH];

s8 g_sx = 0;
s8 g_sy = 0;
s8 g_sz = 0;
s8 g_vx = 0;
s8 g_vy = 0;
s8 g_vz = 0;

s8 g_nBgBlockPos = 0;

#define PERS_DEPTH (64)
#define PERS_NEAR (16) 
#define PERS_FAR  (PERS_NEAR+PERS_DEPTH)
#define PERS_SCREEN_DISTANCE (32)

#define PERS_MAX_X (32)

#define PERS_SCREEN_X (16)
#define PERS_SCREEN_Y (8)

s8 g_nPersValues[PERS_MAX_X][PERS_DEPTH];

void PersInit()
{
	s8 x, z;
	for( x = 0 ; x < PERS_MAX_X ; x++ )
	{
		for( z = 0 ; z < PERS_DEPTH ; z++ )
		{
			s16 sx = ((s16)PERS_SCREEN_DISTANCE)*((s16)x)/((s16)(z+PERS_NEAR));
			g_nPersValues[x][z] = (s8) sx;
		}
	}
}

s8 PersPositive( s8 x, s8 z )
{
	z -= PERS_NEAR;

	while( x >= PERS_MAX_X )
	{
		x >>= 1;
		z >>= 1;
	}
	
	while( z >= PERS_DEPTH )
	{
		x >>= 1;
		z >>= 1;
	}

	return g_nPersValues[x][z];
}

s8 Pers( s8 x, s8 z )
{
	if( x < 0 )
	{
		return -PersPositive(-x,z);
	}
	else
	{
		return PersPositive(x,z);
	}
}

#define PersX(_x,_z) (Pers(_x,_z) + PERS_SCREEN_X)
#define PersY(_y,_z) (Pers(_y,_z) + PERS_SCREEN_Y)
#define PersSize(_size,_z) (Pers(_size,_z))

void main(void)
{
	u8 i;
	u16 nTimer;

	msxBiosInitializeGraphicMode();
	msxBiosChangeColor( 1, 15, 4, 0 );
	msxTimerSetCallBack( TimerCallBack ); 

	msxRandInit( 0x78, 0xdead );

	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[0],     0x0000, 256*8 );
	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[0],     0x0800, 256*8 );
	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[0],     0x1000, 256*8 );

	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[256*8], 0x2000, 256*8 );
	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[256*8], 0x2800, 256*8 );
	msxBiosCopyRAMtoVRAM( (u16) &g_patterns[256*8], 0x3000, 256*8 );

	msxBiosFillVRAM( 0x1800, 768, 0 );
	msxBiosFillVRAM( 0x1c00, 768, 0 );

	PersInit();

	for( i = 0 ; i < BG_DEPTH ; i++ )
	{
		u8 j;
		struct SBgBlock *pBlock = &s_bgBlock[i];
		for( j = 0 ; j < kBgCount ; j++ )
		{
			BgSet( &pBlock->bg[j], 0, 0, 0, 0, NULL );
		}

		if( i < 4 )
		{
//			continue;
		}

		if( i & 1 )
		{
			BgSet( &pBlock->bg[kBg_Road], 0,4,0, 16, ( (i & 2) ? g_obj3dRoad0 : g_obj3dRoad1 ) );
		}
		BgSet( &pBlock->bg[kBg_Ceil], 0,-2,0, 40, NULL );//( (i & 1) ? g_obj3dRoad0 : g_obj3dRoad1 ) );

		if( i & 1 )
		{
			BgSet( &pBlock->bg[kBg_FarLeft], -22,-4,0, 8, g_obj3dBuilding1Left );
			BgSet( &pBlock->bg[kBg_FarRight], 22,-2,0, 6, g_obj3dBuildingRight );
		}
		else
		{
			if( i & 2)
			{
				BgSet( &pBlock->bg[kBg_FarLeft], -18,-2,0, 4, g_obj3dTreeDark );
				BgSet( &pBlock->bg[kBg_FarLeft], -22,-4,0, 8, g_obj3dBuilding2Left );
				BgSet( &pBlock->bg[kBg_FarRight], 18,-2,0, 4, g_obj3dTreeDark );
			}
			else
			{
				BgSet( &pBlock->bg[kBg_FarLeft], -18,-2,0, 4, g_obj3dTreeLight );
//				BgSet( &pBlock->bg[kBg_FarLeft], -22,-2,0, 6, g_obj3dBuilding1Left );
				BgSet( &pBlock->bg[kBg_FarRight], 18,-2,0, 4, g_obj3dTreeLight );
			}
		}


		//if( i & 1 )
		{
			BgSet( &pBlock->bg[kBg_FenceLeft], -20, 2,0, 4, ( (i & 1) ? g_obj3dFenceRed : g_obj3dFenceWhite ) );
			BgSet( &pBlock->bg[kBg_FenceRight], 20, 2,0, 4, ( (i & 1) ? g_obj3dFenceBlue : g_obj3dFenceWhite ) );
		}

	}

	g_nTimer = 0;
	nTimer = g_nTimer;

	for(;;)
	{
		u8 stick = msxBiosGetStick(0);

		if( ( 1 <= stick && stick <= 2) || 8 <= stick )
		{
			g_sy--;
			g_vy = -1;
		}
		else if( 4 <= stick && stick <= 6 )
		{
			g_sy++;
			g_vy = +1;
		}
		else
		{
			g_vy = 0;
		}

		if( 2 <= stick && stick <= 4 )
		{
			g_sx++;
			g_vx = 1;
		}
		else if( 6 <= stick && stick <= 8 )
		{
			g_sx--;
			g_vx = -1;
		}
		else
		{
			g_vx = 0;
		}

		if( msxBiosGetTrigger( 0 ) )
		{
			g_vz = +2;
			g_sz += 7;
		}
		else
		{
			g_vz = +1;
			g_sz += 1;
		}

		

		PatClearBuffer();

		{
			s8 z = PERS_FAR - (g_sz & 0x7);
			for( i = 0 ; i < BG_DEPTH ; i++ )
			{
				u8 j;
				struct SBgBlock *pBlock = &s_bgBlock[((g_sz >> 3)+7-i)&(BG_DEPTH-1)];

				for( j = 0 ; j < kBgCount ; j++ )
				{
					struct SBg* pBg = &pBlock->bg[j];
					if( pBg->pObj )
					{
						Obj3dBlit( PersX(pBg->x-g_sx,z), PersY(pBg->y-g_sy,z), PersSize(pBg->size,z), pBg->pObj );
					}
				}

				z -= 8;
			}

		}



		//PatFill( g_sx, g_sy, 4, 4, 0 );
		//PatFill( g_sx, g_sy, 4, 4, 0 );

		while( g_nTimer == nTimer )
		{
		}
		nTimer = g_nTimer;

		PatFlip();
	}
}


void TimerCallBack(void)
{
//	SpbSendSprite(0x1b00);
	g_nTimer++;
}



