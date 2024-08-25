using System;
using System.Collections.Generic;

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
