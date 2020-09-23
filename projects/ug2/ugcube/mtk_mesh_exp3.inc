const static s8x3 s_vertexExp3[] =
{
	{ -2,-28,0 },
	{ 16,-20,0 },
	{ 25,-1,0 },
	{ 16,18,0 },
	{ -2,26,0 },
	{ -22,18,0 },
	{ 0,-31,0 },
	{ 22,-22,0 },
	{ 31,0,0 },
	{ 22,22,0 },
	{ 0,31,0 },
	{ -22,22,0 },
	{ -31,0,0 },
	{ -22,-22,0 },
};
const static u16 s_indexExp3[] =
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
const SMesh g_meshExp3 =
{
	14,14,
	s_vertexExp3,
	s_indexExp3
};
