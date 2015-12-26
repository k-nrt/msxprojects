using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace makesincos
{
	class Program
	{
		static void Main(string[] args)
		{
			int nCount = 64;

			System.IO.StreamWriter writer = new System.IO.StreamWriter("sin_table.inc");

			for( int i = 0 ; i < nCount ; i++ )
			{
				double fRad = (((double)i)/((double)nCount))*Math.PI/2.0;
				double fSin = Math.Sin( fRad );
				
				fSin *= 256.0f;
				int nSin = ((int) Math.Ceiling(fSin)) & 0xffff;


				writer.Write(String.Format("0x{0,2:X4}", nSin));
		
				if( i%8 == 7 )
				{
					writer.WriteLine( "," );
				}
				else
				{
					writer.Write(",");
				}
			}

			for (int i = 0; i < nCount; i++)
			{
				double fRad = (((double)(i+128)) / ((double)nCount)) * Math.PI / 2.0;
				double fSin = Math.Sin(fRad);

				fSin *= 256.0f;
				int nSin = ((int)Math.Floor(fSin)) & 0xffff;
				

				writer.Write(String.Format("0x{0,2:X4}", nSin));

				if (i % 8 == 7)
				{
					writer.WriteLine(",");
				}
				else
				{
					writer.Write(",");
				}
			}


			writer.Close();
			System.Console.ReadLine();
		}
	}
}
