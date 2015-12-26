const SUgxModelVertex g_vertexEnemy0[] =
{
	{ 0,-10,0 },
	{ 10,0,0 },
	{ 0,0,-10 },
	{ -10,0,0 },
	{ 0,10,0 },
	{ 0,-20,0 },
	{ 20,0,0 },
	{ -20,0,0 },
	{ 0,20,0 },
};
const u8 g_indexEnemy0[] =
{
	LINE(0,2),
	LINE(2,1),
	LINE(1,0),
	LINE(0,3),
	LINE(3,2),
	LINE(2,4),
	LINE(4,1),
	LINE(3,4),
	LINE(7,3),
	LINE(5,0),
	LINE(1,6),
	LINE(4,8),
};
const SUgxModel g_modelEnemy0 =
{
	10, 10, 10, 10, 10, 10,
	9,12,
	g_vertexEnemy0,
	g_indexEnemy0
};
