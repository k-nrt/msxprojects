﻿using System;
using System.Collections.Generic;

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
		public static void WriteMesh(string strFileName, Model model, string strName)
		{
			System.IO.StreamWriter writer = new System.IO.StreamWriter(strFileName);

			writer.WriteLine("const static s8x3 s_vertex{0}[] =", strName);
			writer.WriteLine("{");
			foreach (Vertex vertex in model.Vertices)
			{
				writer.WriteLine("\t{{ {0:d},{1:d},{2:d} }},", vertex.X, vertex.Y, vertex.Z);
			}
			writer.WriteLine("};");

			writer.WriteLine("const static u16 s_index{0}[] =", strName);
			writer.WriteLine("{");
			foreach (Edge edge in model.Edges)
			{
				writer.WriteLine("\tLINE({0:d},{1:d}),", edge.Start, edge.End);
			}
			writer.WriteLine("};");

			writer.WriteLine("const SMesh g_mesh{0} =", strName);
			writer.WriteLine("{");
			writer.WriteLine("\t{0},{1},", model.Vertices.Length, model.Edges.Length);
			writer.WriteLine("\ts_vertex{0},", strName);
			writer.WriteLine("\ts_index{0}", strName);
			writer.WriteLine("};");

			writer.Close();


		}
	}
}
