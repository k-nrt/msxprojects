#include "pers.h"
#include "slot.h"
#include "math.h"
#include "vdp_command.h"
#include "sincos.h"
#include <msx-bios-wrapper.h>

typedef struct 
{
	u8 sx, sy;
	//	s16 x, y, z;
	//	u8 clip;
	//	u8 pad;
} SUgxInternalVertex;

//-------------------------------------------------------------------
//. 内部コマンドバッファ.
//-------------------------------------------------------------------
enum EUgxCommand
{
	kUgxCommand_RenderObjectNoClip,
	kUgxCommand_RenderObjectClip,
	kUgxCommand_RenderPrimitive,
	kUgxCommand_End,
};

typedef struct
{
	u8 eType;
	u8 pad[7];
} SUgxCommand;

//. オブジェクト描画コマンド.
typedef struct 
{
	u8 eType;
	u8 u8Color;
	void *pVertex;
	void *pIndex;
	u8 nbLines;
	//SUgxModel *pObject;
} SUgxCommandRenderObject;

//. ライン描画コマンド.
typedef struct
{
	u8 eType;
	u8 ePrimitive;
	u8 color;
	void *pVertices;
	u8 nbVertices;
} SUgxCommandPrimitive;


//. 透視変換テーブル.
struct PERS
{
	u8 m_u16_x1[16][256];
	u8 m_u16_x16[16][256];
};

typedef struct
{
	u8 u8Page1RomSlot;
	u8 u8Page1RamSlot;
//	struct PERS*	pPers;

	u16 m_u16InternalVertexHeap;
	u16 m_u16InternalModelHeap;
	u16 m_u16InternalVRAMHeap;

	SUgxCommand *m_pCommand;

	SUgxCommandPrimitive m_commandPrimitive;
} SUgxContext;

SUgxContext s_ugxContext;

#define INTERNAL_VERTEX_BUFFER_SIZE		(0x200)
#define INTERNAL_COMMAND_BUFFER_SIZE	(0x200)
#define INTERNAL_MODEL_BUFFER_SIZE		(0x4000 - (INTERNAL_VERTEX_BUFFER_SIZE + INTERNAL_COMMAND_BUFFER_SIZE))

#define INTERNAL_BUFFER_ADDRESS			(0x8000)
#define INTERNAL_VERTEX_BUFFER_ADDRESS	(INTERNAL_BUFFER_ADDRESS)
#define INTERNAL_COMMAND_BUFFER_ADDRESS	(INTERNAL_VERTEX_BUFFER_ADDRESS + INTERNAL_VERTEX_BUFFER_SIZE)
#define INTERNAL_MODEL_BUFFER_ADDRESS	(INTERNAL_COMMAND_BUFFER_ADDRESS + INTERNAL_COMMAND_BUFFER_SIZE)

#define INTERNAL_PERS_TABLE_ADDRESS (0xc000)

#define g_pPers ((struct PERS*)INTERNAL_PERS_TABLE_ADDRESS)
//#define g_pVertices ((struct PERS*)kPersTableAddress)

#define RAM_ENABLE() SLOTEnableDI(s_ugxContext.u8Page1RamSlot,INTERNAL_BUFFER_ADDRESS)
#define RAM_DISABLE() SLOTEnableDI(s_ugxContext.u8Page1RomSlot,INTERNAL_BUFFER_ADDRESS); INT_ENABLE()

void UgxDumpContext()
{
	VDPSetPrintPosition(0, 8);
	VDPSetForegroundColor(0xff);

	VDPPrintCore("UGX CONTEXT\n");
	VDPPrintCore("SLOT\n");
	VDPPrintF("%s %x\n", "PAGE1 ROM", s_ugxContext.u8Page1RomSlot);
	VDPPrintF("%s %x\n", "PAGE1 RAM", s_ugxContext.u8Page1RamSlot);

	VDPPrintCore("INTERNAL HEAP SIZE\n");
	VDPPrintF("%s %x/%x\n", "VERTEX ", s_ugxContext.m_u16InternalVertexHeap - INTERNAL_VERTEX_BUFFER_ADDRESS, INTERNAL_VERTEX_BUFFER_SIZE);
	VDPPrintF("%s %x/%x\n", "MODEL  ", s_ugxContext.m_u16InternalModelHeap - INTERNAL_MODEL_BUFFER_ADDRESS, INTERNAL_MODEL_BUFFER_SIZE);
	VDPPrintF("%s %x/%x\n", "COMMAND", (u16)s_ugxContext.m_pCommand - INTERNAL_COMMAND_BUFFER_ADDRESS, INTERNAL_COMMAND_BUFFER_SIZE);
}


void UgxInitialize()
{
	u16 x, z;
	u8 slot;
//	struct PERS *pPers = (struct  PERS*) PERS_g_pPers;
//	PERS_g_pPers = (struct PERS*) 0x8000;
	s_ugxContext.u8Page1RomSlot = SLOTGetSlotInfo(INTERNAL_BUFFER_ADDRESS);
	slot = s_ugxContext.u8Page1RamSlot = SLOTFindRAMSlot(INTERNAL_BUFFER_ADDRESS);

	//SLOTEnableDI(slot, kPersTableAddress);

	for (z = 1; z < 256; z++)
	{
		for (x = 0; x < 16; x++)
		{
			u16 x0 = (x * 160) / z;

			if (x0 < 255)
			{
				g_pPers->m_u16_x1[x][z] = x0;
			}
			else 
			{
				g_pPers->m_u16_x1[x][z] = 0xff;
			}
		}

		for (x = 0; x < 16; x++)
		{
			u16 x1 = (x * 16 * 160) / z;

			if ( x1 < 255 )
			{
				g_pPers->m_u16_x16[x][z] = x1;
			}
			else
			{
				g_pPers->m_u16_x16[x][z] = 0xff;
			}
		}
	}

	//SLOTEnableDI(s_ugxContext.u8Page1RomSlot, kPersTableAddress);
	//INT_ENABLE();

	s_ugxContext.m_u16InternalModelHeap = INTERNAL_MODEL_BUFFER_ADDRESS;
	s_ugxContext.m_u16InternalVRAMHeap = 0;
}

#if 0 
u8 PERSGetValueU8(u8 u8X, u8 u8Z)
{
//	struct PERS *pPers = (struct  PERS*) PERS_g_pPers;
	if (u8X < 16)
	{
		u8 x0 = u8X & 0xf;
		return pPers->m_u16_x1[x0][u8Z];
	}
	else
	{
		u8 x0 = u8X & 0xf;
		u8 x1 = (u8X >> 4) & 0xf;

		return pPers->m_u16_x1[x0][u8Z] + pPers->m_u16_x16[x1][u8Z];
	}
}
#endif

u8 UgxGetValueU16(u16 u16X, u16 u16Z)
{
	if (u16X < 256 && u16Z < 256)
	{
		u8 result;
		//SLOTEnableDI(s_ugxContext.u8Page1RamSlot, kPersTableAddress);
		result = UgxGetValueU8(u16X, u16Z);
		//SLOTEnableDI(s_ugxContext.u8Page1RomSlot, kPersTableAddress);
		//INT_ENABLE();

		return result;
	}

	u16X >>= 1;
	u16Z >>= 1;

	if (u16X < 256 && u16Z < 256)
	{
		u8  result;

		//SLOTEnableDI(s_ugxContext.u8Page1RamSlot, kPersTableAddress);
		result = UgxGetValueU8(u16X, u16Z);
		//SLOTEnableDI(s_ugxContext.u8Page1RomSlot, kPersTableAddress);
		//INT_ENABLE();

		return result;
	}

	return 0xff;
}

void* UgxAllocateFromModelHeap(u8 sizeInByte)
{
	void *pResult = (void*) s_ugxContext.m_u16InternalModelHeap;
	s_ugxContext.m_u16InternalModelHeap += sizeInByte;
	return pResult;
}

void* UgxAllocateFromVRAMHeap(u8 sizeInByte)
{
	void *pResult = (void*)s_ugxContext.m_u16InternalVRAMHeap;
	s_ugxContext.m_u16InternalVRAMHeap += sizeInByte;
	return pResult;
}

void* UgxAllocateFromVertexHeap(u8 sizeInByte)
{
	void *pResult = (void*) s_ugxContext.m_u16InternalVertexHeap;
	s_ugxContext.m_u16InternalVertexHeap += sizeInByte;
	return pResult;
}


void UgxCopyToIntenalRAM8Internal(void *pDst, const void* pSrc, u8 sizeInBytes)
{
	u8 i;
	u8 slotRAM = s_ugxContext.u8Page1RamSlot;
	u8 *pU8Dst = pDst;
	const u8 *pU8Src = pSrc;
	for (i = 0; i < sizeInBytes; i++, pU8Dst++, pU8Src++)
	{
		SLOTWriteDI(slotRAM, (u16)pU8Dst, *pU8Src);
	}

	//INT_ENABLE();
}


void UgxCopyToVRAM8Internal(void *pDst, const void* pSrc, u8 sizeInBytes);
/*
{
	register u8 i;
	register u8 *pU8Dst = pDst;
	register u8 *pU8Src = pSrc;
	VDPSetActivePage(2);

	for (i = 0; i < sizeInBytes; i++, pU8Dst++, pU8Src++)
	{
		msx2BiosWriteVRAM( (u16)pU8Dst, *pU8Src);
		//	SLOTWriteDI(slotRAM, (u16)pU8Dst, *pU8Src);
	}
}
*/
void UgxCopy8Internal(void *pDst, const void* pSrc, u8 sizeInBytes)
{
	u8 i;
	u8 *pU8Dst = pDst;
	const u8 *pU8Src = pSrc;
	for (i = 0; i < sizeInBytes; i++, pU8Dst++, pU8Src++)
	{
		*pU8Dst = *pU8Src;
	}
}



/*
UgxBuffer UgxCreateBuffer(u8 *pBuffer, u8 sizeInBytes)
{
	u8 *pNewBuffer = UgxAllocateFromModelHeap(sizeInBytes);
	UgxCopyToIntenalRAM8Internal(pNewBuffer, pBuffer, sizeInBytes);

	return (UgxBuffer)pNewBuffer;
}
*/
UgxBuffer UgxCreateIndexBuffer(const u8 *pBuffer, u8 nbIndices)
{
	u8 *pNewBuffer = UgxAllocateFromModelHeap(nbIndices);
	UgxCopyToIntenalRAM8Internal(pNewBuffer, pBuffer, nbIndices);

	return (UgxBuffer)pNewBuffer;
}

UgxBuffer UgxCreateVertexBuffer(const SUgxModelVertex *pVertices, u8 nbVertices, enum ERotationOrder eRotationOrder, u8 u8RotX, u8 u8RotY, u8 u8RotZ, u16 u16Scale)
{
	u8 i;
	SUgxModelVertex *pNewVertices = UgxAllocateFromModelHeap(nbVertices*sizeof(SUgxModelVertex));

	for (i = 0; i < nbVertices; i++)
	{
		SUgxModelVertex vertex;
		s8x3 v3Rot = { u8RotX, u8RotY, u8RotZ };
		SUgxModelVertex vertexScale;

		vertexScale.x = Math_MulU8xU16_U32(pVertices[i].x, u16Scale) >> 8;
		vertexScale.y = Math_MulU8xU16_U32(pVertices[i].y, u16Scale) >> 8;
		vertexScale.z = Math_MulU8xU16_U32(pVertices[i].z, u16Scale) >> 8;

		S8x3_Rotate(&vertex, &vertexScale, eRotationOrder, &v3Rot);
		UgxCopyToIntenalRAM8Internal(pNewVertices + i, &vertex, sizeof(SUgxModelVertex));
	}

	return (UgxBuffer)pNewVertices;
}

/*
UgxModelHandle UgxCreateModel(const SUgxModel *pModel, enum ERotationOrder eRotationOrder, u8 u8RotX, u8 u8RotY, u8 u8RotZ)
{
	u8 i;
	SUgxModel *pNewModel = UgxAllocateFromModelHeap(sizeof(SUgxModel));

	SUgxModelVertex *pNewVertices = UgxAllocateFromModelHeap(pModel->nbVertices*sizeof(SUgxModelVertex));
	u8 *pNewIndices = UgxAllocateFromModelHeap(pModel->nbLines * 2);

	UgxCopyToIntenalRAM8Internal(pNewModel, pModel, sizeof(SUgxModel));

	UgxCopyToIntenalRAM8Internal(&pNewModel->pVertices, &pNewVertices, sizeof(SUgxModelVertex*));
	UgxCopyToIntenalRAM8Internal(&pNewModel->pIndices, &pNewIndices, sizeof(SUgxModelVertex*));

	for (i = 0; i < pModel->nbVertices; i++)
	{
		SUgxModelVertex vertex;
		s8x3 v3Rot = { u8RotX, u8RotY, u8RotZ };

		S8x3_Rotate(&vertex, &pModel->pVertices[i], eRotationOrder, &v3Rot);
		UgxCopyToIntenalRAM8Internal(pNewVertices+i, &vertex, sizeof(SUgxModelVertex));
	}

	UgxCopyToIntenalRAM8Internal(pNewIndices, pModel->pIndices, pModel->nbLines*2);

	return (UgxModelHandle) pNewModel;
}

UgxModelHandle UgxCreateModelSharedIndex(UgxModelHandle modelHandle, enum ERotationOrder eRotationOrder, u8 u8RotX, u8 u8RotY, u8 u8RotZ)
{
	u8 i;
	SUgxModel *pModel;
	SUgxModel *pNewModel;
	SUgxModelVertex *pVertices;

	RAM_ENABLE();

	pModel = (SUgxModel*)modelHandle;
	pNewModel = UgxAllocateFromModelHeap(sizeof(SUgxModel));

	pVertices = UgxAllocateFromModelHeap(pModel->nbVertices*sizeof(SUgxModelVertex));

	UgxCopy8Internal(pNewModel, pModel, sizeof(SUgxModel));
	pNewModel->pVertices = pVertices;

	for (i = 0; i < pModel->nbVertices; i++)
	{
		s8x3 v3Rot = { u8RotX, u8RotY, u8RotZ };
		S8x3_Rotate(&pNewModel->pVertices[i], &pModel->pVertices[i], eRotationOrder, &v3Rot);
	}
	RAM_DISABLE();

	return (UgxModelHandle)pNewModel;
}
*/
void UgxClearScreen()
{
	VDPWait();
	VDPFill(UGX_SCREEN_LEFT, UGX_SCREEN_TOP, UGX_SCREEN_RIGHT - UGX_SCREEN_LEFT + 1, UGX_SCREEN_BOTTOM - UGX_SCREEN_TOP + 1);
}

void UgxBeginFrame()
{
	s_ugxContext.m_u16InternalVertexHeap = INTERNAL_VERTEX_BUFFER_ADDRESS;
	s_ugxContext.m_pCommand = (SUgxCommand*) INTERNAL_COMMAND_BUFFER_ADDRESS;
}


#if 1
u8 UgxGetValueU16Internal(u16 u16X, u16 u16Z);
#else
u8 UgxGetValueU16Internal(u16 u16X, u16 u16Z)
{
	register u16 x = u16X;
	register u16 z = u16Z;
	if (x < 256 && z < 256)
	{
		return UgxGetValueU8(x,z);
	}

	x >>= 1;
	z >>= 1;
	
	if (x < 256 && z < 256)
	{
		return UgxGetValueU8(x,z);
	}

	x >>= 1;
	z >>= 1;

	if (x < 256 && z < 256)
	{
		return UgxGetValueU8(x, z);
	}

	return 0xff;
}
#endif

s16 s_s16ViewX;
s16 s_s16ViewY;
s16 s_s16ViewZ;

u8 s_u8ScreenX;
u8 s_u8ScreenY;

#if 1
u8 UgxPersXInternal();
#else
u8 UgxPersXInternal()//s16 x, s16 z, u8 *pOutX)
{
	register s16 x = s_s16ViewX;
	register s16 z = s_s16ViewZ;
	if (x < 0)
	{
		u8 sx = UgxGetValueU16Internal(-x, z);

		if (UGX_LEFT < sx)
		{
			return FALSE;
		}
		s_u8ScreenX = UGX_CENTER_X - sx;
	}
	else if (0 < x)
	{
		u8 sx = UgxGetValueU16Internal(x, z);
		if (UGX_RIGHT < sx)
		{
			return FALSE;
		}
		s_u8ScreenX = UGX_CENTER_X + sx;
	}
	else
	{
		s_u8ScreenX = UGX_CENTER_X;
	}
	return TRUE;
}
#endif
#if 1
u8 UgxPersYInternal();
#else
u8 UgxPersYInternal()//s16 y, s16 z, u8 *pOutY)
{
	register s16 y = s_s16ViewY;
	register s16 z = s_s16ViewZ;
	if (y < 0)
	{
		u8 sy = UgxGetValueU16Internal(-y, z);

		if (UGX_TOP < sy)
		{
			return FALSE;
		}
		s_u8ScreenY	= UGX_CENTER_Y - sy;
	}
	else if (0 < y)
	{
		u8 sy = UgxGetValueU16Internal(y, z);
		if (UGX_BOTTOM < sy)
		{
			return FALSE;
		}
		s_u8ScreenY = UGX_CENTER_Y + sy;
	}
	else
	{
		s_u8ScreenY = UGX_CENTER_Y;
	}
	return TRUE;
}
#endif

s16x3 g_v3ModelPosition;

u8 g_nbVertices;
s8 *g_pInVertices;
SUgxInternalVertex *g_pOutVertices;
u8 g_u8Clip;

#if 1
void UgxPersModelInternal();
#else
void UgxPersModelInternal()
{
	register u8 i;
	//register SUgxInternalVertex *pOutVertices = UgxAllocateFromVertexHeap(nbVertices*sizeof(SUgxInternalVertex));
	g_u8Clip = 0;
	for (i = 0; i < g_nbVertices; i++, g_pOutVertices++)
	{
		s_s16ViewX = g_v3ModelPosition.x + (s16)*g_pInVertices;
		g_pInVertices++;

		s_s16ViewY = g_v3ModelPosition.y + (s16)*g_pInVertices;
		g_pInVertices++;

		s_s16ViewZ = g_v3ModelPosition.z + (s16)*g_pInVertices;
		g_pInVertices++;

		if (s_s16ViewZ < 16)
		{
			g_pOutVertices->sy = 0xff;
			g_u8Clip++;
			continue;
		}

		if (UgxPersXInternal() == FALSE)
		{
			g_pOutVertices->sy = 0xff;
			g_u8Clip++;
			continue;
		}
		g_pOutVertices->sx = s_u8ScreenX;

		if (UgxPersYInternal() == FALSE)
		{
			g_pOutVertices->sy = 0xff;
			g_u8Clip++;
			continue;
		}
		g_pOutVertices->sy = s_u8ScreenY;
	}

	return clip;
}
#endif

#if 1
void UgxDrawModel(UgxBuffer vertices, u8 nbVertices, UgxBuffer indices, u8 nbLines)
{
	//register u8 i;
	//register u8 clip;
	register SUgxCommandRenderObject *pCommand = (SUgxCommandRenderObject*)s_ugxContext.m_pCommand;
	//register SUgxInternalVertex *pOutVertices;
	//register s8 *pInVertices;
	s_ugxContext.m_pCommand++;

	RAM_ENABLE();
	g_pOutVertices = UgxAllocateFromVertexHeap(nbVertices*sizeof(SUgxInternalVertex));

	pCommand->eType = kUgxCommand_RenderObjectNoClip;
	pCommand->u8Color = s_ugxContext.m_commandPrimitive.color;
	pCommand->pIndex = (void*) indices;
	pCommand->nbLines = nbLines;
	pCommand->pVertex = (void*)g_pOutVertices;

	g_nbVertices = nbVertices;
	g_pInVertices = (s8*)vertices;

#if 1
	UgxPersModelInternal();
#else
	clip = 0;
	for (i = 0; i < nbVertices; i++, pOutVertices++)
	{
		s_s16ViewX = x + (s16)*pInVertices;
		pInVertices++;

		s_s16ViewY = y + (s16)*pInVertices;
		pInVertices++;

		s_s16ViewZ = z + (s16)*pInVertices;
		pInVertices++;

		if (s_s16ViewZ < 16)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}

		if (UgxPersXInternal() == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}
		pOutVertices->sx = s_u8ScreenX;

		if (UgxPersYInternal() == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}
		pOutVertices->sy = s_u8ScreenY;
	}
#endif
	if (g_u8Clip == 0)
	{
		pCommand->eType = kUgxCommand_RenderObjectNoClip;
	}
	else if (g_u8Clip < nbVertices)
	{
		pCommand->eType = kUgxCommand_RenderObjectClip;
	}
	else
	{
		s_ugxContext.m_pCommand--;
	}
	RAM_DISABLE();
}
#elif 0
void UgxAppendModel(s16 x, s16 y, s16 z, UgxModelHandle model)
{
	u8 i;
	u8 clip;
	SUgxModel *pModel = (SUgxModel*)model;
	SUgxCommandRenderObject *pCommand = (SUgxCommandRenderObject*) s_ugxContext.m_pCommand;
	SUgxInternalVertex *pOutVertices;
	s8 *pInVertices;
	s_ugxContext.m_pCommand++;

	RAM_ENABLE();
	pOutVertices = UgxAllocateFromVertexHeap(pModel->nbVertices*sizeof(SUgxInternalVertex));

	pCommand->eType = kUgxCommand_RenderObjectNoClip;
	pCommand->u8Color = s_ugxContext.m_commandPrimitive.color;
	pCommand->pObject = pModel;
	pCommand->pVertex = pOutVertices;

	pInVertices = (s8*)pModel->pVertices;

	clip = 0;
	for (i = 0; i < pModel->nbVertices; i++, pOutVertices++)
	{
		static s16 xx, yy, zz;
		xx = x+(s16)*pInVertices;
		pInVertices++;

		yy = y+(s16)*pInVertices;
		pInVertices++;

		zz = z+(s16)*pInVertices;
		pInVertices++;

		if (zz < 16)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}

		if (UgxPersXInternal(xx, zz, &pOutVertices->sx) == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}

		if (UgxPersYInternal(yy, zz, &pOutVertices->sy) == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}
	}

	if (clip == 0)
	{
		pCommand->eType = kUgxCommand_RenderObjectNoClip;
	}
	else if (clip < pModel->nbVertices)
	{
		pCommand->eType = kUgxCommand_RenderObjectClip;
	}
	else
	{
		s_ugxContext.m_pCommand--;
	}
	RAM_DISABLE();
}
#else
void UgxAppendModel(s16 x, s16 y, s16 z, u8 rot, UgxModelHandle model)
{
	u8 i;
	u8 clip;
	SUgxModel *pModel = (SUgxModel*)model;
	SUgxCommandRenderObject *pCommand = (SUgxCommandRenderObject*) s_ugxContext.m_pCommand;
	SUgxInternalVertex *pOutVertices;
	s8 *pInVertices;
	static s16 sin, cos;
	s_ugxContext.m_pCommand++;

	RAM_ENABLE();
	pOutVertices = UgxAllocateFromVertexHeap(pModel->nbVertices*sizeof(SUgxInternalVertex));

	pCommand->eType = kUgxCommand_RenderObjectNoClip;
	pCommand->u8Color = s_ugxContext.m_commandPrimitive.color;
	pCommand->pObject = pModel;
	pCommand->pVertex = pOutVertices;

	pInVertices = (s8*)pModel->pVertices;

	sin = SinCos_GetSin(rot);
	cos = SinCos_GetCos(rot);

	clip = 0;
	for (i = 0; i < pModel->nbVertices; i++, pOutVertices++)
	{
		static s16 xx, yy, zz;
		static s16 px, py, pz;
		static s16 rx, ry, rz;
		px = (s16)*pInVertices;
		pInVertices++;

		py = (s16)*pInVertices;
		pInVertices++;

		pz = (s16)*pInVertices;
		pInVertices++;

		rx = (s16)(Math_MulS16xS16_S32(px, cos) >> 8) - (s16)(Math_MulS16xS16_S32(pz, sin) >> 8);
		ry = py;
		rz = (s16)(Math_MulS16xS16_S32(px, sin) >> 8) + (s16)(Math_MulS16xS16_S32(pz, cos) >> 8);

		xx = x + rx;
		yy = y + ry;
		zz = z + rz;

		if (zz < 16)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}

		if (UgxPersXInternal(xx, zz, &pOutVertices->sx) == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}

		if (UgxPersYInternal(yy, zz, &pOutVertices->sy) == FALSE)
		{
			pOutVertices->sy = 0xff;
			clip++;
			continue;
		}
	}

	if (clip == 0)
	{
		pCommand->eType = kUgxCommand_RenderObjectNoClip;
	}
	else if (clip < pModel->nbVertices)
	{
		pCommand->eType = kUgxCommand_RenderObjectClip;
	}
	else
	{
		s_ugxContext.m_pCommand--;
	}
	RAM_DISABLE();
}
#endif

void UgxBegin(enum EUgxPrimitive ePrimitive)
{
	s_ugxContext.m_commandPrimitive.eType = kUgxCommand_RenderPrimitive;
	s_ugxContext.m_commandPrimitive.ePrimitive = ePrimitive;
	s_ugxContext.m_commandPrimitive.pVertices = (void*) s_ugxContext.m_u16InternalVertexHeap;
	s_ugxContext.m_commandPrimitive.nbVertices = 0;
}

void UgxColor(u8 color)
{
	s_ugxContext.m_commandPrimitive.color = color;
}

void UgxVertex2u8(u8 x, u8 y)
{
	u8* pVertex = UgxAllocateFromVertexHeap(2);
	SLOTWriteDI(s_ugxContext.u8Page1RamSlot, (u16)pVertex, x);
	SLOTWriteDI(s_ugxContext.u8Page1RamSlot, (u16)(pVertex+1), y);
	s_ugxContext.m_commandPrimitive.nbVertices++;
	INT_ENABLE();
}

void UgxEnd()
{
	SUgxCommandPrimitive *pCommand = (SUgxCommandPrimitive*) s_ugxContext.m_pCommand;
	s_ugxContext.m_pCommand++;

	RAM_ENABLE();
	UgxCopy8Internal(pCommand, &s_ugxContext.m_commandPrimitive, sizeof(SUgxCommandPrimitive));

	RAM_DISABLE();
}

void UgxEndFrame()
{
	SUgxCommand *pCommand = s_ugxContext.m_pCommand;
	s_ugxContext.m_pCommand++;

	RAM_ENABLE();
	pCommand->eType = kUgxCommand_End;

	RAM_DISABLE();
}


#if 1
void UgxOnCommandRenderObjectClip(SUgxCommandRenderObject *pCommand);
#else
void UgxOnCommandRenderObjectClip(SUgxCommandRenderObject *pCommand)
{
	u8 i;
	//SUgxModel *pModel = pCommand->pObject;
	SUgxInternalVertex *pVertices = pCommand->pVertex;
	u8 *pIndex = pCommand->pIndex;

	VDPSetForegroundColor(pCommand->u8Color);
	for (i = 0; i < pCommand->nbLines; i++)
	{
		u8 i0, i1;
		SUgxInternalVertex *pV0, *pV1;

		i0 = *pIndex;
		pIndex++;
		i1 = *pIndex;
		pIndex++;

		pV0 = &pVertices[i0];
		if (pV0->sy == 0xff)
		{
			continue;
		}

		pV1 = &pVertices[i1];
		if ( pV1->sy == 0xff)
		{
			continue;
		}

		VDPWaitLine(pV0->sx, pV0->sy, pV1->sx, pV1->sy);
	}
}
#endif

#if 1
extern void UgxOnCommandRenderObjectNoClip(SUgxCommandRenderObject *pCommand);
#else
void UgxOnCommandRenderObjectNoClip(SUgxCommandRenderObject *pCommand)
{
	register u8 i;
	register SUgxInternalVertex *pVertices = pCommand->pVertex;
	register u8 *pIndex = pCommand->pIndex;

	VDPSetForegroundColor(pCommand->u8Color);
	for (i = 0; i < pCommand->nbLines; i++)
	{
		register u8 i0, i1;
		register SUgxInternalVertex *pV0, *pV1;

		i0 = *pIndex;
		pIndex++;
		i1 = *pIndex;
		pIndex++;

		pV0 = &pVertices[i0];
		pV1 = &pVertices[i1];

		VDPWaitLine(pV0->sx, pV0->sy, pV1->sx, pV1->sy);
	}
}
#endif
void UgxOnCommandRenderPrimitivePoints(SUgxCommandPrimitive *pCommand)
{
	register u8 i;
	register u8 *pVertex = pCommand->pVertices;
	register u8 nbPrimitives = pCommand->nbVertices;
	VDPSetForegroundColor(pCommand->color);

	for (i = 0; i < nbPrimitives; i++)
	{
		static u8 x, y;
		x = *pVertex;
		pVertex++;
		y = *pVertex;
		pVertex++;

		VDPWait();
		//VDPPoint(x, y);
	}
}

void UgxOnCommandRenderPrimitiveLines(SUgxCommandPrimitive *pCommand)
{
	register u8 i;
	register u8 *pVertex = pCommand->pVertices;
	register u8 nbPrimitives = pCommand->nbVertices >> 1;
	VDPSetForegroundColor(pCommand->color);

	for (i = 0; i < nbPrimitives; i++)
	{
		static u8 x0, y0, x1, y1;
		x0 = *pVertex;
		pVertex++;
		y0 = *pVertex;
		pVertex++;
		x1 = *pVertex;
		pVertex++;
		y1 = *pVertex;
		pVertex++;
		VDPWaitLine(x0, y0, x1, y1);
	}
}

void UgxOnCommandRenderPrimitiveRectangles(SUgxCommandPrimitive *pCommand)
{
	register u8 i;
	register u8 *pVertex = pCommand->pVertices;
	register u8 nbPrimitives = pCommand->nbVertices >> 1;
	VDPSetForegroundColor(pCommand->color);

	for (i = 0; i < nbPrimitives; i++)
	{
		static u8 x, y;
		static u16 w, h;
		x = *pVertex;
		pVertex++;
		y = *pVertex;
		pVertex++;
		w = *pVertex;
		pVertex++;
		h = *pVertex;
		pVertex++;
		VDPWait();
		VDPFill(x, y, w, h);
	}
}

#if 0
void UgxOnCommandRenderPrimitiveLineStrip(SUgxCommandPrimitive *pCommand)
{

}
#else
void UgxOnCommandRenderPrimitiveLineStrip(SUgxCommandPrimitive *pCommand)
{
	register u8 i;
	register u8 *pVertex = pCommand->pVertices;
	register u8 nbLines = pCommand->nbVertices;
	static u8 x0, y0;
	VDPSetForegroundColor(pCommand->color);

	x0 = *pVertex;
	pVertex++;
	y0 = *pVertex;
	pVertex++;

	nbLines -= 1;
	
	for (i = 0; i < nbLines; i++)
	{
		static u8 x1, y1;
		x1 = *pVertex;
		pVertex++;
		y1 = *pVertex;
		pVertex++;

		VDPWaitLine(x0, y0, x1, y1);
		
		x0 = x1;
		y0 = y1;
	}
}
#endif
void UgxOnCommandRenderPrimitive(SUgxCommandPrimitive *pCommand)
{
	/*
	const void(*pFunctions[])(SUgxCommandPrimitive*) =
	{
		UgxOnCommandRenderPrimitivePoints,
		UgxOnCommandRenderPrimitiveLines,
		UgxOnCommandRenderPrimitiveRectangles,
		UgxOnCommandRenderPrimitiveLineStrip,
	};

	pFunctions[pCommand->ePrimitive](pCommand);
	*/

	switch (pCommand->ePrimitive)
	{
	case kUgxPrimitive_Points:
		UgxOnCommandRenderPrimitivePoints(pCommand);
		break;

	case kUgxPrimitive_Lines:
		UgxOnCommandRenderPrimitiveLines(pCommand);
		break;

	case kUgxPrimitive_Rectangles:
		UgxOnCommandRenderPrimitiveRectangles(pCommand);
		break;

	case kUgxPrimitive_LineStrip:
		UgxOnCommandRenderPrimitiveLineStrip(pCommand);
		break;
	}
}

void UgxFlushCommands()
{
	SUgxCommand *pCommand = (SUgxCommand*) INTERNAL_COMMAND_BUFFER_ADDRESS;

	RAM_ENABLE();

	for (;pCommand != NULL;pCommand++)
	{
		switch (pCommand->eType)
		{
		case kUgxCommand_RenderObjectClip:
			UgxOnCommandRenderObjectClip( (SUgxCommandRenderObject*) pCommand);
			break;

		case kUgxCommand_RenderObjectNoClip:
			UgxOnCommandRenderObjectNoClip((SUgxCommandRenderObject*)pCommand);
			break;

		case kUgxCommand_RenderPrimitive:
			UgxOnCommandRenderPrimitive((SUgxCommandPrimitive*)pCommand);
			break;

		case kUgxCommand_End:
		default:
			pCommand = NULL;
			break;
			//RAM_DISABLE();
			//return;
		}
	}

	RAM_DISABLE();
	return;
}

