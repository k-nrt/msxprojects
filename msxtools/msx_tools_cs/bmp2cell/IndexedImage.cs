using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace bmp2cell
{
	class IndexedImage
	{
		public ColorList ColorList = null;
		public int[] Pixels = null;
		public int Width = 0;
		public int Height = 0;

		public IndexedImage() { }
		public IndexedImage(ColorList colorList, Bitmap bitmap, ColorSpace colorSpace, Dither dither )
		{
			ColorList = colorList;
			Width = bitmap.Width;
			Height = bitmap.Height;

			Pixels = new int[Width * Height];

			for (int y = 0; y < Height; y++)
			{
				if (y % 10 == 0)
				{
					Console.WriteLine("Indexing... {0}/{1}", y, Height);
				}

				for (int x = 0; x < Width; x++)
				{
					Color color = bitmap.GetPixel(x, y);
					SetIndex( x, y, ColorList.GetNearestColorIndexDither(color, x, y, colorSpace, dither) );
					//SetIndex( x, y, ColorList.GetNearestColorIndex(color) );
				}
			}
		}

		public void SetIndex(int x, int y, int index)
		{
			Pixels[y * Width + x] = index;
		}

		public int GetIndex(int x, int y)
		{
			return Pixels[y * Width + x];
		}

		public Color GetPixel(int x, int y)
		{
			return ColorList.GetColor(GetIndex(x, y));
		}

		public Bitmap CreateBitmap()
		{
			Bitmap bitmap = new Bitmap(Width, Height);

			for (int y = 0; y < Height; y++)
			{
				for (int x = 0; x < Width; x++)
				{
					bitmap.SetPixel(x, y, ColorList.GetColor(GetIndex(x,y)));
				}
			}

			return bitmap;
		}

	}
}
