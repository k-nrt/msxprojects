using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace makemap
{
    class Program
    {
        static void Main(string[] args)
        {
            string strPath = "r-type2-240x180.png";

            Bitmap bitmap = new Bitmap(strPath);

            Dictionary<uint, int> colors = new Dictionary<uint, int>();
            colors.Add(0xff000000, Int32.MaxValue);
            colors.Add(0xffe0e0e0, Int32.MaxValue);

            Bitmap bitmap333 = MakeRGB333(bitmap, colors, 15);

            int chipWidth = 40;
            int chipHeight = 12;

            int horizontalChips = bitmap.Width/chipWidth;
            int verticalChips = bitmap.Height/chipHeight;
            
            List<MapChip> mapChips = new List<MapChip>();
            int[][] mapIndices = new int[horizontalChips][];

            for (int x = 0; x < horizontalChips; x++)
            {
                mapIndices[x] = new int[verticalChips];

                for (int y = 0; y < verticalChips; y++)
                {
                    MapChip mapChip = new MapChip(x, y, chipWidth, chipHeight, bitmap333);

                    if (!mapChips.Contains(mapChip))
                    {
                        mapChips.Add(mapChip);
                        System.Console.WriteLine("{0} {1} {2}", x, y, mapChips.Count);
                    }
                    int index = mapChips.IndexOf(mapChip);
                    mapIndices[x][y] = index;
                }
            }

            int outputChipWidth = 256;
            int outputChipHeight = 512;
            int cx = 0;
            int cy = 0;
            Bitmap bitmapChips = new Bitmap(outputChipWidth, outputChipHeight);
            foreach (MapChip mapChip in mapChips)
            {
                for (int y = 0; y < chipHeight; y++)
                {
                    for (int x = 0; x < chipWidth; x++)
                    {
                        Color pixel = bitmap333.GetPixel(mapChip.m_chipX*chipWidth + x, mapChip.m_chupY*chipHeight + y); 
                        bitmapChips.SetPixel(cx + x, cy + y, pixel);
                    }
                }

                cx += chipWidth;
                if (outputChipWidth < (cx + chipWidth))
                {
                    cx = 0;
                    cy += chipHeight;
                    if (outputChipHeight < (cy + chipHeight))
                    {
                        break;
                    }
                }
            }

            bitmapChips.Save("chips.png", ImageFormat.Png);

            int[][] colorIndices = new int[bitmapChips.Width][];
            List<int> outputColors = new List<int>();
            for (int x = 0; x < bitmapChips.Width; x++)
            {
                colorIndices[x] = new int[bitmapChips.Height];

                for (int y = 0; y < bitmapChips.Height; y++)
                {
                    int argb = bitmapChips.GetPixel(x,y).ToArgb();
                    if (!outputColors.Contains(argb))
                    {
                        outputColors.Add(argb);
                    }

                    int index = outputColors.IndexOf(argb);
                    colorIndices[x][y] = index;
                }
            }

            
            FileStream writer = new FileStream("stage1.rbg",FileMode.Create,FileAccess.Write);
            foreach(int argb in outputColors)
            {
                int r = (argb >> 21) & 0x07;
                int g = (argb >> 13) & 0x07;
                int b = (argb >> 5) & 0x07;

                writer.WriteByte((byte) ((r << 4) | b));
                writer.WriteByte((byte) g);
            }
            writer.Close();

            writer = new FileStream("stage1.pgt", FileMode.Create, FileAccess.Write);
            for(int y = 0; y < bitmapChips.Height; y++)
            {
                for (int x = 0; x < bitmapChips.Width; x+=2)
                {
                    int h = colorIndices[x][y];
                    int l = colorIndices[x+1][y];

                    writer.WriteByte((byte)(((h & 0xf) << 4) | (l & 0xf)));
                }
            }
            writer.Close();

            writer = new FileStream("stage1.pnt", FileMode.Create, FileAccess.Write);
            for (int x = 0; x < horizontalChips; x++)
            {
                for (int y = 0; y < verticalChips; y++)
                {
                    int name = mapIndices[x][y] & 0xff;
                    writer.WriteByte((byte) name);
                }
            }

            writer.Close();
            System.Console.ReadLine();
        }

        static Bitmap MakeRGB333(Bitmap bitmap, Dictionary<uint, int> colors, int maxColors)
        {
            Bitmap bitmap333 = new Bitmap(bitmap.Width, bitmap.Height);

            if (colors.Count <= maxColors)
            { 
                for(int y = 0; y < bitmap.Height; y++)
                {
                    for(int x = 0; x < bitmap.Width; x++)
                    {
                        uint argb = (uint) bitmap.GetPixel(x,y).ToArgb();
                        argb &= 0xffe0e0e0;
                        bitmap333.SetPixel(x,y,Color.FromArgb((int)argb));

                        if (colors.ContainsKey(argb))
                        {
                            if (colors[argb] < Int32.MaxValue)
                            {
                                colors[argb]++;
                            }
                        }
                        else
                        {
                            colors.Add(argb,1);
                        }
                    }
                }

                while(maxColors < colors.Count)
                {
                    uint nearestKey0 = 0;
                    uint nearestKey1 = 0;
                    int nearestDistance = -1;

                    foreach (uint key0 in colors.Keys)
                    {
                        foreach (uint key1 in colors.Keys)
                        {
                            if ( key0 == key1)
                            {
                                continue;
                            }

                            int distance = CalcDistance(key0, key1);
                            if (nearestDistance < 0|| distance < nearestDistance)
                            {
                                nearestKey0 = key0;
                                nearestKey1 = key1;
                                nearestDistance = distance;
                            }
                        }
                    }

                    Console.WriteLine("Combine {0:X} {1:X} {2}", nearestKey0, nearestKey1, nearestDistance);

                    if (colors[nearestKey0] < colors[nearestKey1])
                    {
                        colors.Remove(nearestKey0);
                    }
                    else
                    {
                        colors.Remove(nearestKey1);
                    }
                }
            }
                
            Console.WriteLine("colors {0}", colors.Count);
            foreach(uint key in colors.Keys)
            {
                Console.WriteLine("{0:X} {1}", key, colors[key]);
            }

            for (int y = 0; y < bitmap.Height; y++)
            {
                for (int x = 0; x < bitmap.Width; x++)
                {
                    uint argb = (uint)bitmap333.GetPixel(x, y).ToArgb();
                    argb &= 0xffe0e0e0;

                    //argb = FindColor(argb, colors);
                    argb = FindDitherColor(x, y, argb, colors);
                    bitmap333.SetPixel(x, y, Color.FromArgb((int)argb));

                }
            }
            bitmap333.Save("rgb333.png", ImageFormat.Png);

            return bitmap333;
        }

        static int CalcDistance(uint rgb0, uint rgb1)
        {
            int r0 = (int)((rgb0 & 0x00ff0000) >> 16);
            int g0 = (int)((rgb0 & 0x0000ff00) >> 8);
            int b0 = (int)((rgb0 & 0x000000ff) >> 0);

            int r1 = (int)((rgb1 & 0x00ff0000) >> 16);
            int g1 = (int)((rgb1 & 0x0000ff00) >> 8);
            int b1 = (int)((rgb1 & 0x000000ff) >> 0);

            return Abs(r0 - r1) + Abs(g0 - g1) + Abs(b0 - b1);
        }

        static int Abs(int value)
        {
            if (0 < value)
            {
                return value;
            }
            else
            {
                return -value;
            }
        }

        static uint FindColor(uint argb, Dictionary<uint,int> colors)
        {
            uint resultKey = 0;
            int nearestDistance = -1;
            foreach(uint key in colors.Keys)
            {
                if (key == argb)
                {
                    return key;
                }

                int distance = CalcDistance(argb, key);
                if (nearestDistance < 0 || distance <= nearestDistance)
                {
                    resultKey = key;
                    nearestDistance = distance;
                }
            }

            return resultKey;
        }

        static uint FindDitherColor(int x, int y, uint argb, Dictionary<uint, int> colors)
        {
            uint primaryKey = 0;
            int primaryDistance = -1;
            foreach (uint key in colors.Keys)
            {
                if (key == argb)
                {
                    primaryKey = key;
                    primaryDistance = 0;
                    break;
                }

                int distance = CalcDistance(argb, key);
                if (primaryDistance < 0 || distance < primaryDistance)
                {
                    primaryKey = key;
                    primaryDistance = distance;
                }
            }

            uint secondaryKey = 0;
            int secondaryDistance = -1;
            foreach (uint key in colors.Keys)
            {
                if (key == primaryKey)
                {
                    continue;
                }

                int distance = CalcDistance(argb, key);
                if (secondaryDistance < 0 || distance < secondaryDistance)
                {
                    secondaryKey = key;
                    secondaryDistance = distance;
                }
            }


            int[][] dither = new int[][]
            {
                new int[] {0,   8,  2, 10 },
                new int[] {12,  4, 14,  6 },
                new int[] {0,  11,  1,  9 },
                new int[] {15,  7, 13,  5 },
            };

            int dx = x & 0x03;
            int dy = y & 0x03;

            //primaryDistance *= primaryDistance;
            int threshold = 16 - (int) ((primaryDistance*16)/(primaryDistance + secondaryDistance));

            if (dither[dx][dy] <= threshold)
            {
                return primaryKey;
            }
            else
            {
                return secondaryKey;
            }
        }
    }
}
