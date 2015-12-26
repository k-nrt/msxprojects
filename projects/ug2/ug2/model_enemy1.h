const SUgxModelVertex g_vertexEnemy1[] =
{
	{ 8,-6,4 },
	{ 8,6,4 },
	{ -8,-6,4 },
	{ -8,6,4 },
	{ 22,0,13 },
	{ -22,0,13 },
	{ 6,-4,8 },
	{ 6,3,8 },
	{ -6,-4,8 },
	{ -6,3,8 },
	{ 0,9,14 },
	{ 0,-8,14 },
	{ 0,-4,-16 },
	{ 0,4,-16 },
};
const u8 g_indexEnemy1[] =
{
	LINE(0,1),
	LINE(3,2),
	LINE(1,4),
	LINE(5,3),
	LINE(4,0),
	LINE(2,5),
	LINE(6,7),
	LINE(9,8),
	LINE(9,10),
	LINE(10,7),
	LINE(8,11),
	LINE(11,6),
	LINE(12,13),
	LINE(11,12),
	LINE(10,13),
	LINE(6,12),
	LINE(12,8),
	LINE(7,13),
	LINE(13,9),
};
const SUgxModel g_modelEnemy1 =
{
	10, 10, 10, 10, 10, 10,
	14,19,
	g_vertexEnemy1,
	g_indexEnemy1
};
