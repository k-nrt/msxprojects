using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mqo
{
	/// <summary>
	/// 構文ツリー.
	/// トークン列から構文ツリーを生成.
	/// </summary>
	public class Tree
	{
		public List<Node> RootNode = new List<Node>() { new Node() };

		public Tree(List<Token> tokens)
		{
			Stack<Node> parentNodes = new Stack<Node>();
			Node node = null;

			foreach(Token token in tokens)
			{
				if(token.Name == "{" || token.Name == "(")
				{
					parentNodes.Push(node);
				}
				else if (token.Name == "}" || token.Name == ")")
				{
					parentNodes.Pop();
				}
				else
				{
					node = new Node();
					node.Token = token;
					if (0 < parentNodes.Count)
					{
						parentNodes.Peek().Children.Add(node);
					}
					else
					{
						RootNode[0].Children.Add(node);
					}
				}
			}
		}

		public void Dump(System.IO.StreamWriter writer )
		{
			Dump(writer, 0, RootNode);
		}

		void Dump(System.IO.StreamWriter writer, int depth,List<Node> nodes)
		{
			foreach(Node node in nodes)
			{
				for (int i = 0; i < depth; i++)
				{
					writer.Write("\t");
				}

				if (node.Token == null)
				{
					writer.WriteLine("!root!");
				}
				else
				{
					writer.WriteLine(node.Token.Name);
				}

				Dump(writer, depth + 1, node.Children);
			}
		}
	}
}
