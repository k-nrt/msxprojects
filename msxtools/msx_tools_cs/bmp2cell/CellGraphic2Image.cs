using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;

namespace bmp2cell
{
	class CellGraphic2Image
	{
		public class Pattern
		{
			public Color[] Colors = new Color[8*8];

			public int OriginalX = 0;
			public int OriginalY = 0;

			public Pattern() { }
			public Pattern(Bitmap bitmap, int x, int y)
			{
				OriginalX = x;
				OriginalY = y;
				for (int yy = 0; yy < 8; yy++)
				{
					for (int xx = 0; xx < 8; xx++)
					{
						Colors[xx + yy * 8] = bitmap.GetPixel(x + xx, y + yy);
					}
				}
			}

			public static bool operator==( Pattern pattern1, Pattern pattern2 )
			{
				return pattern1.Equals((object) pattern2);
			}

			public static bool operator !=(Pattern pattern1, Pattern pattern2)
			{
				return !pattern1.Equals((object)pattern2);
			}

			public override bool Equals(object obj)
			{
				Pattern target = (Pattern) obj;
				for (int i = 0; i < 64; i++)
				{
					if (Colors[i] != target.Colors[i])
					{
						return false;
					}
				}

				return true;
			}

			public override int GetHashCode()
			{
				int result = 0;

				foreach (Color color in Colors)
				{
					result = result ^ color.GetHashCode();
				}

				return result;
			}


			public Color GetPixel(int x, int y)
			{
				return Colors[x + y * 8];
			}

			public float Distance(Pattern pattern)
			{
				float fResult = 0.0f;

				ColorSpaceRgb colorSpaceRgb = new ColorSpaceRgb();

				for (int i = 0; i < 64; i++)
				{
					fResult += colorSpaceRgb.Distance(Colors[i], pattern.Colors[i]);
				}

				return fResult;
			}

			public int Dif(Pattern pattern)
			{
				int result = 0;

				for (int i = 0; i < 64; i++)
				{
					result += (int)Math.Abs(Colors[i].R - pattern.Colors[i].R);
					result += (int)Math.Abs(Colors[i].G - pattern.Colors[i].G);
					result += (int)Math.Abs(Colors[i].B - pattern.Colors[i].B);
				}

				return result;
			}
		}

		public Pattern[] Patterns = null;
		public int[] Names = null;
		public int Width = 0;
		public int Height = 0;

		public int PatternXCount { get { return Width / 8; } }
		public int PatternYCount { get { return Height / 8; } }

		public CellGraphic2Image() { }

		public CellGraphic2Image(Bitmap bitmap)
		{
			Width  = (bitmap.Width / 8) * 8;
			Height = (bitmap.Height / 8) * 8;

			Patterns = new Pattern[PatternXCount * PatternYCount];
			Names = new int[PatternXCount*PatternYCount];

			//. とりあえず作る.
			List<Pattern> listPatterns = new List<Pattern>();
			for (int y = 0; y < PatternYCount; y++)
			{
				Console.WriteLine("find cell ... {0}/{1}", y, PatternYCount);
				for (int x = 0; x < PatternXCount; x++)
				{
					Pattern pattern = new Pattern(bitmap, x*8, y*8 );
					int name = listPatterns.Count;
					
					if( listPatterns.Contains( pattern ) == true )
					{
						name = listPatterns.IndexOf( pattern );
					}
					else
					{
						listPatterns.Add(pattern);
					}

					Names[x + y * PatternXCount] = name;
				}
			}

			Patterns = listPatterns.ToArray();

			//. 全パターン間の差を計算する.
			List<PatternScore> listScores = new List<PatternScore>();
			for (int i = 0; i < listPatterns.Count - 1; i++)
			{
				Console.WriteLine("{0}/{1}", i, listPatterns.Count);

				for (int j = i + 1; j < listPatterns.Count; j++)
				{
					int score = listPatterns[i].Dif(listPatterns[j]);
					listScores.Add(new PatternScore(i,j, score));
				}
			}

			PatternScore[] arrayScores = listScores.ToArray();

			//. 差が多い順.
			Array.Sort( arrayScores );
			Array.Reverse(arrayScores);

			int[] remaps = new int[listPatterns.Count];
			List<int> listIndices = new List<int>();
			for (int i = 0; i < listPatterns.Count; i++)
			{
				listIndices.Add(i);
				remaps[i] = i;
			}

			for (int i = 0; i < arrayScores.Length; i++)
			{
				int p0 = arrayScores[i].Pattern0;
				int p1 = arrayScores[i].Pattern1;

				if (listIndices.Contains(p0) == true && listIndices.Contains(p1) )
				{
					listIndices.Remove(p0);

					for (; ; )
					{
						if (p1 == remaps[p1] )
						{
							break;
						}
					
						p1 = remaps[p1];
					}

					for (int j = 0; j < Names.Length; j++)
					{
						if (Names[j] == p0)
						{
							Names[j] = p1;
						}
					}
				}

				if (listIndices.Count <= 256)
				{
					break;
				}
			}

			List<int> listUniques = new List<int>();
			for (int i = 0; i < Names.Length; i++)
			{
				if (listUniques.Contains(Names[i]) == false)
				{
					listUniques.Add(Names[i]);
				}
			}


			System.IO.StreamWriter writer = new System.IO.StreamWriter("log.txt");
			foreach (PatternScore score in arrayScores)
			{
				writer.WriteLine("Score {0}:{1}:{2}", score.Pattern0, score.Pattern1, score.Score);
			}

			for (int i = 0; i < listIndices.Count; i++)
			{
				writer.WriteLine("index {0} : {1}", i, listIndices[i]);
			}

			for (int i = 0; i < remaps.Length; i++)
			{
				writer.WriteLine("remap {0} -> {1}", i, remaps[i]);
			}

			for (int i = 0; i < listUniques.Count; i++)
			{
				writer.WriteLine("unique {0}:{1}", i, listUniques[i]);
			}
			writer.Close();


			/*
			System.IO.StreamWriter writer = new System.IO.StreamWriter("log.txt");

			for (int i = 0; i < dicPatterns.Count; i++)
			{
				writer.WriteLine("Pattern {0} : {1}", i, dicPatterns.Values.ElementAt(i));
			}

			writer.Close();
			 */

			Console.WriteLine("Patterns {0}/{1}" ,listPatterns.Count, PatternXCount*PatternYCount);
			Console.ReadLine();
		}

		class PatternScore : IComparable
		{
			public int Pattern0 = 0;
			public int Pattern1 = 0;
			public int Score = 0;

			public PatternScore(int pattern0, int pattern1,int score)
			{
				Pattern0 = pattern0;
				Pattern1 = pattern1;
				Score = score;
			}

			public int CompareTo(object obj)
			{
				if (Score > ((PatternScore)obj).Score)
				{
					return -1;
				}
				else if (Score < ((PatternScore)obj).Score)
				{
					return 1;
				}
				else
				{
					return 0;
				}
			}
		}

		public int FindPattern(List<Pattern> listPatterns, Pattern target)
		{
			for( int i = 0 ; i < listPatterns.Count ; i++ )
			{
				if ( listPatterns[i].Distance(target) == 0.0f)
				{
					return i;
				}
			}

			return -1;
		}

		public Bitmap CreateBitmap()
		{
			Bitmap bitmap = new Bitmap(Width ,Height);


			for (int y = 0; y < PatternYCount; y++)
			{
				for (int x = 0; x < PatternXCount; x++)
				{
					Pattern pattern = Patterns[Names[x+y*PatternXCount]];

					for (int yy = 0; yy < 8; yy++)
					{
						for (int xx = 0; xx < 8; xx++)
						{
							bitmap.SetPixel(x * 8 + xx, y * 8 + yy, pattern.GetPixel(xx, yy));
						}
					}
				}
			}

			return bitmap;
		}
	}
}
