const static s8x3 s_vertexEnemy2Defence[] =
{
	{ 10,-14,8 },
	{ 4,-14,13 },
	{ 5,-4,17 },
	{ 15,-5,8 },
	{ 5,7,5 },
	{ -6,-19,10 },
	{ -6,-11,13 },
	{ -29,-19,14 },
	{ -13,-23,3 },
	{ 8,-21,-1 },
	{ 1,-25,-9 },
	{ 14,-25,-24 },
	{ 12,-14,-2 },
	{ -9,-19,3 },
	{ 2,-20,-5 },
	{ -8,-6,7 },
	{ 7,-8,-6 },
	{ 1,-20,9 },
	{ -4,-21,2 },
	{ 2,-16,10 },
	{ 3,-21,0 },
	{ -13,28,13 },
	{ -13,26,-5 },
	{ 0,2,11 },
	{ -29,16,3 },
	{ 19,24,-15 },
	{ 9,11,-30 },
	{ 11,1,0 },
	{ 0,24,-17 },
	{ -9,-33,29 },
	{ 16,-9,23 },
	{ -11,39,24 },
	{ -36,-9,23 },
	{ 40,39,-24 },
	{ -8,-28,-10 },
	{ -18,-28,-24 },
	{ 38,42,-31 },
	{ 37,42,-25 },
	{ 42,38,-29 },
	{ -17,-24,-12 },
	{ -7,-32,-19 },
};
const static u16 s_indexEnemy2Defence[] =
{
	LINE(0,1),
	LINE(1,2),
	LINE(2,3),
	LINE(0,3),
	LINE(3,4),
	LINE(4,2),
	LINE(5,6),
	LINE(6,7),
	LINE(7,5),
	LINE(5,8),
	LINE(8,7),
	LINE(9,10),
	LINE(10,11),
	LINE(11,9),
	LINE(9,12),
	LINE(12,11),
	LINE(1,13),
	LINE(14,0),
	LINE(13,15),
	LINE(15,2),
	LINE(3,16),
	LINE(16,14),
	LINE(17,18),
	LINE(18,19),
	LINE(19,20),
	LINE(20,17),
	LINE(21,22),
	LINE(22,23),
	LINE(23,21),
	LINE(21,24),
	LINE(24,23),
	LINE(25,26),
	LINE(26,27),
	LINE(27,28),
	LINE(28,25),
	LINE(27,25),
	LINE(29,30),
	LINE(30,31),
	LINE(31,32),
	LINE(32,29),
	LINE(29,31),
	LINE(33,34),
	LINE(35,36),
	LINE(33,37),
	LINE(37,36),
	LINE(33,38),
	LINE(38,36),
	LINE(34,39),
	LINE(39,35),
	LINE(34,40),
	LINE(40,35),
	LINE(37,39),
	LINE(38,40),
};
const SMesh g_meshEnemy2Defence =
{
	41,53,
	s_vertexEnemy2Defence,
	s_indexEnemy2Defence
};
