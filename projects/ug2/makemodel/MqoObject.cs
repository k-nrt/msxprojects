using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// mqo オブジェクト型.
	/// </summary>
	public class Object
	{
		public string Name = "";
		public int depth = 0;
		public int folding = 0;
		public Float3 scale = new Float3();// 1.000000 1.000000 1.000000
		public Float3 rotation = new Float3(); //0.000000 0.000000 0.000000
		public Float3 translation = new Float3(); // 0.000000 0.000000 0.000000
		public int visible = 0; // 15
		public int locking = 0;
		public int shading = 0;
		public float facet = 59.5f;
		public Float3 color = new Float3(); //0.898 0.498 0.698
		public int color_type = 0;
		public Float3[] vertex = null;
		public Face[] face = null;
	}
}
