using System;
using System.Collections.Generic;

namespace Ugx
{
	public class Edge
	{
		public int Start = 0;
		public int End = 0;

		public Edge() {}
		public Edge(int start, int end)
		{
			Start = start;
			End = end;
		}

		public static bool operator==(Edge a,Edge b)
		{
			if( a.Start == b.Start && a.End == b.End )
			{
				return true;
			}
			if( a.Start == b.End && a.End == b.Start )
			{
				return true;
			}

			return false;
		}

		public static bool operator!=(Edge a,Edge b)
		{
			return !(a==b);
		}
	}
}
