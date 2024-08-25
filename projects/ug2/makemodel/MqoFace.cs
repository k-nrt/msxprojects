using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// mqo フェイス型.
	/// </summary>
	public class Face
	{
		public int Vertices = 0;
		public int[] V = null;
		public int M = 0;
		public Float2[] UV = null;

		public Face() { }

		public Face(int nbVertices)
		{
			Vertices = nbVertices;
			V = new int[nbVertices];
			M = 0;
			UV = new Float2[nbVertices];
		}
	}
}
