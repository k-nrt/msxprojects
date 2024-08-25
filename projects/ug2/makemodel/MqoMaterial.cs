using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// mqo マテリアル型.
	/// </summary>
	public class Material
	{
		public string Name = ""; //"mat1"
		public int shader = 0; //shader 3
		public Float4 col = new Float4(); //col 0.447 0.412 1.000 1.000
		public float dif = 0.0f; //dif0.800
		public float amb = 0.0f; //amb 0.600
		public float emi = 0.0f; //emi 0.000
		public float spc = 0.0f; //spc 0.000
		public float power = 0.0f; //power 5.00
	}
}
