using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Ugx
{
	public class ModelFactory
	{
		public static Model Create( Mqo.Document mqoDocument, string strPrefix, float fScale )
		{
			Model model = new Model();
			List<Vertex> listVertices = new List<Vertex>();
			List<Edge> listEdges = new List<Edge>();

			int nVertexOffset = 0;
			foreach(Mqo.Object mqoObject in mqoDocument.Object )
			{
				if(mqoObject.visible == 0 && strPrefix == "")
				{
					continue;
				}

				if(strPrefix != "")
				{
					if(mqoObject.Name.Length < strPrefix.Length)
					{
						continue;
					}

					if (mqoObject.Name.Substring(0, strPrefix.Length) != strPrefix)
					{
						continue;
					}
				}
				else
				{
					if (mqoObject.visible == 0 )
					{
						continue;
					}
				}

				foreach(Mqo.Float3 f3Vertex in mqoObject.vertex )
				{
					listVertices.Add(new Vertex(f3Vertex.X * fScale, -f3Vertex.Y * fScale, f3Vertex.Z * fScale));
				}

				foreach(Mqo.Face face in mqoObject.face)
				{
					for (int i = 0; i < face.Vertices; i++)
					{
						int i0 = i;
						int i1 = (i + 1) % face.Vertices;
						Edge edge = new Edge(nVertexOffset + face.V[i0], nVertexOffset + face.V[i1]);
						AddEdge(listEdges, edge);
					}
				}
				nVertexOffset += mqoObject.vertex.Length;
			}

			model.Vertices = listVertices.ToArray();
			model.Edges = listEdges.ToArray();

			return model;
		}

		public static void AddEdge( List<Edge> listEdges, Edge edgeAdd )
		{
			foreach( Edge edge in listEdges )
			{
				if( edge == edgeAdd )
				{
					return;
				}
			}

			listEdges.Add(edgeAdd);
		}
	}
}
