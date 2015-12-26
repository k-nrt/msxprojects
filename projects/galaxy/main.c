#include <msx-bios-wrapper.h>
#include <msx-timer.h>
#include <msx-sprite.h>
#include <msx-rand.h>

#include "sprite_blinker.h"

//. 便利マクロ.
#define MIN(a,b) ((a<b)?a:b)
#define MAX(a,b) ((a>b)?a:b)
#define CLAMP(a,min,max) MAX(MIN(a,max),min)

//. タイマ割り込み.
u16 g_nTimer;
void TimerCallBack(void) __critical
{
	SpbSendSprite(0x1b00);
	g_nTimer++;
}

//. スプライトパターン.
const u8 s_spritePatterns[] =
{
#include "galaxy_sprite.inl"
};

//. 前の状態.
u8 g_nPrevStick = 0;
u8 g_nPrevButton = 0;

//. 自機.
s8 g_px = 0;
s8 g_py = 0;
s16 g_pz = 32;

s8 g_vx = 0;
s8 g_vy = 0;

s8 g_vxx = 0;
s8 g_vyy = 0;

u8 g_pattern = 3*4;

//. 自機ビーム.
struct SBeam
{
	u8 m_status;
	u8 pad0;
	s8 m_x;
	s8 m_y;
	s16 m_z;
	u16 pad1;
};

#define BEAM_COUNT (4)
struct SBeam g_beams[BEAM_COUNT];

//. 敵.
struct SEnemy
{
	u8  m_status;
	u8  pad;
	s8 m_x;
	s8 m_y;
	s16 m_z;
	u16 pad1;
};

//. 星.
struct SStar
{
	s8 m_x;
	s8 m_y;
	s16 m_z;
};


#define ENEMY_COUNT (4)
struct SEnemy g_enemies[ENEMY_COUNT];

#define STAR_COUNT (12)
struct SStar g_stars[STAR_COUNT];


//. パース変換テーブル.
static u8 g_pers[64][64];

#define PERS_Z_NEAR (16)
#define PERS_Z_FAR (PERS_Z_NEAR+80)
#define PERS_VALUE (256)

void PersInit()
{
	u8 i, j;

	for( i = 0 ; i < 64 ; i++ )
	{
		for( j = 0 ; j < 64 ; j++ )
		{
			s16 x = ((s16)i*PERS_VALUE) / ((s16)j + PERS_Z_NEAR);
			if( x > 255 )
			{
				g_pers[i][j] = 0xff;
			}
			else
			{
				g_pers[i][j] = (u8) x;
			}
		}
	}
}

s16 Perspective( s8 x, s8 z )
{
	s8 tz = z - PERS_Z_NEAR;
	if( tz < 0 )
	{
		tz = 0;
	}

	if( tz >= 64 )
	{
		tz = 63;
	}

	if( 64 <= x )
	{
		return (x*PERS_VALUE) / z;
	}
	else if( 0 <= x )
	{
		return (s16) g_pers[x][tz];
	}
	else if( -64 < x )
	{
		return (s16) -g_pers[-x][tz];
	}
	else
	{
		return (x*PERS_VALUE) / z;
	}
}

#define PERS_X(x,z) (Perspective(x,z)+128)
#define PERS_Y(y,z) (Perspective(y,z)+95)

//. 敵パターンテーブル.
const static u8 s_enemyPattern[] =
{
	0*4,
	0*4,
	1*4,
	1*4,
	14*4,
	14*4,
	14*4,
	14*4,

	13*4,
	13*4,
	13*4,
	13*4,
	12*4,
	12*4,
	12*4,
	12*4,

	11*4,
	11*4,
	11*4,
	11*4,
	10*4,
	10*4,
	10*4,
	10*4,

	9*4,
	9*4,
	9*4,
	9*4,
	8*4,
	8*4,
	8*4,
	8*4,
};

//. 敵パターンテーブル.
const static u8 s_enemyPattern2[] =
{
	0*4,  //. 0
	0*4,  //. 1
	1*4,  //. 2
	1*4,
	36*4, //. 4
	36*4,
	36*4,
	36*4,

	36*4, //. 6
	36*4,
	36*4,
	36*4,
	36*4, //. 8
	36*4,
	36*4,
	36*4,

	35*4, //. 10
	35*4,
	35*4,
	35*4,
	34*4, //. 12
	34*4,
	34*4,
	34*4,

	33*4, //. 14
	33*4,
	33*4,
	33*4,
	32*4, //. 16
	32*4,
	32*4,
	32*4,
};

//. 敵パターンテーブル.
const static u8 s_enemyPattern3[] =
{
	0*4,  //. 0
	1*4,  //. 2
	46*4, //. 4
	46*4,

	45*4, //. 6
	45*4,
	44*4, //. 8
	44*4,

	43*4, //. 10
	43*4,
	42*4, //. 12
	42*4,

	41*4, //. 14
	41*4,
	40*4, //. 16
	40*4,
};

//. 自機弾パターんテーブル.
const static u8 s_beamPattern[] =
{
	0*4,
	0*4,
	20*4,
	20*4,
	19*4,
	19*4,
	19*4,
	19*4,

	18*4,
	18*4,
	18*4,
	18*4,
	18*4,
	18*4,
	18*4,
	18*4,

	17*4,
	17*4,
	17*4,
	17*4,
	17*4,
	17*4,
	17*4,
	17*4,

	16*4,
	16*4,
	16*4,
	16*4,
	16*4,
	16*4,
	16*4,
	16*4,
};

//. 自機弾パターんテーブル.
const static u8 s_enemyShotPattern[] =
{
	0*4,
	0*4,
	0*4,
	1*4,
	1*4,
	54*4,
	54*4,
	54*4,

	53*4,
	53*4,
	53*4,
	53*4,
	52*4,
	52*4,
	52*4,
	52*4,

	51*4,
	51*4,
	51*4,
	51*4,
	50*4,
	50*4,
	50*4,
	50*4,

	49*4,
	49*4,
	49*4,
	49*4,
	48*4,
	48*4,
	48*4,
	48*4,
};

const static u8 s_crossPattern[] =
{
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,

	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,

	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,

	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
	2*4,
};

const static u8 s_starPattern[] =
{
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,

	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,

	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,

	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
	0 * 4,
};


void SetSprite3d( u8 plane, s8 x, s8 y, s16 z, const u8 *pPattern, s8 size, u8 color )
{
	static s16 sx,sy;
	static u8 pers, pat;
	static s8 pz;

	if (256 < z)
	{
		z = 256;
	}

	//. テーブル参照用 Z 作成.
	while( (z - PERS_Z_NEAR) > 63 )
	{
		z >>= 1;
		x >>= 1;
		y >>= 1;
		size >>= 1;
	}

	pz = z - PERS_Z_NEAR;
	if( pz < 0 )
	{
		pz = 0;
	}

	//. Y 座標変換.
	if( 64 <= y )
	{
		sy = 95 - 16 + ((y*128)/z);
	}
	else if( 0 <= y )
	{
		sy = 95 - 16 + (s16) g_pers[y][pz];
	}
	else if( -64 < y )
	{
		sy = 95 - 16 - (s16) g_pers[-y][pz];
	}
	else
	{
		sy = 95 - 16 + ((y*128)/z);
	}

	//. X 座標変換.
	if( 64 <= x )
	{
		sx = 128 - 16 + (x*128)/z;
	}
	else if( 0 <= x )
	{
		sx = 128 - 16 + (s16) g_pers[x][pz];
	}
	else if( -64 < x )
	{
		sx = 128 - 16 - (s16) g_pers[-x][pz];
	}
	else
	{
		sx = 128 - 16 + (x*128)/z;
	}

	//. パターンサイズ変換.
	pat = pPattern[CLAMP(g_pers[size][pz], 0, 15)];

	//. スプライト設定.
	SpbSetSprite( plane, sx, sy, pat, color );
}

void SetSprite3d2( u8 plane, s8 x, s8 y, s16 z, u8 pattern, s8 size, u8 color )
{
	static s16 sx,sy;
	static u8 pers, pat;
	static s8 pz;

	//. テーブル参照用 Z 作成.
	while( (z - PERS_Z_NEAR) > 63 )
	{
		z >>= 1;
		x >>= 1;
		y >>= 1;
		size >>= 1;
	}

	pz = z - PERS_Z_NEAR;
	if( pz < 0 )
	{
		pz = 0;
	}

	//. Y 座標変換.
	if( 64 <= y )
	{
		sy = 95 - 16 + ((y*128)/z);
	}
	else if( 0 <= y )
	{
		sy = 95 - 16 + (s16) g_pers[y][pz];
	}
	else if( -64 < y )
	{
		sy = 95 - 16 - (s16) g_pers[-y][pz];
	}
	else
	{
		sy = 95 - 16 + ((y*128)/z);
	}

	//. X 座標変換.
	if( 64 <= x )
	{
		sx = 128 - 16 + (x*128)/z;
	}
	else if( 0 <= x )
	{
		sx = 128 - 16 + (s16) g_pers[x][pz];
	}
	else if( -64 < x )
	{
		sx = 128 - 16 - (s16) g_pers[-x][pz];
	}
	else
	{
		sx = 128 - 16 + (x*128)/z;
	}

	//. スプライト設定.
	SpbSetSprite( plane, sx, sy, pattern, color );
}


void main(void)
{
	u8 i;
	u16 nTimer;

	
	msxBiosInitializeText32Mode();
	msxBiosChangeColor( 1, 15, 1, 7 );

	msxBiosCopyRAMtoVRAM( (u16) s_spritePatterns, 0x3800, 256*8 );

	msxTimerSetCallBack( TimerCallBack ); 

	msxSpriteSetMode( kSpriteMode_16x16x2 );
	//msxSpriteSetPattern( 4, &s_spritePatterns[i] );

	msxRandInit( 0x12, 0x3456 );

	SpbInit();

	{
		struct SBeam *pBeam = g_beams;
		for( i = 0 ; i < BEAM_COUNT ; i++, pBeam++ )
		{
			pBeam->m_status = 0;
			pBeam->m_x = 0;
			pBeam->m_y = 0;
			pBeam->m_z = 0;
		}
	}

	{
		struct SEnemy *pEnemy = g_enemies;
		for( i = 0 ; i < ENEMY_COUNT ; i++, pEnemy++ )
		{
			pEnemy->m_status = 0;
			pEnemy->m_x = 0;
			pEnemy->m_y = 0;
			pEnemy->m_z = 0;
		}
	}

	{
		struct SStar *pStar = g_stars;
		for (i = 0; i < STAR_COUNT; i++, pStar++)
		{
			pStar->m_x = msxRandGet8();
			pStar->m_y = msxRandGet8();
			pStar->m_z = msxRandGet8();
		}
	}
	PersInit();

	/*
	for( i = 0 ; i < 8 ; i++ )
	{
		g_enemies[i].m_status = 1;
		g_enemies[i].m_x = -6 + (i&0x3)*4;
		g_enemies[i].m_y = -6 + (i>>2)*12;
		g_enemies[i].m_z = 32;
	}

	for( i = 0 ; i < 8 ; i++ )
	{
		g_enemies[i+8].m_status = 1;
		g_enemies[i+8].m_x = -6 + (i&0x3)*4;
		g_enemies[i+8].m_y = -6 + (i>>2)*12;
		g_enemies[i+8].m_z = 64;
	}

	for( i = 0 ; i < 8 ; i++ )
	{
		g_enemies[i+16].m_status = 1;
		g_enemies[i+16].m_x = -6 + (i&0x3)*4;
		g_enemies[i+16].m_y = -6 + (i>>2)*12;
		g_enemies[i+16].m_z = 48;
	}
	*/
	g_nTimer = 0;
	nTimer = g_nTimer;

	for(;;)
	{
		{
			u8 stick = msxBiosGetStick(0);

			if( stick == 8 || stick == 1 || stick == 2 )
			{
				if( g_py > -10 )
				{
					g_py -= 1;
				}

				if( g_vyy < 8 )
				{
					g_vyy++;
				}
			}
			else if( stick == 4 || stick == 5 || stick == 6 )
			{
				if( g_py < 10 )
				{
					g_py += 1;
				}

				if( g_vyy > -8 )
				{
					g_vyy--;
				}
			}
			else
			{
				if( g_vy > 0 )
				{
					g_vyy--;
				}
				else if( g_vy < 0 )
				{
					g_vyy++;
				}
			}

			if( stick == 2 || stick == 3 || stick == 4 )
			{
				if (g_px < 14)
				{
					g_px += 1;

					if (g_vxx > -8)
					{
						g_vxx--;
					}
				}
				else
				{
					if (g_vx > 0)
					{
						g_vxx--;
					}
					else if (g_vx < 0)
					{
						g_vxx++;
					}
				}
				g_pattern = 4*4;
			}
			else if( stick == 6 || stick == 7 || stick == 8 )
			{
				if (g_px > -14)
				{
					g_px -= 1;

					if (g_vxx < 8)
					{
						g_vxx++;
					}
				}
				else
				{
					if (g_vx > 0)
					{
						g_vxx--;
					}
					else if (g_vx < 0)
					{
						g_vxx++;
					}
				}
				g_pattern = 3 * 4;
			}
			else
			{
				if( g_vx > 0 )
				{
					g_vxx--;
				}
				else if( g_vx < 0 )
				{
					g_vxx++;
				}
			}

			g_vx = g_vxx >> 2;
			g_vy = g_vyy >> 2;
		}

		{
			u8 button = msxBiosGetTrigger( 0 );
			struct SBeam *pBeam = g_beams;

			//if( g_nPrevButton == 0 && button != 0 )
			if( button != 0 )
			{
				for( i = 0 ; i < BEAM_COUNT ; i++, pBeam++ )
				{
					if( pBeam->m_status == 0 )
					{
						pBeam->m_status = 1;
						pBeam->m_x = g_px;
						pBeam->m_y = g_py;
						pBeam->m_z = g_pz - 4;
						break;
					}
				}
			}

			g_nPrevButton = button;
		}

		{
			struct SBeam *pBeam = g_beams;

			for( i = 0 ; i < BEAM_COUNT ; i++, pBeam++ )
			{
				if( pBeam->m_status )
				{
					pBeam->m_x += g_vx;
					pBeam->m_y += g_vy;
					pBeam->m_z += 8;
					if( pBeam->m_z > 256 )
					{
						pBeam->m_status = 0;
						SpbSetSprite( 28 + i, 0, 209, 0, 0 );
					}
					else
					{
						//u8 p = s_beamPattern[CLAMP( PERS_X(4,pBeam->m_z)-128, 0, 31)];
						//SpbSetSprite( 28 + i, PERS_X(pBeam->m_x,pBeam->m_z) - 16, PERS_Y(pBeam->m_y,pBeam->m_z) - 16, p, 7 );
						SetSprite3d( 28+i, pBeam->m_x, pBeam->m_y, pBeam->m_z, s_beamPattern, 4, 7 );
					}
				}
				else
				{
				}
			}
		}

		{
			struct SEnemy *pEnemy = g_enemies;

			for( i = 0 ; i < ENEMY_COUNT ; i++, pEnemy++ )
			{
				if( pEnemy->m_status == 0 )
				{
					pEnemy->m_status = 1;
					pEnemy->m_x = (msxRandGet16() & 0x3f) - 32;
					pEnemy->m_y = ((msxRandGet16() & 0xff) >> 2) - 32;
					pEnemy->m_z = 512;//PERS_Z_FAR;
				}
				else if( pEnemy->m_status == 1 )
				{
					pEnemy->m_x += g_vx;
					pEnemy->m_y += g_vy;
					pEnemy->m_z -= 8;
					if( pEnemy->m_z < (PERS_Z_NEAR) )
					{
						pEnemy->m_status = 0;
						SpbSetSprite( 8 + i, 0, 209, 0, 0 );
					}
					else
					{
						u8 j;
						struct SBeam *pBeam = g_beams;
						for( j = 0 ; j < BEAM_COUNT ; j++, pBeam++ )
						{
							if( pBeam->m_status )
							{
								if( pEnemy->m_x < (pBeam->m_x - 6) || (pBeam->m_x + 6) < pEnemy->m_x )
								{
									continue;
								}

								if( pEnemy->m_y < (pBeam->m_y - 6) || (pBeam->m_y + 6) < pEnemy->m_y )
								{
									continue;
								}

								
								if( pEnemy->m_z < (pBeam->m_z - 8) || (pBeam->m_z + 8) < pEnemy->m_z )
								{
									continue;
								}

								pEnemy->m_status = 2;
								pEnemy->pad = 0;
								//SpbSetSprite( 8 + i, 0, 209, 0, 0 );

								pBeam->m_status = 0;
								SpbSetSprite( 28 + j, 0, 209, 0, 0 );
							}
						}

						//if( pEnemy->m_status )
						{
							//SetSprite3d( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, s_enemyPattern2, 8, 6 );
							SetSprite3d( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, s_enemyPattern3, 8, 2 );
							//SetSprite3d( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, s_enemyShotPattern, 2, 13 );
						}
					}
				}
				else
				{
					//. 爆発.
					if( pEnemy->pad < 2 )
					{
						u8 p = ((pEnemy->m_z < 128) ? 24*4 : 27*4);
						SetSprite3d2( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, p, 16, 15 );
						//u8 p = ((pEnemy->m_z < 64) ? 24*4 : 27*4);
						//SpbSetSprite( 8 + i, PERS_X(pEnemy->m_x,pEnemy->m_z) - 16, PERS_Y(pEnemy->m_y,pEnemy->m_z)-16, p, 15 );
						pEnemy->pad++;
					}
					else if( pEnemy->pad < 4 )
					{
						u8 p = ((pEnemy->m_z < 128) ? 25*4 : 28*4);
						SetSprite3d2( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, p, 16, 11 );
						//u8 p = ((pEnemy->m_z < 64) ? 25*4 : 28*4);
						//SpbSetSprite( 8 + i, PERS_X(pEnemy->m_x,pEnemy->m_z) - 16, PERS_Y(pEnemy->m_y,pEnemy->m_z)-16, p, 11 );
						pEnemy->pad++;
					}
					else if( pEnemy->pad < 6 )
					{
						u8 p = ((pEnemy->m_z < 128) ? 26*4 : 29*4);
						SetSprite3d2( 8+i,pEnemy->m_x, pEnemy->m_y, pEnemy->m_z, p, 16, 8 );
						//u8 p = ((pEnemy->m_z < 64) ? 26*4 : 29*4);
						//SpbSetSprite( 8 + i, PERS_X(pEnemy->m_x,pEnemy->m_z) - 16, PERS_Y(pEnemy->m_y,pEnemy->m_z)-16, p, 8 );
						pEnemy->pad++;
					}
					else
					{
						pEnemy->m_status = 0;
						SpbSetSprite( 8 + i, 0, 209, 0, 0 );
					}
				}
			}
		}

		{
			struct SStar *pStar = g_stars;

			for (i = 0; i < STAR_COUNT; i++, pStar++)
			{
				if (pStar->m_z < PERS_Z_NEAR)
				{
					pStar->m_x = (msxRandGet16() & 0x7f) - 64;
					pStar->m_y = ((msxRandGet16() & 0x1ff) >> 2) - 64;
					pStar->m_z = 256;//PERS_Z_FAR;
				}

				pStar->m_x += g_vx;
				pStar->m_y += g_vy;
				pStar->m_z -= 8;

				SetSprite3d(8 + ENEMY_COUNT + i, pStar->m_x, pStar->m_y, pStar->m_z, s_starPattern, 16, 15);
			}
		}


		SpbSetSprite( 0,  PERS_X(g_px,g_pz)-16, PERS_Y(g_py,g_pz)-16, g_pattern, 15 );
		SetSprite3d( 31, g_px, g_py, 256, s_crossPattern, 16, 2 );
//		SpbSetSprite( 31, PERS_X(g_px,256)-16, PERS_Y(g_py,256)-16, 2*4, 2 );
//		SpbSetSprite( 1, g_px, g_py,    32, 15 );


//		SpbSendSprite(0x3800);
//		while( g_nTimer <= (nTimer+1) )
		while( g_nTimer == nTimer )
		{
		}
		
		nTimer = g_nTimer;
	}
}



