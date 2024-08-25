using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// トークナイザ.
	/// トークン列を生成.
	/// </summary>
	public class Tokenizer
	{
		/// <summary>
		/// トークン抽出.
		/// </summary>
		/// <param name="strFileName">ファイル名</param>
		public static List<Token> Tokenize(string strFileName)
		{
			List<Token> tokens = new List<Token>();
			System.IO.StreamReader reader = new System.IO.StreamReader(strFileName);
			
			if(reader.EndOfStream)
			{
				return null;
			}

			while (!reader.EndOfStream)
			{
				string strLine = reader.ReadLine();
				string strToken = "";
				bool isString = false;
				bool isEscape = false;

				foreach(char c in strLine)
				{
					if(isString)
					{
						//. 文字列だったら全部取ってくる.
						if(isEscape)
						{
							//. エスケープ中.
							strToken += c;
							isEscape = false;
						}
						else if( c == '\\' )
						{
							//. エスケープ開始.
							isEscape = true;
						}
						else if( c == '"')
						{
							//. 文字列終了.
							strToken += c;
							AppendToken(tokens, strToken);
							strToken ="";
							isString = false;
						}
						else
						{
							strToken += c;
						}
					}
					else
					{
						//. 区切り文字.
						string strSplitters = " \t";

						//. 区切り文字だけど自分もトークン.
						string strSeparators = "(){}";

						if( 0 <= strSeparators.IndexOf(c) )
						{
							//. トークン区切り文字.
							AppendToken(tokens,strToken);
							strToken = "";

							strToken += c;
							AppendToken(tokens, strToken);
							strToken = "";
						}
						else if (0<= strSplitters.IndexOf(c))
						{
							//. 区切り文字.
							AppendToken(tokens, strToken);
							strToken = "";
						}
						else if(c == '\"')
						{
							//. 文字列開始.
							AppendToken(tokens, strToken);
							strToken = "";

							strToken += c;
							isString = true;
						}
						else
						{
							strToken += c;
						}
					}
				}
				AppendToken(tokens, strToken);
				//AppendToken("\n");
			}

			reader.Close();

			return tokens;
		}

		public static void AppendToken(List<Token> tokens, string strToken)
		{
			if(0<strToken.Length)
			{
				tokens.Add(new Token(strToken));
			}
		}

		public void Dump(List<Token> tokens, System.IO.StreamWriter writer)
		{
			foreach(Token token in tokens)
			{
				if (token.Name == "\n")
				{
					writer.WriteLine("[EOL]");
				}
				else
				{
					writer.WriteLine(token.Name);
				}
			}
		}
	}
}
