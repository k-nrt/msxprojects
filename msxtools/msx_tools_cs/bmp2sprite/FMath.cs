using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace bmp2sprite
{
    class FMath
    {
        public static float Powf(float x, float y)
        {
            return (float)Math.Pow((double)x, (double)y);
        }

        public static float Sqrtf(float x)
        {
            return (float)Math.Sqrt((double)x);
        }
    }
}
