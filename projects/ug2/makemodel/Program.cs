using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace makemodel
{
	class Program
	{
		static void Main(string[] args)
		{
			string strInput = "";
			string strOutput = "";
			float fScale = 1.0f;
			string strPrefix = "";
			string strName = "";

			for (int i = 0; i < args.Length;i++ )
			{
				string arg = args[i];
				switch (arg)
				{
					case "-i":
						i++;
						strInput = args[i];
						break;

					case "-o":
						i++;
						strOutput = args[i];
						break;

					case "-scale":
						i++;
						float.TryParse(args[i], out fScale);
						break;

					case "-prefix":
						i++;
						strPrefix = args[i];
						break;

					case "-name":
						i++;
						strName = args[i];
						break;
				}
			}

			if(strInput == "")
			{
				Console.WriteLine("-i がない");
				return;
			}

			if(strOutput == "")
			{
				Console.WriteLine("-o がない");
				return;
			}

			if(strName == "")
			{
				Console.WriteLine("-name がない");
				return;
			}


			List<Mqo.Token> listTokens = Mqo.Tokenizer.Tokenize( strInput );
			if(listTokens==null)
			{
				Console.WriteLine( strInput + "が読めない");
				return;
			}

			/*
			System.IO.StreamWriter writer = new System.IO.StreamWriter("tokens.txt");
			tokenizer.Dump(writer);
			writer.Close();
			*/

			Mqo.Tree tree = new Mqo.Tree(listTokens);

			/*
			writer = new System.IO.StreamWriter("tree.txt");
			tree.Dump(writer);
			writer.Close();
			*/

			Mqo.NodeIterator it = new Mqo.NodeIterator(tree.RootNode, 0);

			Mqo.Document document = Mqo.Parser.ParseDocument( it.GetChildren() );

			Ugx.Model model = Ugx.ModelFactory.Create(document, strPrefix, fScale);

			Ugx.ModelWriter.Write(strOutput, model, strName);
		}
	}
}
