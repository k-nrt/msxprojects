using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace bmp2sprite
{
	class ColorList
	{
		public Color[] Colors = null;

		public ColorList() { }
		public ColorList(Color[] colors)
		{
			Colors = colors;
		}

		public Color GetColor(int iIndex)
		{
			return Colors[iIndex];
		}

		class ColorDistance
		{
			public int Index = 0;
			public Color Color = new Color();
			public float Distance = 0.0f;

			public ColorDistance(int index, Color color, float fDistance)
			{
				Index = index;
				Color = Color.FromArgb(color.ToArgb());
				Distance = fDistance;
			}
		};

        public int GetNearestColor(Color colorTarget, ColorSpace colorSpace )
        {
            if (colorTarget.ToArgb() == Colors[0].ToArgb())
            {
                //. color key.
                return 0;
            }

            ColorDistance[] distances = new ColorDistance[Colors.Length-1];
            for (int i = 0; i < distances.Length; i++)
            {
                distances[i] = new ColorDistance(i+1, Colors[i+1], colorSpace.Distance(Colors[i+1], colorTarget));
            }

            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j < distances.Length - i - 1; j++)
                {
                    if (distances[j].Distance < distances[j + 1].Distance)
                    {
                        ColorDistance swap = distances[j];
                        distances[j] = distances[j + 1];
                        distances[j + 1] = swap;
                    }
                }
            }

            return distances[distances.Length - 1].Index;
        }

		public int GetNearestColorIndexDither(Color colorTarget, int x, int y, ColorSpace colorSpace, Dither dither )
		{
            if( Colors.Length <= 1 )
            {
                return 0;
            }

            if (Colors.Length <= 2)
            {
                return 1;
            }

            if (colorTarget.ToArgb() == Colors[0].ToArgb())
            {
                //. color key.
                return 0;
            }

			ColorDistance[] distances = new ColorDistance[Colors.Length-1];
			for (int i = 0; i < distances.Length; i++)
			{
				distances[i] = new ColorDistance(i+1,Colors[i+1], colorSpace.Distance(Colors[i+1], colorTarget));
			}

			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < distances.Length - i - 1; j++)
				{
					if (distances[j].Distance < distances[j + 1].Distance)
					{
						ColorDistance swap = distances[j];
						distances[j] = distances[j + 1];
						distances[j + 1] = swap;
					}
				}
			}

			Color color1 = distances[distances.Length-1].Color;
            Color color2 = distances[distances.Length - 2].Color;
            float f = distances[distances.Length - 1].Distance + distances[distances.Length - 2].Distance;
			float fLerp = 0.0f;

			if (f > 0)
			{
                fLerp = FMath.Powf(distances[distances.Length - 1].Distance / f, 1.0f);
			}

            return (dither.SelectDither(x, y, fLerp) == 0 ? distances[distances.Length - 1].Index : distances[distances.Length - 2].Index);
		}
	}
}
