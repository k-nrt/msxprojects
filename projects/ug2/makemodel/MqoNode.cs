using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Mqo
{
	/// <summary>
	/// 構文ツリーのノード.
	/// </summary>
	public class Node
	{
		public Token Token = null;
		public List<Node> Children = new List<Node>();
	}
}
