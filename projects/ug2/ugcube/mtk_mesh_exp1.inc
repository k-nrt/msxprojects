const static s8x3 s_vertexExp1[] =
{
	{ 11,-15,0 },
	{ 4,-12,0 },
	{ 1,-5,0 },
	{ 4,1,0 },
	{ 11,4,0 },
	{ 18,1,0 },
	{ 21,-5,0 },
	{ 18,-12,0 },
	{ 0,-23,0 },
	{ -16,-16,0 },
	{ -23,0,0 },
	{ -16,16,0 },
	{ 0,23,0 },
	{ 16,16,0 },
	{ 23,0,0 },
	{ 16,-16,0 },
};
const static u16 s_indexExp1[] =
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
const SMesh g_meshExp1 =
{
	16,16,
	s_vertexExp1,
	s_indexExp1
};
