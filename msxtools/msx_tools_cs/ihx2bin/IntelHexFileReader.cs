using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using static System.Net.WebRequestMethods;

namespace ihx2bin
{
    internal class IntelHexFileReader
    {
        public IntelHexFileReader(StreamReader reader)
        {
            m_image = new byte[65536];

            while(true)
            {
                //. 1 行読む.
                string line = reader.ReadLine();
                if (line == null)
                {
                    break;
                }

                //. この行のバイト数.
                int length = GetByte(line[1], line[2]);

                //. 書き込みアドレス.
                int offset = GetWord(line[3], line[4], line[5], line[6]);

                //. タイプ.
                int type = GetByte(line[7], line[8]);

                if (type == 0x00)
                {
                    //. フツーのバイナリ.
                    for (int i = 0; i < length; i++)
                    {
                        m_image[offset + i] = (byte) GetByte(line[9 + i * 2], line[10 + i * 2]);
                    }
                }
		        else if(type == 0x01 )
        		{
		        	//. 終了.
        			break;
		        }
        	}
        }

        private int GetHex(char c)
        {
            if ('0' <= c && c <= '9')
            {
                return c - '0';
            }
            else if ('A' <= c && c <= 'F')
            {
                return 10 + (c - 'A');
            }

            return 0;
        }

        private int GetByte(char h, char l)
        {
            return GetHex(h) << 4 | GetHex(l);
        }

        int GetWord(char hh, char hl, char lh, char ll)
        {
            return GetByte(hh, hl) << 8 | GetByte(lh, ll);
        }

        public byte[] Image
        {
            get { return m_image; }
        }

        private byte[] m_image;
    }
}
