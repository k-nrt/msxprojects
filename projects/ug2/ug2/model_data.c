#include "model_data.h"

#pragma codeseg CODE2


#define LINE(i0,i1) (i0),(i1)

#include "model_player.h"
#include "model_beam.h"
#include "model_exp0.h"
#include "model_exp1.h"
#include "model_exp2.h"
#include "model_exp3.h"
#include "model_enemy0.h"
#include "model_enemy1.h"
#include "model_enemy2.h"

const SUgxModelVertex g_vertexCube[] =
{
	{ -10, -10, 10 },
	{ -10, +10, 10 },
	{ +10, +10, 10 },
	{ +10, -10, 10 },
	{ -10, -10, -10 },
	{ -10, +10, -10 },
	{ +10, +10, -10 },
	{ +10, -10, -10 },
};

const u8 g_indexCube[] =
{
	LINE(0, 1),
	LINE(1, 2),
	LINE(2, 3),
	LINE(3, 0),
	LINE(4, 5),
	LINE(5, 6),
	LINE(6, 7),
	LINE(7, 4),
	LINE(0, 4),
	LINE(1, 5),
	LINE(2, 6),
	LINE(3, 7),
};

const SUgxModel g_modelCube =
{
	10, 10, 10, 10, 10, 10,
	8, 12,
	g_vertexCube,
	g_indexCube,
};

const SUgxModelVertex g_vertexGridXY[] =
{
	{ -96, -96, 0 },
	{ -32, -96, 0 },
	{ 32, -96, 0 },
	{ 96, -96, 0 },

	{ -96, 96, 0 },
	{ -32, 96, 0 },
	{ 32, 96, 0 },
	{ 96, 96, 0 },

	{ -96, -96, 0 },
	{ -96, -32, 0 },
	{ -96, 32, 0 },
	{ -96, 96, 0 },

	{ 96, -96, 0 },
	{ 96, -32, 0 },
	{ 96, 32, 0 },
	{ 96, 96, 0 },

	{ -32, -32, -64 },
	{ 32, -32, -64 },
	{ 32, 32, -64 },
	{ -32, 32, -64 },
};

const u8 g_indexGridXY[] =
{
	LINE(0, 4),
	LINE(1, 5),
	LINE(2, 6),
	LINE(3, 7),
	LINE(8, 12),
	LINE(9, 13),
	LINE(10, 14),
	LINE(11, 15),
	LINE(16, 17),
	LINE(17, 18),
	LINE(18, 19),
	LINE(19, 16),
};

const SUgxModel g_modelGridXY =
{
	10, 10, 10, 10, 10, 10,
	20, 12,
	g_vertexGridXY,
	g_indexGridXY
};


