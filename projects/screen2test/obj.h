#ifndef OBJ_H
#define OBJ_H
#include <msx-bios-wrapper.h>

enum EObjType
{
	kObjType_4x4,
	kObjType_32x1,
};

struct SObj
{
	enum EObjType type;
	u8 pat[1];
};


struct SObj4x4
{
	enum EObjType type;
	u8 p1x1[1*1];
	u8 p2x2[2*2];
	u8 p3x3[3*3];
	u8 p4x4[4*4];
};

struct SObj32x1
{
	enum EObjType type;
	u8 p8x1[8*1];
	u8 p10x1[10*1];
	u8 p16x1[16*1];
	u8 p18x1[18*1];
	u8 p24x1[24*4];
	u8 p32x1[32*4];
};

extern const struct SObj4x4 g_objBuilding0;
extern const struct SObj4x4 g_objBuilding1;
extern const struct SObj4x4 g_objBuildingL0;

extern const struct SObj4x4 g_objFenceRedLeft;
extern const struct SObj4x4 g_objFenceWhiteLeft;

typedef char Obj3d;

extern const Obj3d g_obj3dFenceRed[];
extern const Obj3d g_obj3dFenceWhite[];
extern const Obj3d g_obj3dFenceBlue[];

extern const Obj3d g_obj3dBuildingLeft[];
extern const Obj3d g_obj3dBuildingRight[];
extern const Obj3d g_obj3dBuilding1Left[];
extern const Obj3d g_obj3dBuilding2Left[];
extern const Obj3d g_obj3dBuilding1Right[];

extern const Obj3d g_obj3dRoad0[];
extern const Obj3d g_obj3dRoad1[];

extern const Obj3d g_obj3dTreeDark[];
extern const Obj3d g_obj3dTreeLight[];


void Obj3dBlit( s8 x, s8 y, int size, const Obj3d *pObj3d );

void ObjBlit( s8 x, s8 y, int size, const struct SObj *pObj );

#endif //OBJ_H