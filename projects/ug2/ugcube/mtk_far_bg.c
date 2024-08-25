#include "macros.h"
#include "mtk_far_bg.h"
#include "mtk_player.h"
#include "pers.h"

SMtkFarBg g_mtkFarBg[MTK_FAR_BG_COUNT];

void MtkFarBgInit(void)
{
	u8 i;
	SMtkFarBg *farBg = g_mtkFarBg;
	for(i = 0; i < MTK_FAR_BG_COUNT; i++, farBg++)
	{
		farBg->m_visibility = 0;
		farBg->m_mesh = NULL;
		s8x3Set(farBg->m_position, 0, 0, 0);
		farBg->m_clampPlus = 0;
		farBg->m_clampMinus = 0;
	}
}

void MtkFarBgSetModel(u8 index, const SMtkMesh *pMesh)
{
	SMtkFarBg *farBg = &g_mtkFarBg[index];
	ASSERT(index < MTK_FAR_BG_COUNT);

	farBg->m_visibility = pMesh ? 1 : 0;
	farBg->m_mesh = pMesh;
}

void MtkFarBgSetPosition(u8 index, s8 x, s8 y, s8 z, s8 clamp)
{
	SMtkFarBg *farBg = &g_mtkFarBg[index];
	ASSERT(index < MTK_FAR_BG_COUNT);
	ASSERT(0 < clamp);

	s8x3Set(farBg->m_position, x, y, z);
	farBg->m_clampPlus = clamp;
	farBg->m_clampMinus = -clamp;
}

void MtkFarBgUpdate(void)
{
	static u8 i;
	static SMtkFarBg *pFarBg;
	static s8 x, y, z;

	pFarBg = g_mtkFarBg;
	for(i = 0; i < MTK_FAR_BG_COUNT; i++, pFarBg++)
	{
		if (!pFarBg->m_visibility)
		{
			continue;
		}

		x = pFarBg->m_position.x;
		y = pFarBg->m_position.y;
		z = pFarBg->m_position.z;

		if (g_mtkPlayer.m_angularVelocity.x)
		{
			s8XY xy = SinCos_RotateXYS8Lp(y, z, -g_mtkPlayer.m_angularVelocity.x);
			y = s8XY_GetX(xy);
			z = s8XY_GetY(xy);
		}

		if (g_mtkPlayer.m_angularVelocity.y)
		{
			s8XY xy = SinCos_RotateXYS8Lp(z, x, -g_mtkPlayer.m_angularVelocity.y);
			z = s8XY_GetX(xy);
			x = s8XY_GetY(xy);
		}

#if 0
		if (g_mtkPlayer.m_angularVelocity.z)
		{
			s8XY xy = SinCos_RotateXYS8Lp(x, y, -g_mtkPlayer.m_angularVelocity.z);
			x = s8XY_GetX(xy);
			y = s8XY_GetY(xy);
		}
#endif

		if (x < pFarBg->m_clampMinus)
		{
			x = pFarBg->m_clampMinus;
		}
		else if (pFarBg->m_clampPlus < x)
		{
			x = pFarBg->m_clampPlus;
		}

		if (y < pFarBg->m_clampMinus)
		{
			y = pFarBg->m_clampMinus;
		}
		else if (pFarBg->m_clampPlus < y)
		{
			y = pFarBg->m_clampPlus;
		}

		if (z < pFarBg->m_clampMinus)
		{
			z = pFarBg->m_clampMinus;
		}
		else if (pFarBg->m_clampPlus < z)
		{
			z = pFarBg->m_clampPlus;
		}

		s8x3Set(pFarBg->m_position, x, y, z);
	}
}

void MtkFarBgRender(void)
{
	static u8 i;
	static SMtkFarBg *pFarBg;

	pFarBg = g_mtkFarBg;
	for (i = 0; i < MTK_FAR_BG_COUNT; i++, pFarBg++)
	{
		if (!pFarBg->m_visibility)
		{
			continue;
		}

		PersSetPosition(pFarBg->m_position.x, pFarBg->m_position.y, pFarBg->m_position.z);
		if (!PersClipBBoxVram(pFarBg->m_mesh->m_bboxVram, g_persContext.m_s8ClipNear))
		{
			return;
		}

		ASSERT(pFarBg->m_mesh)
		MtkMeshDrawNoClip(pFarBg->m_mesh);
	}
}
