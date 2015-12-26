const SUgxModelVertex g_vertexExp2[] =
{
	{ -3,-15,0 },
	{ 7,-11,0 },
	{ 11,-1,0 },
	{ 7,9,0 },
	{ -3,13,0 },
	{ -13,9,0 },
	{ -17,-1,0 },
	{ -13,-11,0 },
	{ 0,-19,0 },
	{ 13,-13,0 },
	{ 19,0,0 },
	{ 13,13,0 },
	{ 0,19,0 },
	{ -13,13,0 },
	{ -19,0,0 },
	{ -13,-13,0 },
};
const u8 g_indexExp2[] =
{
	LINE(1,0),
	LINE(1,2),
	LINE(2,3),
	LINE(3,4),
	LINE(4,5),
	LINE(5,6),
	LINE(6,7),
	LINE(7,0),
	LINE(9,8),
	LINE(9,10),
	LINE(10,11),
	LINE(11,12),
	LINE(12,13),
	LINE(13,14),
	LINE(14,15),
	LINE(15,8),
};
const SUgxModel g_modelExp2 =
{
	10, 10, 10, 10, 10, 10,
	16,16,
	g_vertexExp2,
	g_indexExp2
};
