const static s8x3 s_vertexBeam[] =
{
	{ -2,0,32 },
	{ 2,0,32 },
	{ 2,0,-32 },
	{ -2,0,-32 },
};
const static u16 s_indexBeam[] =
{
	LINE(3,2),
	LINE(2,1),
	LINE(1,0),
	LINE(0,3),
};
const SMesh g_meshBeam =
{
	4,4,
	s_vertexBeam,
	s_indexBeam
};
