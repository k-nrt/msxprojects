const SUgxModelVertex g_vertexBeam[] =
{
	{ 0,0,19 },
	{ 8,0,9 },
	{ 0,0,-26 },
	{ -8,0,8 },
};
const u8 g_indexBeam[] =
{
	LINE(3,2),
	LINE(2,1),
	LINE(1,0),
	LINE(0,3),
};
const SUgxModel g_modelBeam =
{
	10, 10, 10, 10, 10, 10,
	4,4,
	g_vertexBeam,
	g_indexBeam
};
