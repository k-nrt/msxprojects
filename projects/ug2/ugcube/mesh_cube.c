
#include "mesh_cube.h"

const static s8x3 s_vertexCube[] =
{
	{ -32, -32, 32 },
	{ -32, +32, 32 },
	{ +32, +32, 32 },
	{ +32, -32, 32 },
	{ -32, -32, -32 },
	{ -32, +32, -32 },
	{ +32, +32, -32 },
	{ +32, -32, -32 },
};

const static u16 s_indexCube[] =
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

const SMesh g_meshCube =
{
	8, 12,
    s_vertexCube,
	s_indexCube,
};


