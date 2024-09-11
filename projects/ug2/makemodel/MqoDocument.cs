using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// mqo ドキュメント型.
	/// </summary>
	public class Document 
	{
		public Scene Scene = new Scene();
		public Material[] Material = null;
		public List<Object> Object = new List<Object>();

		public Document()
		{
		}
	}

}
