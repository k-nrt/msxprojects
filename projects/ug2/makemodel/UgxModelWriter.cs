using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ugx
{
	public class ModelWriter
	{
		public static void Write(string strFileName, Model model, string strName)
		{
			System.IO.StreamWriter writer = new System.IO.StreamWriter(strFileName);

			writer.WriteLine("const SUgxModelVertex g_vertex{0}[] =", strName);
			writer.WriteLine("{");
			foreach(Vertex vertex in model.Vertices)
			{
				writer.WriteLine("\t{{ {0:d},{1:d},{2:d} }},", vertex.X, vertex.Y, vertex.Z);
			}
			writer.WriteLine("};");

			writer.WriteLine("const u8 g_index{0}[] =", strName);
			writer.WriteLine("{");
			foreach (Edge edge in model.Edges)
			{
				writer.WriteLine("\tLINE({0:d},{1:d}),", edge.Start,edge.End);
			}
			writer.WriteLine("};");

			writer.WriteLine("const SUgxModel g_model{0} =", strName);
			writer.WriteLine("{");
			writer.WriteLine("\t10, 10, 10, 10, 10, 10,");
			writer.WriteLine("\t{0},{1},",model.Vertices.Length, model.Edges.Length);
			writer.WriteLine("\tg_vertex{0},",strName);
			writer.WriteLine("\tg_index{0}",strName);
			writer.WriteLine("};");

			writer.Close();


		}
	}
}
