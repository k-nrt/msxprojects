using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace bmp2sprite
{
    abstract class Dither
    {
        public abstract int SelectDither(int x, int y, float fLerp);
    }

    class Dither1x1 : Dither
    {
        public override int SelectDither(int x, int y, float fLerp)
        {
            return 0;
        }
    }

    class Dither2x2 : Dither
    {
        public static int[] s_ditherPattern2x2 = new int[2 * 2]
		{
			0x0, 0x3,
			0x2, 0x1,
		};

        public override int SelectDither(int x, int y, float fLerp)
        {
            int lerp = (int)(fLerp * 4.0f);
            int xx = x % 2;
            int yy = y % 2;

            if (s_ditherPattern2x2[yy * 2 + xx] < lerp)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }

    class Dither4x4 : Dither
    {
        public static int[] s_ditherPattern4x4 = new int[4 * 4]
		{
			0x0, 0x6, 0x3, 0xf,
			0x5, 0x4, 0xe, 0xd,
			0x2, 0xc, 0x1, 0x9,
			0xb, 0xa, 0x8, 0x7,
		};

        public override int SelectDither(int x, int y, float fLerp)
        {
            int lerp = (int)(fLerp * 16.0f);
            int xx = x % 4;
            int yy = y % 4;

            if (s_ditherPattern4x4[yy * 4 + xx] < lerp)
            {
                return 1;
            }
            else
            {
                return 0;
            }
        }
    }
}
