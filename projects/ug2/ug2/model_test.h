const SUgxModelVertex g_vertexTest[] =
{
	{ 0,5,43 },
	{ 8,5,-8 },
	{ -8,5,-8 },
	{ 8,1,15 },
	{ 28,14,-28 },
	{ -28,14,-28 },
	{ -8,1,15 },
	{ 0,-8,-14 },
	{ 8,1,-21 },
	{ -8,1,-21 },
};
const u8 g_indexTest[] =
{
	LINE(1,0),
	LINE(0,7),
	LINE(7,1),
	LINE(0,2),
	LINE(2,7),
	LINE(1,2),
	LINE(3,8),
	LINE(8,4),
	LINE(4,3),
	LINE(5,9),
	LINE(9,6),
	LINE(6,5),
};
const SUgxModel g_modelTest =
{
	10, 10, 10, 10, 10, 10,
	10,12,
	g_vertexTest,
	g_indexTest
};
