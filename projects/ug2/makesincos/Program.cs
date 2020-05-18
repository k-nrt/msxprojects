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
			System.IO.StreamWriter writer = new System.IO.StreamWriter("sin_table.inc");

            writer.WriteLine("// hp (16bit) sin pi/2 plus");
            Write(writer, 0, 64, 256, 0xffff, 32768.0f,"(s16)0x{0,2:X4}");
            writer.WriteLine("// hp (16bit) sin pi/2 minus");
            Write(writer, 128, 64, 256, 0xffff, 32768.0f, "(s16)0x{0,2:X4}");

            writer.WriteLine("// lp (8bit) sin 2pi");
            Write(writer, 0, 256, 256, 0xff, 128.0f, "(s8)0x{0,2:X2}");

            writer.Close();
            System.Console.ReadLine();
        }

        static void Write(System.IO.StreamWriter writer, int nOffset, int nCount, int nResolution, int nMask, double fScale, string strFormat)
        { 
			for( int i = 0 ; i < nCount ; i++ )
			{
				double fRad = Math.PI*2.0*((double)(i+nOffset))/(double)(nResolution);
				double fSin = Math.Sin( fRad );
				
				fSin *= fScale;
				int nSin = 0;
                if((i+nOffset) < (nResolution/2))
                { 
                    nSin = ((int) Math.Ceiling(fSin)) & nMask;
                }
                else
                {
                    nSin = ((int)Math.Floor(fSin)) & nMask;
                }

                writer.Write(String.Format(strFormat, nSin));
		
				if( i%8 == 7 )
				{
					writer.WriteLine( "," );
				}
				else
				{
					writer.Write(",");
				}
			}
        }
	}
}
