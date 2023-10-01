using System;
using System.Collections.Generic;
using System.Drawing;

namespace bmp2cell
{
	class Program
	{
		static void Main(string[] args)
		{
			//. NULL パターン位置
			//. パレット位置
			//. 固定パターン位置

			//. マップ位置
			//. ディザ有無
			//. 色距離 RGB YCbCr 色相優先

			//. パターンファイル名
			//. ネームファイル名

			//. 単純変換 bmp2bmp


			Rectangle rectNull = new Rectangle();
			Rectangle rectPalette = new Rectangle();
			Rectangle rectPatterns = new Rectangle();
			Rectangle rectNames = new Rectangle();
			List<Rectangle> listObjects = new List<Rectangle>();
			string strDither = "1x1";
			string strColorSpace = "RGB";

			string strPatternFileName = null;
			string strNameFileName = null;
			string strBitmapFileName = null;

			string strInputFileName = null;

			for (int i = 0; i < args.Length; i++)
			{
				switch (args[i])
				{
					case "-null":
						rectNull = new Rectangle( int.Parse( args[i+1] ), int.Parse( args[i+2] ), int.Parse( args[i+3] ), int.Parse( args[i+4] ) );
						i += 4;
						break;

					case "-palette":
						rectPalette = new Rectangle(int.Parse(args[i + 1]), int.Parse(args[i + 2]), int.Parse(args[i + 3]), int.Parse(args[i + 4]));
						i += 4;
						break;

					case "-pattern":
						rectPatterns = new Rectangle(int.Parse(args[i + 1]), int.Parse(args[i + 2]), int.Parse(args[i + 3]), int.Parse(args[i + 4]));
						i += 4;
						break;

					case "-name":
						rectNames = new Rectangle(int.Parse(args[i + 1]), int.Parse(args[i + 2]), int.Parse(args[i + 3]), int.Parse(args[i + 4]));
						i += 4;
						break;

					case "-dither":
						strDither = args[i+1];
						i++;
						break;

					case "-color":
						strColorSpace = args[i+1];
						i++;
						break;

					case "-patternout":
						strPatternFileName = args[i + 1];
						i++;
						break;

					case "-nameout":
						strNameFileName = args[i + 1];
						i++;
						break;

					case "-bitmapout":
						strBitmapFileName = args[i + 1];
						i++;
						break;

					default:
						strInputFileName = args[i];
						break;
				}
			}

			Bitmap bitmapSource  = new Bitmap(strInputFileName);

			//. パンチスルーイメージ.
			Bitmap bitmapNull = null;
			if (rectNull.Width != 0 && rectNull.Height != 0)
			{
				bitmapNull = new Bitmap(8, 8);
				for (int y = 0; y < 8; y++)
				{
					for (int x = 0; x < 8; x++)
					{
						bitmapNull.SetPixel(x, y, bitmapSource.GetPixel(rectNull.X + x, rectNull.Y + y));
					}
				}
			}

			//. パレット作成.
			Color[] colors = s_colors;
			if (rectPalette.Width != 0 && rectPalette.Height != 0)
			{
				List<Color> listColors = new List<Color>();
				for (int y = 0; y < rectPalette.Height; y+= 8)
				{
					for (int x = 0; x < rectPalette.Width; x+=8)
					{
						listColors.Add(bitmapSource.GetPixel(rectPalette.X + x, rectPalette.Y + y));

					}
				}

				colors = listColors.ToArray();
			}


			ColorList colorList = new ColorList(colors);

			ColorSpace colorSpace = new ColorSpaceRgb();
			if (strColorSpace == "RGB")
			{
				colorSpace = new ColorSpaceRgb();
			}
			else if (strColorSpace == "YCbCr")
			{
				colorSpace = new ColorSpaceYCbCr();
			}

			Dither dither = new Dither1x1();
			if (strDither == "1x1")
			{
				dither = new Dither1x1();
			}
			else if (strDither == "2x2")
			{
				dither = new Dither2x2();
			}
			else if (strDither == "4x4")
			{
				dither = new Dither4x4();
			}

			IndexedImage indexedImage = new IndexedImage(colorList, bitmapSource, colorSpace, dither );
			Bitmap bitmap = indexedImage.CreateBitmap();
			bitmap.Save("temp.png");

			ColorSpaceRgb colorSpaceRgb = new ColorSpaceRgb();
			Graphic2Image graphic2Image = new Graphic2Image(indexedImage,colorSpaceRgb);

			Bitmap bitmapGraphic2 = graphic2Image.CreateBitmap();
			if (strBitmapFileName != null)
			{
				bitmapGraphic2.Save( strBitmapFileName );
			}


			if (strNameFileName != null && strPatternFileName != null)
			{
				CellGraphic2Image cellImage = new CellGraphic2Image(bitmapGraphic2);
				Bitmap bitmapCell = cellImage.CreateBitmap();
				bitmapCell.Save("cell.png");//, System.Drawing.Imaging.ImageFormat.Png);
			}

			if (strPatternFileName != null)
			{
				if (rectPatterns.Width <= 0 || rectPatterns.Height <= 0 )
				{
					rectPatterns = new Rectangle(0, 0, graphic2Image.Width, graphic2Image.Height);
				}

				graphic2Image.OutputPatterns( strPatternFileName, rectPatterns );
			}
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
	}
}
