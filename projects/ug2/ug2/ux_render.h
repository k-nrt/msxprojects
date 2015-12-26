//-------------------------------------------------------------------
// �`��V�X�e��.
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
//. �O�Œ�`���郂�f���^.
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
//. �O�Ń��C����`���Ƃ��̃C���^�[�t�F�[�X.
//-------------------------------------------------------------------
struct SUxLine
{
	u8 sx, sy, ex, ey, nColor;
};

//-------------------------------------------------------------------
//. �������_.
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
//. �����R�}���h�o�b�t�@.
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

//. �I�u�W�F�N�g�`��R�}���h.
struct SUxCommandObject
{
	u8 eType;
	u8 pad0;
	struct SUxVertex *pVertex;
	struct SUxObject *pObject;
};

//. ���C���`��R�}���h.
struct SUxCommandLine
{
	u8 eType;
	u8 sx, sy, ex, ey;
	u8 nColor;
};

//-------------------------------------------------------------------
//. �V�X�e���R���e�L�X�g.
//-------------------------------------------------------------------
struct SUxRenderContext
{
	//. �o�^�ʒu.
	s16 nPosX;
	s16 nPosY;
	s16 nPosZ;
	//. �o�^�I�u�W�F�N�g.
	struct SUxObject *pTarget ;

	//. �`��y�[�W.
	u8 nRenderPage;

	//. �ϊ��ςݒ��_�o�b�t�@.
	u8 nbVertices;
	struct SUxVertex* pVertexBuffer; //. �擪.
	struct SUxVertex* pVertex; //. ���݈ʒu.

	//. �R�}���h.
	u8 nbCommands;
	struct SUxCommand* pCommandBuffer; //. �擪.
	struct SUxCommand* pCommand; //. ���݈ʒu.
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
