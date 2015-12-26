using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ugx
{
	public class Vertex
	{
		public int X = 0;
		public int Y = 0;
		public int Z = 0;

		public Vertex()
		{ }

		public Vertex(float x,float y, float z)
		{
			X = (int) x;
			Y = (int) y;
			Z = (int) z;
		}
	}
}
