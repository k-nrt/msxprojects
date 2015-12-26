const SUgxModelVertex g_vertexExp1[] =
{
	{ -7,-10,0 },
	{ -3,-8,0 },
	{ -1,-3,0 },
	{ -3,1,0 },
	{ -7,3,0 },
	{ -12,1,0 },
	{ -14,-3,0 },
	{ -12,-8,0 },
	{ 0,-15,0 },
	{ 11,-11,0 },
	{ 15,0,0 },
	{ 11,11,0 },
	{ 0,15,0 },
	{ -11,11,0 },
	{ -15,0,0 },
	{ -11,-11,0 },
};
const u8 g_indexExp1[] =
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
const SUgxModel g_modelExp1 =
{
	10, 10, 10, 10, 10, 10,
	16,16,
	g_vertexExp1,
	g_indexExp1
};
