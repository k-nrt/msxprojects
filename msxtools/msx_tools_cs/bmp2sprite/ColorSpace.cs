using System;
using System.Drawing;

namespace bmp2sprite
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

            float pow = 1.0f; //.2.0f
            fY1 = FMath.Powf(fY1 / 255.0f, pow);
            fCb1 /= 255.0f;
            fCr1 /= 255.0f;

            fY2 = FMath.Powf(fY2 / 255.0f, pow);
            fCb2 /= 255.0f;
            fCr2 /= 255.0f;

            float fScale = 1.0f;// 2.4f;// 1.6f;
            float fY = (fY1 - fY2) * 1.0f;// fScale;
            float fCb = (fCb1 - fCb2) * fScale;
            float fCr = (fCr1 - fCr2) * fScale;

            //return FMath.Sqrtf( fY * fY + fCb * fCb + fCr * fCr );
            return fY * fY + fCb * fCb + fCr * fCr;
            //return 10.0f - (fY1 * fY2 + fCb1 * fCb2 + fCr1 * fCr2);
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
}
