//-------------------------------------------------------------------
// 描画システム.
//-------------------------------------------------------------------
#ifndef UX_RENDER_H
#define UX_RENDER_H

#include "msx-types.h"

#define DUX_SCREEN_CENTER_X (128)
#define DUX_SCREEN_CENTER_Y (106)

#define DUX_SCREEN_LEFT   (16)
#define DUX_SCREEN_RIGHT  (256 - 16)
#define DUX_SCREEN_TOP    (16)
#define DUX_SCREEN_BOTTOM (212 - 16)

#define DUX_SCREEN_LEFT_SIZE   (112 - 1)
#define DUX_SCREEN_RIGHT_SIZE  (112 - 1)
#define DUX_SCREEN_TOP_SIZE    (90 - 1)
#define DUX_SCREEN_BOTTOM_SIZE (90 - 1)

//-------------------------------------------------------------------
//. 外で定義するモデル型.
//-------------------------------------------------------------------
struct SUxObject
{
	u8 nXPlusSize;
	u8 nXMinusSize;
	u8 nYPlusSize;
	u8 nYMinusSize;
	u8 nZPlusSize;
	u8 nZMinusSize;
	u8 nbVertices;
	s8 *pVertices;
	u8 nbIndices;
	u8 *pIndices;
};

//-------------------------------------------------------------------
//. 外でラインを描くときのインターフェース.
//-------------------------------------------------------------------
struct SUxLine
{
	u8 sx, sy, ex, ey, nColor;
};

//-------------------------------------------------------------------
//. 内部頂点.
//-------------------------------------------------------------------
enum EUxVertexType
{
	kUxVertexType_2d,
	kUxVertexType_3d,
};

struct SUxVertex
{
	u8 nClip, eType;
	s16 s16X, s16Y, s16Z;
};

//-------------------------------------------------------------------
//. 内部コマンドバッファ.
//-------------------------------------------------------------------
enum EUxCommand
{
	kUxCommand_RenderObjectNoClip,
	kUxCommand_RenderObjectClip,
	kUxCommand_End,
};

struct SUxCommand
{
	u8 eType;
	u8 pad[7];
};

//. オブジェクト描画コマンド.
struct SUxCommandObject
{
	u8 eType;
	u8 pad0;
	struct SUxVertex *pVertex;
	struct SUxObject *pObject;
};

//. ライン描画コマンド.
struct SUxCommandLine
{
	u8 eType;
	u8 sx, sy, ex, ey;
	u8 nColor;
};

//-------------------------------------------------------------------
//. システムコンテキスト.
//-------------------------------------------------------------------
struct SUxRenderContext
{
	//. 登録位置.
	s16 nPosX;
	s16 nPosY;
	s16 nPosZ;
	//. 登録オブジェクト.
	struct SUxObject *pTarget ;

	//. 描画ページ.
	u8 nRenderPage;

	//. 変換済み頂点バッファ.
	u8 nbVertices;
	struct SUxVertex* pVertexBuffer; //. 先頭.
	struct SUxVertex* pVertex; //. 現在位置.

	//. コマンド.
	u8 nbCommands;
	struct SUxCommand* pCommandBuffer; //. 先頭.
	struct SUxCommand* pCommand; //. 現在位置.
};

extern struct SUxRenderContext g_render;

//-------------------------------------------------------------------
// API.
//-------------------------------------------------------------------
extern void UxRenderInit();

extern void UxRenderClear();
extern void UxRenderAddObject();
extern struct SUxLine* UxRenderAddLine2d();

extern void UxRenderFlip();

//extern void UxRenderSetRenderPage(u8 nPage);
//extern void UxRenderSetRenderColor(u8 nColor);

#endif
