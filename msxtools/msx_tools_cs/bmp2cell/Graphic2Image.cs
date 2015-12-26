using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace bmp2cell
{
	class Graphic2Image
	{
		private ColorList m_colorList = null;
		public int Width = 0;
		public int Height = 0;
		public int[] Pixels = null;

		public int[] ForegroundColors = null;
		public int[] BackgroundColors = null;

		public Graphic2Image() { }
		public Graphic2Image(IndexedImage indexedImage, ColorSpace colorSpace )
		{
			m_colorList = indexedImage.ColorList;
			Width  = indexedImage.Width;
			Height = indexedImage.Height;

			Pixels = new int[Width*Height];
			ForegroundColors = new int[Width*Height];
			BackgroundColors = new int[Width*Height];

			for (int y = 0; y < Height; y++)
			{
				if (y % 10 == 0)
				{
					Console.WriteLine("converting to graphic2 ... {0}/{1}", y, Height);
				}

				for (int x = 0; x < Width; x += 8)
				{
					//. ユニークカラーの取得.
					List<int> listIndices = new List<int>();
					for (int i = 0; i < 8; i++)
					{
						if (x + i >= Width)
						{
							continue;
						}

						int index = indexedImage.GetIndex(x + i, y);
						if (listIndices.Contains(index) == false)
						{
							listIndices.Add(index);
						}
					}

					if( listIndices.Count <= 1 )
					{
						for (int i = 0; i < 8; i++)
						{
							if (x + i >= Width)
							{
								continue;
							}
							SetIndex( x+ i , y,  listIndices[0], listIndices[0], 0 );
						}
					}
					else
					{
						List<ColorDistance> listColorDistance = new List<ColorDistance>();
						ColorSpaceRgb colorSpaceRgb = new ColorSpaceRgb();
						Color color0, color1;
						for (int i = 0; i < listIndices.Count - 1; i++)
						{
							for (int j = i + 1; j < listIndices.Count; j++)
							{
								color0 = indexedImage.ColorList.Colors[listIndices[i]];
								color1 = indexedImage.ColorList.Colors[listIndices[j]];
								listColorDistance.Add( new ColorDistance( listIndices[i], listIndices[j], colorSpaceRgb.Distance( color0, color1 ) ) );
							}
						}

						ColorDistance[] arrayColorDistance = listColorDistance.ToArray();
						Array.Sort(arrayColorDistance);

						int index0 = arrayColorDistance[0].Index0;
						int index1 = arrayColorDistance[0].Index1;
						color0 = indexedImage.ColorList.Colors[index0];
						color1 = indexedImage.ColorList.Colors[index1];

						for (int i = 0; i < 8; i++)
						{
							if (x + i >= Width)
							{
								continue;
							}

							Color color = indexedImage.GetPixel(x + i, y);
							float d0 = colorSpaceRgb.Distance(color0, color);
							float d1 = colorSpaceRgb.Distance(color1, color);

							SetIndex(x + i, y, d0 < d1 ? index0 : index1, index0, index1 );
						}
					}



					/*
					//. 多い色２つ取ってくる.
					Dictionary<int, int> dicColors = new Dictionary<int, int>();

					for (int i = 0; i < 8; i++)
					{
						if ((x + i) >= Width)
						{
							continue;
						}

						int color = indexedImage.GetIndex(x + i, y);
						if (dicColors.ContainsKey(color))
						{
							dicColors[color] += 1;
						}
						else
						{
							dicColors.Add(color, 1);
						}
					}

					if (dicColors.Keys.Count == 1)
					{
						for (int i = 0; i < 8; i++)
						{
							if (x + i >= Width)
							{
								continue;
							}

							SetIndex( x+ i , y, dicColors.Keys.ElementAt(0), dicColors.Keys.ElementAt(0), 0 );
						}
					}
					else
					{
						ColorCount[] colors = new ColorCount[dicColors.Keys.Count];
						for (int i = 0; i < colors.Length; i++)
						{
							colors[i] = new ColorCount(dicColors.Keys.ElementAt(i), dicColors.Values.ElementAt(i));
						}

						Array.Sort(colors);

						for (int i = 0; i < 8; i++)
						{
							if (x + i >= Width)
							{
								continue;
							}

							Color color = indexedImage.GetPixel(x + i, y);
							float d0 = colorSpace.Distance( m_colorList.GetColor(colors[0].Color), color);
							float d1 = colorSpace.Distance( m_colorList.GetColor(colors[1].Color), color);

							SetIndex(x + i, y, d0 < d1 ? colors[0].Color : colors[1].Color, colors[0].Color, colors[1].Color);
						}
					}
					*/
				}
			}
		}

		class ColorCount : IComparable
		{
			public int Color = 0;
			public int Count = 0;

			public ColorCount(int color, int count)
			{
				Color = color;
				Count = count;
			}

			public int CompareTo(object obj)
			{
				if (Count > ((ColorCount)obj).Count)
				{
					return -1;
				}
				else if (Count < ((ColorCount)obj).Count)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}

		}

		class ColorDistance : IComparable
		{
			public int Index0;
			public int Index1;
			public float Distance;

			public ColorDistance(int index0, int index1, float distance)
			{
				Index0 = index0;
				Index1 = index1;
				Distance = distance;
			}

			public int CompareTo(object obj)
			{
				if (Distance > ((ColorDistance)obj).Distance)
				{
					return -1;
				}
				else if (Distance < ((ColorDistance)obj).Distance)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		};


		public void SetIndex( int x, int y, int color, int foreground, int background )
		{
			int pos = y*Width+x;
			Pixels[pos] = color;
			ForegroundColors[pos] = foreground;
			BackgroundColors[pos] = background;
		}

		public int GetIndex( int x, int y )
		{
			return Pixels[y*Width+x];
		}

		public bool IsForgroudColor(int x, int y)
		{
			return ( GetIndex(x,y) == ForegroundColors[y*Width+x] ? true : false );
		}

		public Color GetPixel( int x, int y )
		{
			return m_colorList.GetColor( GetIndex( x, y ) );
		}

		public Bitmap CreateBitmap()
		{
			Bitmap bitmap = new Bitmap(Width, Height);

			for (int y = 0; y < Height; y++)
			{
				for (int x = 0; x < Width; x++)
				{
					bitmap.SetPixel(x, y, GetPixel(x, y));
				}
			}

			return bitmap;
		}

		public void OutputPatterns(string strFileName, Rectangle rect)
		{
			System.IO.StreamWriter writer = new System.IO.StreamWriter(strFileName);

			writer.WriteLine("\t// pattern name table");
			int pid = 0;
			for (int py = rect.Y ; py < (rect.Y + rect.Height) ; py += 8)
			{
				for (int px = rect.X ; px < (rect.X + rect.Width) ; px += 8)
				{
					writer.Write("\t");
					for (int y = 0; y < 8; y++)
					{
						int pattern = 0;
						for (int x = 0; x < 8; x++)
						{
							pattern |= (IsForgroudColor(px + x, py + y) ? 1 << (7 - x) : 0);
						}

						writer.Write(string.Format("0x{0,0:X2},", pattern));
					}
					writer.WriteLine("// {0:d}", pid);
					pid++;
				}
			}
			/*
			writer.Close();
		}

		public void OutputColors(string strFileName, Rectangle rect)
		{
			 * 
			System.IO.StreamWriter writer = new System.IO.StreamWriter(strFileName);

			 */

			writer.WriteLine("\t// color table");
			pid = 0;
			for (int py = rect.Y ; py < (rect.Y + rect.Height) ; py += 8)
			{
				for (int px = rect.X ; px < (rect.X + rect.Width) ; px+= 8 )
				{
					writer.Write( "\t" );
					for (int y = 0; y < 8; y++)
					{
						int pos = (py+y)*Width + px;
						int color = ((ForegroundColors[pos]+1) << 4) | (BackgroundColors[pos] + 1);

						writer.Write( string.Format( "0x{0,0:X2},", color ));
					}
					writer.WriteLine( "// {0:d}", pid );
					pid++;
				}
			}

			writer.Close();
		}
	}
}
