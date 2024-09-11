const static s8x3 s_vertexExp0[] =
{
	{ 0,-15,0 },
	{ -10,-10,0 },
	{ -15,0,0 },
	{ -10,10,0 },
	{ 0,15,0 },
	{ 10,10,0 },
	{ 15,0,0 },
	{ 10,-10,0 },
};
const static u16 s_indexExp0[] =
{
	LINE(1,0),
	LINE(1,2),
	LINE(2,3),
	LINE(3,4),
	LINE(4,5),
	LINE(5,6),
	LINE(6,7),
	LINE(7,0),
};
const SMesh g_meshExp0 =
{
	8,8,
	s_vertexExp0,
	s_indexExp0
};
