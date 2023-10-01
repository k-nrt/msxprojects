using System;
using System.Collections.Generic;
using System.Drawing;

namespace msxbmp2mc
{
	class Program
	{
		static void Main(string[] args)
		{
			bool isObj = false;
			string strInputName = null;
			string strOutputName = null;
			for(int i = 0 ; i < args.Length ; i++)
			{
				if(args[i] == "-o")
				{
					strOutputName = args[i+1];
					i++;
				}
				else if(args[i] == "-obj")
				{
					isObj = true;
				}
				else
				{
					strInputName = args[i];
				}
			}

			if(strInputName == null)
			{
				return;
			}

			if(strOutputName == null)
			{
				return;
			}

			System.IO.StreamWriter writer = new System.IO.StreamWriter(strOutputName);

			Bitmap bitmap = new Bitmap(strInputName);
			Color colorCode0 = bitmap.GetPixel(0,0);

			for(int cy = 0 ; cy < bitmap.Height ; cy+=8)
			{
				for(int cx = 0 ; cx < bitmap.Width ; cx+=8)
				{
					for(int x = 0 ; x < 8 ; x+=2)
					{
						for(int y = 0 ; y < 8 ; y++)
						{
							Color colorLeft = bitmap.GetPixel(cx + x, cy + y);
							int nCodeLeft = GetNearesetColorCode(colorLeft);
							if(colorLeft == colorCode0)
							{
								nCodeLeft = 0;
							}

							Color colorRight = bitmap.GetPixel(cx + x + 1, cy + y);
							int nCodeRight = GetNearesetColorCode(colorRight);
							if(colorRight == colorCode0)
							{
								nCodeRight = 0;
							}

							if(isObj)
							{
								byte byteMask = 0x00;
								if( nCodeLeft == 0 && nCodeRight == 0 )
								{
									byteMask = 0xff;
								}
								else if(nCodeLeft == 0)
								{
									byteMask = 0xf0;
								}
								else if(nCodeRight == 0)
								{
									byteMask = 0x0f;
								}

								writer.BaseStream.WriteByte(byteMask);
							}

							byte bytePixel = (byte) ((nCodeLeft << 4) | (nCodeRight));

							writer.BaseStream.WriteByte(bytePixel);
						}
					}
				}
			}

			writer.Close();
		}

		static int GetNearesetColorCode(Color colorTarget)
		{
			Color[] colors = new Color[15]
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

			float fTR = colorTarget.R;
			float fTG = colorTarget.G;
			float fTB = colorTarget.B;

			float fNearest = -1.0f;
			int iCode = -1;
			for(int i = 0 ; i < colors.Length ; i++)
			{
				float fR = colors[i].R - colorTarget.R;
				float fG = colors[i].G - colorTarget.G;
				float fB = colors[i].B - colorTarget.B;

				float fDistance = fR*fR + fG*fG + fB*fB;

				if(iCode < 0 || fDistance < fNearest)
				{
					iCode = i;
					fNearest = fDistance;
				}
			}

			return iCode+1;
		}
	}
}
