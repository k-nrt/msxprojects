const static s8x3 s_vertexPlanet[] =
{
	{ 9,-30,0 },
	{ 0,-31,0 },
	{ -9,-30,0 },
	{ -18,-25,0 },
	{ -25,-18,0 },
	{ -30,-9,0 },
	{ -32,0,0 },
	{ -30,9,0 },
	{ -25,18,0 },
	{ -18,25,0 },
	{ -9,30,0 },
	{ 0,32,0 },
	{ 9,30,0 },
	{ 18,25,0 },
	{ 25,18,0 },
	{ 30,9,0 },
	{ 32,0,0 },
	{ 30,-9,0 },
	{ 25,-18,0 },
	{ 18,-25,0 },
	{ 9,-30,0 },
	{ 0,-31,0 },
};
const static u16 s_indexPlanet[] =
{
	LINE(0,1),
	LINE(1,2),
	LINE(2,3),
	LINE(3,4),
	LINE(4,5),
	LINE(5,6),
	LINE(6,7),
	LINE(7,8),
	LINE(8,9),
	LINE(9,10),
	LINE(10,11),
	LINE(11,12),
	LINE(12,13),
	LINE(13,14),
	LINE(14,15),
	LINE(15,16),
	LINE(16,17),
	LINE(17,18),
	LINE(18,19),
	LINE(19,20),
	LINE(20,21),
};
const SMesh g_meshPlanet =
{
	22,21,
	s_vertexPlanet,
	s_indexPlanet
};
