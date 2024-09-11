using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// 構文ツリーノードのイテレータ.
	/// </summary>
	public class NodeIterator
	{
		public List<Node> Nodes = null;
		public int Position = 0;

		public NodeIterator( List<Node> nodes, int nPosition)
		{
			Nodes = nodes;
			Position = nPosition;
		}

		public Node GetNode()
		{
			return Nodes[Position];
		}

		public NodeIterator GetChildren()
		{
			return new NodeIterator(Nodes[Position].Children, 0);
		}

		public string Name()
		{
			return Nodes[Position].Token.Name;
		}

		public void Next()
		{
			Position++;
		}

		public void Back()
		{
			Position--;
		}

		public Node Find(string strName)
		{
			for( ; Position < Nodes.Count ; Position++ )
			{
				if(Nodes[Position].Token.Name == strName)
				{
					return Nodes[Position];
				}
			}

			return null;
		}

		public bool IsEnd()
		{
			if( Nodes.Count <= Position)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
}
