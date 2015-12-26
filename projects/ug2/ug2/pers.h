#ifndef PERS_H
#define PERS_H
#include <msx-types.h>
#include "vec_math.h"

extern void UgxInitialize();

extern u8 UgxGetValueU8(u8 x, u8 z);
extern u8 UgxGetValueU16(u16 x, u16 z);

typedef s8x3 SUgxModelVertex;

typedef struct
{
	u8 nXPlusSize;
	u8 nXMinusSize;

	u8 nYPlusSize;
	u8 nYMinusSize;

	u8 nZPlusSize;
	u8 nZMinusSize;

	u8 nbVertices;
	u8 nbLines;

	SUgxModelVertex *pVertices;
	u8 *pIndices;
} SUgxModel;

//typedef u16 UgxModelHandle;
typedef u16 UgxBuffer;

enum EUgxPrimitive
{
	kUgxPrimitive_Points,
	kUgxPrimitive_Lines,
	kUgxPrimitive_Rectangles,
	kUgxPrimitive_LineStrip,
};

#define UGX_CENTER_X (128)
#define UGX_CENTER_Y (106)

#define UGX_LEFT   (UGX_CENTER_X-16)
#define UGX_RIGHT  (UGX_CENTER_X-16-1)
#define UGX_TOP    (UGX_CENTER_Y - 32-1)
#define UGX_BOTTOM (UGX_CENTER_Y-32-1)

#define UGX_SCREEN_LEFT   (UGX_CENTER_X-UGX_LEFT)
#define UGX_SCREEN_RIGHT  (UGX_CENTER_X+UGX_RIGHT)
#define UGX_SCREEN_TOP    (UGX_CENTER_Y-UGX_TOP)
#define UGX_SCREEN_BOTTOM (UGX_CENTER_Y+UGX_BOTTOM)

UgxBuffer UgxCreateIndexBuffer(const u8 *pBuffer, u8 nbIndices);
UgxBuffer UgxCreateVertexBuffer(const SUgxModelVertex *pVertices, u8 nbVertices, enum ERotationOrder eRotationOrder, u8 u8RotX, u8 u8RotY, u8 u8RotZ, u16 u16Scale);

void UgxClearScreen();

void UgxBeginFrame();

extern s16x3 g_v3ModelPosition;

#define UgxSetModelPosition(x,y,z) Vec3Set(g_v3ModelPosition,x,y,z)
void UgxDrawModel(UgxBuffer vertices, u8 nbVertices, UgxBuffer indices, u8 nbLines);

void UgxBegin(enum EUgxPrimitive ePrimitive);
void UgxColor(u8 color);
void UgxVertex2u8(u8 x, u8 y);
void UgxEnd();

void UgxEndFrame();

void UgxFlushCommands();

void UgxDumpContext();

#endif //PERS_H
