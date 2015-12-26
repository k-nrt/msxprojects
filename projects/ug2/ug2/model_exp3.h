const SUgxModelVertex g_vertexExp3[] =
{
	{ -1,-19,0 },
	{ 11,-13,0 },
	{ 16,0,0 },
	{ 11,12,0 },
	{ -1,17,0 },
	{ -14,12,0 },
	{ 0,-21,0 },
	{ 14,-14,0 },
	{ 21,0,0 },
	{ 14,14,0 },
	{ 0,21,0 },
	{ -14,14,0 },
	{ -21,0,0 },
	{ -14,-14,0 },
};
const u8 g_indexExp3[] =
{
	LINE(1,0),
	LINE(1,2),
	LINE(2,3),
	LINE(3,4),
	LINE(4,5),
	LINE(7,6),
	LINE(7,8),
	LINE(8,9),
	LINE(9,10),
	LINE(10,11),
	LINE(11,12),
	LINE(13,6),
	LINE(5,12),
	LINE(0,13),
};
const SUgxModel g_modelExp3 =
{
	10, 10, 10, 10, 10, 10,
	14,14,
	g_vertexExp3,
	g_indexExp3
};
