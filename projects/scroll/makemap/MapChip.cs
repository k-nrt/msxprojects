using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace makemap
{
    class MapChip
    {
        int[] m_pixels = null;
        public int m_chipX;
        public int m_chupY;
        int m_width;
        int m_height;

        public MapChip(int chipX, int chipY, int w, int h, Bitmap bitmap)
        {
            m_pixels = new int[w*h];
            m_chipX = chipX;
            m_chupY = chipY;
            m_width = w;
            m_height = h;

            for (int px = 0; px < w; px++)
            {
                for (int py = 0; py < h; py++)
                {
                    int xx = px + chipX * w;
                    int yy = py + chipY * h;

                    Color color = bitmap.GetPixel(xx, yy);
                    int argb = color.ToArgb();

                    m_pixels[px+py*w] = argb & 0x00e0e0e0;
                }
            }
        }

        public override bool Equals(object obj)
        {
            MapChip mapChip = (MapChip) obj;

            if (m_width != mapChip.m_width || m_height != mapChip.m_height)
            {
                return false;
            }    

            for(int i = 0; i < m_pixels.Length; i++)
            {
                if(m_pixels[i] != mapChip.m_pixels[i])
                {
                    return false;
                }    
            }
            return true;
        }
    }
}
