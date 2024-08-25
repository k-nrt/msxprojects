#ifndef MTK_FAR_BG_H
#define MTK_FAR_BG_H

#include "vec_math.h"
#include "mtk_mesh.h"

typedef struct tagMtkFarBg
{
	u8 m_visibility;
	const SMtkMesh* m_mesh;
	s8x3 m_position;
	s8 m_clampPlus;
	s8 m_clampMinus;
} SMtkFarBg;

#define MTK_FAR_BG_COUNT (2)
extern SMtkFarBg g_mtkFarBg[MTK_FAR_BG_COUNT];

extern void MtkFarBgInit(void);
extern void MtkFarBgSetModel(u8 index, const SMtkMesh *pMesh);
extern void MtkFarBgSetPosition(u8 index, s8 x, s8 y, s8 z, s8 clamp);
extern void MtkFarBgUpdate(void);
extern void MtkFarBgRender(void);

#endif //MTK_FAR_BG_H
