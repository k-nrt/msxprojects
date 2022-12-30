using System;
using System.Drawing;

namespace bmp2cell
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

		/*
		public int GetNearestColorIndex(Color colorTarget)
		{
			float fNearest = -1.0f;
			int iCode = -1;
			for (int i = 0; i < Colors.Length; i++)
			{
				float fDistance = Imaging.Distance(colorTarget, Colors[i]);

				if (iCode < 0 || fDistance < fNearest)
				{
					iCode = i;
					fNearest = fDistance;
				}
			}

			return iCode;
		}
		 */

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

		public int GetNearestColorIndexDither(Color colorTarget, int x, int y, ColorSpace colorSpace, Dither dither )
		{
			ColorDistance[] distances = new ColorDistance[Colors.Length];
			for (int i = 0; i < distances.Length; i++)
			{
				distances[i] = new ColorDistance(i,Colors[i], colorSpace.Distance(Colors[i], colorTarget));
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

			Color color1 = distances[Colors.Length-1].Color;
			Color color2 = distances[Colors.Length-2].Color;
			float f = distances[Colors.Length - 1].Distance + distances[13].Distance;
			float fLerp = 0.0f;

			if (f > 0)
			{
				fLerp = FMath.Powf(distances[Colors.Length - 1].Distance / f, 1.0f);
			}

			return (dither.SelectDither(x, y, fLerp) == 0 ? distances[Colors.Length - 1].Index : distances[Colors.Length - 2].Index);
		}
	}
}
