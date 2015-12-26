using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace bmp2cell
{
	abstract class ColorSpace
	{
		public abstract float Distance(Color color1, Color color2);
	}

	class ColorSpaceYCbCr : ColorSpace
	{
		public override float Distance(Color color1, Color color2)
		{
			float fY1 = 0.29891f * ((float)color1.R) + 0.58661f * ((float)color1.G) + 0.11448f * ((float)color1.B);
			float fCb1 = -0.16874f * ((float)color1.R) - 0.33126f * ((float)color1.G) + 0.50000f * ((float)color1.B);
			float fCr1 = 0.50000f * ((float)color1.R) - 0.41869f * ((float)color1.G) - 0.08131f * ((float)color1.B);

			float fY2 = 0.29891f * ((float)color2.R) + 0.58661f * ((float)color2.G) + 0.11448f * ((float)color2.B);
			float fCb2 = -0.16874f * ((float)color2.R) - 0.33126f * ((float)color2.G) + 0.50000f * ((float)color2.B);
			float fCr2 = 0.50000f * ((float)color2.R) - 0.41869f * ((float)color2.G) - 0.08131f * ((float)color2.B);

			float pow = 2.0f; //.2.0f
			fY1 = FMath.Powf(fY1 / 255.0f, pow);
			fCb1 /= 255.0f;
			fCr1 /= 255.0f;

			fY2 = FMath.Powf(fY2 / 255.0f, pow);
			fCb2 /= 255.0f;
			fCr2 /= 255.0f;

			float fScale = 2.4f;// 2.4f;// 1.6f;
			float fY = (fY1 - fY2) * 1.0f;// fScale;
			float fCb = (fCb1 - fCb2) * fScale;
			float fCr = (fCr1 - fCr2) * fScale;

			//return FMath.Sqrtf( fY * fY + fCb * fCb + fCr * fCr );
			return fY * fY + fCb * fCb + fCr * fCr;
			//return Powf(Sqrtf(fY * fY + fCb * fCb + fCr * fCr), 4.8f);
		}
	}

	class ColorSpaceRgb : ColorSpace
	{
		public override float Distance(Color color1, Color color2)
		{
			float fR = color1.R - color2.R;
			float fG = color1.G - color2.G;
			float fB = color1.B - color2.B;

			return FMath.Sqrtf(fR * fR + fG * fG + fB * fB);
		}
	}

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

		public override int  SelectDither(int x, int y, float fLerp)
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
