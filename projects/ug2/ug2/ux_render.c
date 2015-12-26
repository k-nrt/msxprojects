//-------------------------------------------------------------------
// 描画システム.
//-------------------------------------------------------------------
#include "ux_render.h"
#include "vdp_command.h"
//#include "msx-disk-wrapper.h"

//static void UxRenderObjectNoClip( struct SUxVertex *pVertex, u8 *pIndex, u8 nbIndices );

//-------------------------------------------------------------------
//. システムコンテキスト.
//-------------------------------------------------------------------
struct SUxRenderContext g_render;
#if 0

u16 g_pers[64 * 64];

//-------------------------------------------------------------------
// システム初期化.
//-------------------------------------------------------------------
void UxRenderInit()
{
	u16 i;
	u8 *p;
	//. パース変換テーブル作成.
	u8 x, z;
	u16 *pDst = g_pers;// (u16*)0x4000;
	for( z = 0 ; z < 64 ; z++ )
	{
		u16 zz = (u16)z + 64;
		for( x = 0 ; x < 128 ; x+= 2 )
		{
			u16 nPers;
			u16 xx = (u16)x;
			nPers = (xx*160)/zz;

			if( nPers > 32767 )
			{
				nPers = 0xffff;
			}
			*pDst = nPers;
			pDst++;
		}
	}

	//. オブジェクト登録.
	g_render.nPosX = 0;
	g_render.nPosY = 0;
	g_render.nPosZ = 0;
	g_render.pTarget = NULL;

	//. 描画ページ.
	g_render.nRenderPage = 0;

	//. 変換済み頂点バッファ.
	g_render.nbVertices = 0;
	g_render.pVertexBuffer = (struct SUxVertex*) 0x1000;
	g_render.pVertex = g_render.pVertexBuffer;

	//. コマンド.
	g_render.nbCommands = 0;
	g_render.pCommandBuffer = (struct SUxCommand*) (0x1000 + sizeof(struct SUxVertex) * 256);
	g_render.pCommand = g_render.pCommandBuffer;
}

//-------------------------------------------------------------------
// 画面クリア.
//-------------------------------------------------------------------
void UxRenderClear()
{
	//msxDiskVDPWait();
	//msxDiskVDPClear( DUX_SCREEN_LEFT, DUX_SCREEN_TOP, g_render.nRenderPage, DUX_SCREEN_RIGHT - DUX_SCREEN_LEFT + 2, DUX_SCREEN_BOTTOM - DUX_SCREEN_TOP + 2, 0 );
}

//-------------------------------------------------------------------
// オブジェクト登録.
//-------------------------------------------------------------------
void UxRenderAddObject()
{
	static s8                      *pSrcVertex = NULL;// g_render.pTarget->pVertices;
	static struct SUxCommandObject *pDstObject = NULL;// (struct SUxCommandObject*) g_render.pCommand;
	static u8 nObjectClip = 0;
	static s16 px = 0;// g_render.nPosX;
	static s16 py = 0;// g_render.nPosY;
	static s16 pz = 0;// g_render.nPosZ;
	static const s16 *pPers = (s16*) 0x4000;

	u8 i;
	u8 nClip;
	s16 x, y, z, sx, sy;

	pSrcVertex = g_render.pTarget->pVertices;
	pDstObject = (struct SUxCommandObject*) g_render.pCommand;
	nObjectClip = 0;
	px = g_render.nPosX;
	py = g_render.nPosY;
	pz = g_render.nPosZ;
	pPers = g_pers;// (s16*)0x4000;

	pDstObject->pVertex = g_render.pVertex;
	pDstObject->pObject = g_render.pTarget;

	for( i = 0 ; i < g_render.pTarget->nbVertices ; i++ )
	{
		x = ((s16)pSrcVertex[0]) + px;
		y = ((s16)pSrcVertex[1]) + py;
		z = ((s16)pSrcVertex[2]) + pz;

		for (; z < 64;)
		{
			x <<= 1;
			y <<= 1;
			z <<= 1;
		}

		for(; z > 127 || x > 127 || y > 127 ; )
		{
			x >>= 1;
			y >>= 1;
			z >>= 1;
		}
		z -= 64;

		nClip = 0;

		if( z > 0 )
		{
			const s16 *pPersZ = pPers + (z << 6);
			if( x >= 0 )
			{
				if( x & 0x1 )
				{
					x >>= 1;
					sx = (pPersZ[x] + pPersZ[x+1])>>1;
				}
				else
				{
					sx = pPersZ[x>>1];
				}

				if( sx > DUX_SCREEN_RIGHT_SIZE )
				{
					nClip |= 1;
				}
			}
			else
			{
				x = -x;
				if( x & 0x1 )
				{
					x >>= 1;
					sx = (pPersZ[x] + pPersZ[x+1])>>1;
				}
				else
				{
					sx = pPersZ[x>>1];
				}

				if( sx > DUX_SCREEN_LEFT_SIZE )
				{
					nClip |= 2;
				}
				sx = -sx;
			}

			if( y >= 0 )
			{
				if( y & 0x1 )
				{
					y >>= 1;
					sy = (pPersZ[y] + pPersZ[y+1])>>1;
				}
				else
				{
					sy = pPersZ[y>>1];
				}

				if( sy > DUX_SCREEN_BOTTOM_SIZE )
				{
					nClip |= 4;
				}
			}
			else
			{
				y = -y;
				if( y & 0x1 )
				{
					y >>= 1;
					sy = (pPersZ[y] + pPersZ[y+1])>>1;
				}
				else
				{
					sy = pPersZ[y>>1];
				}

				if( sy > DUX_SCREEN_TOP_SIZE )
				{
					nClip |= 8;
				}
				sy = -sy;
			}
			g_render.pVertex->nClip = nClip;
			g_render.pVertex->eType = kUxVertexType_2d;
			g_render.pVertex->s16X = sx + DUX_SCREEN_CENTER_X;
			g_render.pVertex->s16Y = sy + DUX_SCREEN_CENTER_Y;
			//g_render.pVertex->s16X = px + pSrcVertex[0] + DUX_SCREEN_CENTER_X;
			//g_render.pVertex->s16Y = py + pSrcVertex[1] + DUX_SCREEN_CENTER_Y;
		}
		else
		{
			nClip = 0xf;
			g_render.pVertex->nClip = 0xf;
			g_render.pVertex->eType = kUxVertexType_3d;
			g_render.pVertex->s16X = x;
			g_render.pVertex->s16Y = y;
			g_render.pVertex->s16Z = z;
		}

		nObjectClip |= nClip;

		pSrcVertex += 3;
		g_render.pVertex++;
		g_render.nbVertices++;
	}
	
	if( nObjectClip )
	{
		pDstObject->eType = kUxCommand_RenderObjectClip;
	}
	else
	{
		pDstObject->eType = kUxCommand_RenderObjectNoClip;
	}

	g_render.pCommand++;
}

//-------------------------------------------------------------------
// 2D ライン登録.
//-------------------------------------------------------------------
struct SUxLine* UxRenderAddLine2d()
{
	return NULL;
}

//-------------------------------------------------------------------
// 描画とフリップ.
//-------------------------------------------------------------------
void UxRenderFlip()
{
	static struct SUxCommand *pCommand;

	//msxDiskVDPWait();
	UxRenderSetRenderPage(g_render.nRenderPage);
	UxRenderSetRenderColor(0xff);

	//. 最後に終了を書いてからコマンド解釈する.
	g_render.pCommand->eType = kUxCommand_End;

	for( pCommand = g_render.pCommandBuffer ; pCommand->eType != kUxCommand_End ; pCommand++ )
	{
		switch( pCommand->eType )
		{
		case kUxCommand_RenderObjectClip:
			{
				u8 i;
				//struct SUxObject *pObject = ((struct SUxCommandObject*)pCommand)->pObject;
				u8 nbIndices = ((struct SUxCommandObject*)pCommand)->pObject->nbIndices;
				u8 *pIndex   = ((struct SUxCommandObject*)pCommand)->pObject->pIndices;
				struct SUxVertex *pVertex = ((struct SUxCommandObject*)pCommand)->pVertex;
				for (i = 0; i < nbIndices; i++)
				{
					s16 sx, sy, ex, ey;
					struct SUxVertex *pStart, *pEnd;

					pStart = (struct SUxVertex*)(((u8*)pVertex) + *pIndex);
					pIndex++;
					pEnd   = (struct SUxVertex*)(((u8*)pVertex) + *pIndex);
					pIndex++;

					if( pStart->nClip & pEnd->nClip & 0xf )
					{
						continue;
					}

					if( pStart->nClip == 0xf || pStart->nClip == 0xf )
					{
						continue;
					}

					sx = pStart->s16X;
					sy = pStart->s16Y;
					ex = pEnd->s16X;
					ey = pEnd->s16Y;

					if( sx > ex )
					{
						s16 tx = sx;
						s16 ty = sy;
						sx = ex;
						sy = ey;
						ex = tx;
						ey = ty;
					}

					if( sx < DUX_SCREEN_LEFT && DUX_SCREEN_LEFT <= ex )
					{
						s16 tx = ex;
						s16 ty = ey;
						for(;;)
						{
							s16 x = (sx + tx)>>1;
							s16 y = (sy + ty)>>1;

							if( x < DUX_SCREEN_LEFT )
							{
								sx = x;
								sy = y;
							}
							else
							{
								tx = x;
								ty = y;
							}

							if( sx == DUX_SCREEN_LEFT )
							{
								break;
							}

							if( tx == DUX_SCREEN_LEFT )
							{
								sx = tx;
								sy = ty;
								break;
							}

						}
					}
					
					if( sx < DUX_SCREEN_RIGHT && DUX_SCREEN_RIGHT <= ex )
					{
						s16 tx = sx;
						s16 ty = sy;
						for(;;)
						{
							s16 x = (tx + ex)>>1;
							s16 y = (ty + ey)>>1;

							if( x < DUX_SCREEN_RIGHT )
							{
								tx = x;
								ty = y;
							}
							else
							{
								ex = x;
								ey = y;
							}

							if( ex == DUX_SCREEN_RIGHT )
							{
								break;
							}

							if( tx == DUX_SCREEN_RIGHT )
							{
								ex = tx;
								ey = ty;
								break;
							}
						}
					}

					if( sy > ey )
					{
						s16 tx = sx;
						s16 ty = sy;
						sx = ex;
						sy = ey;
						ex = tx;
						ey = ty;
					}

					if( sy < DUX_SCREEN_TOP && DUX_SCREEN_TOP <= ey )
					{
						s16 tx = ex;
						s16 ty = ey;
						for(;;)
						{
							s16 x = (sx + tx)>>1;
							s16 y = (sy + ty)>>1;

							if( y < DUX_SCREEN_TOP )
							{
								sx = x;
								sy = y;
							}
							else
							{
								tx = x;
								ty = y;
							}

							if( sy == DUX_SCREEN_TOP )
							{
								break;
							}

							if( ty == DUX_SCREEN_TOP )
							{
								sx = tx;
								sy = ty;
								break;
							}
						}
					}

					if( sy < DUX_SCREEN_BOTTOM && DUX_SCREEN_BOTTOM <= ey )
					{
						s16 tx = sx;
						s16 ty = sy;
						for(;;)
						{
							s16 x = (tx + ex)>>1;
							s16 y = (ty + ey)>>1;

							if( y < DUX_SCREEN_BOTTOM )
							{
								tx = x;
								ty = y;
							}
							else
							{
								ex = x;
								ey = y;
							}

							if( ey == DUX_SCREEN_BOTTOM )
							{
								break;
							}

							if( ty == DUX_SCREEN_BOTTOM )
							{
								ex = tx;
								ey = ty;
								break;
							}
						}
					}

					VDPWaitLine( (u8) sx, (u8) sy, (u8) ex, (u8) ey, 0xff, g_render.nRenderPage );
				}
			}
			break;

		case kUxCommand_RenderObjectNoClip:
			{
				u8 nbIndices = ((struct SUxCommandObject*)pCommand)->pObject->nbIndices;
				u8 *pIndex   = ((struct SUxCommandObject*)pCommand)->pObject->pIndices;
				struct SUxVertex *pVertex = ((struct SUxCommandObject*)pCommand)->pVertex;
				UxRenderObjectNoClip( pVertex, pIndex, nbIndices );
			}
			break;

		default:
			break;
		}
	}

	//. ディスプレイページ変更.
	//VDPSetPage( g_render.nRenderPage );

	g_render.nbVertices = 0;
	g_render.pVertex = g_render.pVertexBuffer;

	g_render.nbCommands = 0;
	g_render.pCommand = g_render.pCommandBuffer;
	g_render.pCommand->eType = kUxCommand_End; //. エンドを入れとく.

	//g_render.nRenderPage++;
	//g_render.nRenderPage &= 1;
}

/*
void UxRenderObjectNoClip( struct SUxVertex *pVertex, u8 *pIndex, u8 nbIndices )
{
	u8 i;
	for( i = 0 ; i < nbIndices ; i++ )
	{
		struct SUxVertex *pStart, *pEnd;
		pStart = pVertex + *pIndex;
		pIndex++;
		pEnd   = pVertex + *pIndex;
		pIndex++;
//		msxDiskVDPWait();
//		msxDiskVDPLine( pStart->s16X, pStart->s16Y, pEnd->s16X, pEnd->s16Y, 0xff, g_render.nRenderPage );
		msxDiskVDPWaitLine( pStart->s16X, pStart->s16Y, pEnd->s16X, pEnd->s16Y, 0xff, g_render.nRenderPage );
	}
}
*/
#endif 
