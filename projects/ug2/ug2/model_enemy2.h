const SUgxModelVertex g_vertexEnemy2[] =
{
	{ 0,-13,12 },
	{ 11,-7,6 },
	{ 11,-7,-6 },
	{ 0,-13,-12 },
	{ -11,-7,-6 },
	{ -11,-7,6 },
	{ 0,13,12 },
	{ 11,7,6 },
	{ 11,7,-6 },
	{ 0,13,-12 },
	{ -11,7,-6 },
	{ -11,7,6 },
};
const u8 g_indexEnemy2[] =
{
	LINE(0,1),
	LINE(1,7),
	LINE(7,6),
	LINE(6,0),
	LINE(1,2),
	LINE(2,8),
	LINE(8,7),
	LINE(2,3),
	LINE(3,9),
	LINE(9,8),
	LINE(3,4),
	LINE(4,10),
	LINE(10,9),
	LINE(4,5),
	LINE(5,11),
	LINE(11,10),
	LINE(5,0),
	LINE(6,11),
	LINE(9,6),
	LINE(3,0),
};
const SUgxModel g_modelEnemy2 =
{
	10, 10, 10, 10, 10, 10,
	12,20,
	g_vertexEnemy2,
	g_indexEnemy2
};
