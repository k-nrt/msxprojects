const static s8x3 s_vertexExp2[] =
{
	{ -4,-24,0 },
	{ 10,-17,0 },
	{ 17,-1,0 },
	{ 10,13,0 },
	{ -4,20,0 },
	{ -20,13,0 },
	{ -27,-1,0 },
	{ -20,-17,0 },
	{ 0,-29,0 },
	{ 20,-20,0 },
	{ 29,0,0 },
	{ 20,20,0 },
	{ 0,29,0 },
	{ -20,20,0 },
	{ -29,0,0 },
	{ -20,-20,0 },
};
const static u16 s_indexExp2[] =
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
const SMesh g_meshExp2 =
{
	16,16,
	s_vertexExp2,
	s_indexExp2
};
