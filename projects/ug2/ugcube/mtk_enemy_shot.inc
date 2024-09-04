const static s8x3 s_vertexEnemyShot[] =
{
	{ 0,-16,8 },
	{ -32,0,-8 },
	{ 32,0,-8 },
	{ 0,16,8 },
};
const static u16 s_indexEnemyShot[] =
{
	LINE(0,1),
	LINE(2,0),
	LINE(1,3),
	LINE(3,2),
};
const SMesh g_meshEnemyShot =
{
	4,4,
	s_vertexEnemyShot,
	s_indexEnemyShot
};
