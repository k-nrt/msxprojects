using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mqo
{
	/// <summary>
	/// mqo シーン型.
	/// </summary>
	public class Scene
	{
		public Float3 pos = new Float3(); // 0.0000 0.0000 1500.0000
		public Float3 lookat = new Float3(); //. 0.0000 0.0000 0.0000
		public float head = 0.0f; //-0.5236
		public float pich = 0.0f; //0.5236
		public float bank = 0.0f; //0.0000
		public int ortho = 0; //0
		public float zoom2 = 0.0f; //5.0000
		public Float3 amb = new Float3(); //0.250 0.250 0.250
		public Light[] dirlights = null;

		public Scene() { }
	}
}
