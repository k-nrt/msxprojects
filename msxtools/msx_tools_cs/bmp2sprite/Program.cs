using System;
using System.Collections.Generic;
using System.Linq;
using System.Drawing;
using System.Drawing.Imaging;

namespace bmp2sprite
{
	class Program
	{
		static void Main(string[] args)
		{
			string strInputName = "";
			string strOutputName = "";

            Rectangle rectPalette = new Rectangle(0, 0, 0, 0);
            Rectangle rectPatterns = new Rectangle(0, 0, 8 * 16, 8 * 16);

            ColorSpace colorSpace = new ColorSpaceYCbCr();

            bool isSpriteMode2 = false;

			for( int i = 0 ; i < args.Length ; i++ )
			{
				switch( args[i] )
				{
					case "-o":
						strOutputName = args[i+1];
						i++;
						break;

                    case "-palette":
                        rectPalette = new Rectangle(int.Parse(args[i + 1]), int.Parse(args[i + 2]), int.Parse(args[i + 3]), int.Parse(args[i + 4]));
                        i += 4;
                        break;

                    case "-pattern":
                        rectPatterns = new Rectangle(int.Parse(args[i + 1]), int.Parse(args[i + 2]), int.Parse(args[i + 3]), int.Parse(args[i + 4]));
                        i += 4;
                        break;

                    case "-spritemode2":
                        isSpriteMode2 = true;
                        break;

					default:
						strInputName = args[i];
						break;
				}
			}

			if( strInputName == "" )
			{
				return;
			}

			if( strOutputName == "" )
			{
				return;
			}

			Bitmap bitmap = new Bitmap( strInputName );

            ColorList colorList = CreatePalette(bitmap, rectPalette);

            CreateSpriteMode2(strOutputName, bitmap, rectPatterns, colorList, colorSpace);

            Bitmap test = ReduceColors(bitmap, rectPatterns, colorList );
            test.Save("test.bmp");

            /*

			System.IO.StreamWriter writer = new System.IO.StreamWriter(strOutputName);

			Color colorZero = bitmap.GetPixel(0, 0);

			int nXCount = bitmap.Width/16;
			int nYCount = bitmap.Height/16;

			for (int y = 0; y < nYCount; y++)
			{
				for (int x = 0; x < nXCount; x++)
				{
					writer.Write("\t");

					//. 左半分.
					for (int yy = 0; yy < 16; yy++)
					{
						int nBits = 0;
						for (int xx = 0; xx < 8; xx++)
						{
							if (bitmap.GetPixel(x * 16 + xx, y * 16 + yy) != colorZero)
							{
								nBits |= 0x80 >> xx;
							}
						}

						writer.Write( string.Format( "0x{0:X2},", nBits ) ); 
					}

					writer.Write( " " );

					//. 右半分.
					for (int yy = 0; yy < 16; yy++)
					{
						int nBits = 0;
						for (int xx = 0; xx < 8; xx++)
						{
							if (bitmap.GetPixel(x * 16 + xx + 8, y * 16 + yy) != colorZero)
							{
								nBits |= 0x80 >> xx;
							}
						}

						writer.Write( string.Format( "0x{0:X2},", nBits ) ); 

						if( yy < 15 )
						{
//							writer.Write( "," );
						}
					}

					writer.WriteLine();
				}
			}

			writer.Close();
             */
		}

		static Color[] s_colors = new Color[15]
		{
			Color.FromArgb( 0xff, 0x00, 0x00, 0x00 ),
			Color.FromArgb( 0xff, 0x21, 0xdb, 0x20 ),
			Color.FromArgb( 0xff, 0x6a, 0xff, 0x6b ),

			Color.FromArgb( 0xff, 0x21, 0x24, 0xff ),
			Color.FromArgb( 0xff, 0x4a, 0x6d, 0xff ),
			Color.FromArgb( 0xff, 0xb5, 0x24, 0x21 ),
			Color.FromArgb( 0xff, 0x55, 0xd3, 0xff ),

			Color.FromArgb( 0xff, 0xff, 0x24, 0x22 ),
			Color.FromArgb( 0xff, 0xfe, 0x6d, 0x6a ),
			Color.FromArgb( 0xff, 0xdf, 0xdb, 0x20 ),
			Color.FromArgb( 0xff, 0xde, 0xdb, 0x94 ),

			Color.FromArgb( 0xff, 0x21, 0x91, 0x21 ),
			Color.FromArgb( 0xff, 0xdf, 0x49, 0xb5 ),
			Color.FromArgb( 0xff, 0xb6, 0xb6, 0xb6 ),
			Color.FromArgb( 0xff, 0xff, 0xff, 0xff ),
		};

        static ColorList CreatePalette(Bitmap bitmapSource, Rectangle rectPalette)
        {
            //. パレット作成.
            Color[] colors = s_colors;
            if (rectPalette.Width != 0 && rectPalette.Height != 0)
            {
                List<Color> listColors = new List<Color>();
                for (int y = 0; y < rectPalette.Height; y += 8)
                {
                    for (int x = 0; x < rectPalette.Width; x += 8)
                    {
                        listColors.Add(bitmapSource.GetPixel(rectPalette.X + x, rectPalette.Y + y));

                    }
                }

                colors = listColors.ToArray();
            }

            return new ColorList(colors);
        }

        class SpritePattern
        {
            public int[] Colors = new int[16];
            public int[] Patterns = new int[32];

            public SpritePattern()
            {
                for(int i = 0 ; i < 16 ; i++ )
                {
                    Colors[i] = 0;
                }

                for(int i = 0 ; i < 32 ; i++ )
                {
                    Patterns[i] = 0;
                }
            }

            public bool IsNull()
            {
                for( int i = 0 ; i < 32 ; i++ )
                {
                    if(Patterns[i] != 0)
                    {
                        return false;
                    }
                }

                return true;
            }

            public void WritePatterns( System.IO.StreamWriter writer )
            {
                for (int i = 0; i < 32; i++)
                {
                    writer.Write(string.Format("0x{0:X2},", Patterns[i]));
                }
                writer.WriteLine();
            }

            public void WriteColors( System.IO.StreamWriter writer )
            {
                for (int i = 0; i < 16; i++)
                {
                    writer.Write(string.Format("0x{0:X2},", Colors[i]));
                }
                writer.WriteLine();
            }
        }

        static void CreateSpriteMode2( string strOutputName, Bitmap bitmap, Rectangle rectPatterns, ColorList colorList, ColorSpace colorSpace )
        {
            System.IO.StreamWriter writer = new System.IO.StreamWriter(strOutputName);

            for (int y = 0; y < rectPatterns.Height; y += 16)
            {
                int yy = rectPatterns.Y + y;
                for( int x = 0 ; x < rectPatterns.Width ; x+=16 )
                {
                    int xx = rectPatterns.X + x;

                    SpritePattern pattern0 = new SpritePattern();
                    SpritePattern pattern1 = new SpritePattern();

                    for (int i = 0; i < 16; i++)
                    {
                        //. 3 色探す.
                        Dictionary<int, int> dicIndexToBits = FindColors(bitmap, xx, yy + i, colorList, colorSpace);

                        pattern1.Colors[i] = 0x60;

                        if( dicIndexToBits.Count <= 0 )
                        {
                            continue;
                        }

                        if( dicIndexToBits.Count <= 3 )
                        {
                            pattern0.Colors[i] = dicIndexToBits.Keys.ElementAt(0);
                        }

                        if( dicIndexToBits.Count >= 2 )
                        {
                            pattern1.Colors[i] |= dicIndexToBits.Keys.ElementAt(1);
                        }

                        for( int j = 0 ; j < 8 ; j++ )
                        {
                            int leftIndex = colorList.GetNearestColor(bitmap.GetPixel(xx + j, yy + i),colorSpace);
                            if( dicIndexToBits.ContainsKey( leftIndex ))
                            {
                                int leftBits = dicIndexToBits[leftIndex];
                                if( (leftBits & 0x1) == 0x1) 
                                {
                                    pattern0.Patterns[i] |= 0x80 >> j;
                                }

                                if( (leftBits & 0x2) == 0x2)
                                {
                                    pattern1.Patterns[i] |= 0x80 >> j;
                                }
                            }

                            int rightIndex = colorList.GetNearestColor(bitmap.GetPixel(xx + 8+ j, yy + i), colorSpace);
                            if (dicIndexToBits.ContainsKey(rightIndex))
                            {
                                int rightBits = dicIndexToBits[rightIndex];
                                if ((rightBits & 0x1) == 0x1)
                                {
                                    pattern0.Patterns[16+i] |= 0x80 >> j;
                                }

                                if ((rightBits & 0x2) == 0x2)
                                {
                                    pattern1.Patterns[16 + i] |= 0x80 >> j;
                                }
                            }
                        }
                    }

                    if (!pattern0.IsNull() && !pattern1.IsNull() )
                    {
                        writer.WriteLine("\t//. 2.");
                        pattern0.WritePatterns(writer);
                        pattern1.WritePatterns(writer);
                        pattern0.WriteColors(writer);
                        pattern1.WriteColors(writer);
                    }
                    else if( !pattern0.IsNull() )
                    {
                        writer.WriteLine("\t//. 1.");
                        pattern0.WritePatterns(writer);
                        pattern0.WriteColors(writer);
                    }
                }
            }

            writer.Close();
        }

        static Dictionary<int,int> FindColors(Bitmap bitmap,int x, int y, ColorList colorList, ColorSpace colorSpace )
        {
            Dictionary<int, int> dicColors = new Dictionary<int, int>();

            for( int i = 0 ; i < 16 ; i++ )
            {
                Color color = bitmap.GetPixel(x + i, y);
                int index = colorList.GetNearestColor(color, colorSpace );

                if (index <= 0)
                {
                    continue;
                }

                if( dicColors.ContainsKey( index ) )
                {
                    dicColors[index] += 1;
                }
                else
                {
                    dicColors.Add(index, 1);
                }
            }

            Dictionary<int,int> result = new Dictionary<int,int>();
            if( dicColors.Count <= 0 )
            {
                //return null;
            }
            else if (dicColors.Count <= 1)
            {
                //. 単色.
                result.Add( dicColors.Keys.ElementAt(0), 1 );
                return result;
            }
            else if( dicColors.Count <= 2)
            {
                //. 2 色.
                result.Add( dicColors.Keys.ElementAt(0), 1 );
                result.Add( dicColors.Keys.ElementAt(1), 2 );
                return result;
            }
            else if( dicColors.Count <= 3 )
            {
                int i0 = dicColors.Keys.ElementAt(0);
                int i1 = dicColors.Keys.ElementAt(1);
                int i2 = dicColors.Keys.ElementAt(2);

                if (i0 == (i1 | i2)  )
                {
                    result.Add(i1, 1);
                    result.Add(i2, 2);
                    result.Add(i0, 3);
                }
                else if( i1 == (i2 | i0))
                {
                    result.Add(i2, 1);
                    result.Add(i0, 2);
                    result.Add(i1, 3);
                }
                else if( i2 == (i0 | i1))
                {
                    result.Add(i0, 1);
                    result.Add(i1, 2);
                    result.Add(i2, 3);
                }
            }

            return result;
        }

        static Bitmap ReduceColors( Bitmap bitmapSrc, Rectangle rectPattern, ColorList colorListPalette )
        {
            Bitmap result = new Bitmap(rectPattern.Width, rectPattern.Height);

            ColorSpace colorSpace = new ColorSpaceYCbCr();
            Dither dither = new Dither2x2();

            //ColorList[] colorLists = new ColorList


            Color colorKey = bitmapSrc.GetPixel(0,0);

            for (int y = 0; y < rectPattern.Height; y++)
            {
                int yy = rectPattern.Y + y;
                for (int x = 0; x < rectPattern.Width; x+= 16)
                {
                    Dictionary<Color, int> dicColorCount = new Dictionary<Color, int>();
                    dicColorCount.Add( colorKey, 16 );

                    for( int i = 0 ; i < 16 ; i++ )
                    {
                        int xx = rectPattern.X + x + i;

                        Color color = bitmapSrc.GetPixel(xx, yy);

                        if( dicColorCount.ContainsKey(color))
                        {
                            dicColorCount[color] += 1;
                        }
                        else
                        {
                            dicColorCount.Add(color, 1);
                        }
                    }

                    Color[] colors = dicColorCount.Keys.ToArray();
                    int[] counts = dicColorCount.Values.ToArray();

                    for( int i = 0 ; i < colors.Length - 1 ; i++ )
                    {
                        for( int j = 0 ; j < colors.Length -  1 - i ; j++ )
                        {
                            if( counts[j] < counts[j+1] )
                            {
                                int swap0 = counts[j];
                                Color colorSwap = colors[j];
                                counts[j] = counts[j + 1];
                                colors[j] = colors[j + 1];
                                counts[j + 1] = swap0;
                                colors[j + 1] = colorSwap;
                            }
                        }
                    }
/*
                    20 2
                    21 3
                    22 2
                    23 3
                    24 6
                    25 7
                    26 6
                    27 7
                    28 10
                    29 11
                    210 10
                    211 11
                    212 14
                    213 15
                    214 14
                    215 15

                    30 3
                    31 3
                    32 3
                    33 3
                    34 7
                    35 7
                    36 7
                    37 7
                    38 11
                    39 11
                    310 11
                    311 11
                    312 15
                    313 15
                    314 15
                    315 15
*/

                    ColorList colorList = new ColorList(((colors.Length < 4) ? colors.ToArray() : colors.Take(4).ToArray() ));

                    for (int i = 0; i < 16; i++)
                    {
                        int xx = rectPattern.X + x + i;

                        Color color = bitmapSrc.GetPixel(xx, yy);
                        int index = 0;
                        if (color.ToArgb() == colorKey.ToArgb())
                        {
                            result.SetPixel(x + i, y, colorKey);
                        }
                        else
                        {
                            index = colorList.GetNearestColorIndexDither(color, xx, yy, colorSpace, dither);
                            //index = colorListPalette.GetNearestColorIndexDither(colorList.GetColor(index),xx,yy,colorSpace,dither);
                            //result.SetPixel(x + i, y, colorListPalette.GetColor(index));
                            result.SetPixel(x + i, y, colorList.GetColor(index));
                        }
                    }
                }
            }

            return result;
        }
	}
}
