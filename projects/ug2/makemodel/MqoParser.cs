using System;
using System.Collections.Generic;

namespace Mqo
{
	/// <summary>
	/// mqo パーサ.
	/// </summary>
	public class Parser
	{
		public static Document ParseDocument(NodeIterator it)
		{
			Document document = new Document();

			var fieldInfos = document.GetType().GetFields();

			for (; !it.IsEnd(); it.Next())
			{
				System.Reflection.FieldInfo fieldInfo = FindFieldInfo(document.GetType().GetFields(), it.Name());
				if (fieldInfo == null)
				{
					continue;
				}

				Type type = fieldInfo.FieldType;
				if (type == typeof(Scene))
				{
					fieldInfo.SetValue(document, ParseScene(it.GetChildren()));
				}
				else if (type == typeof(Material[]))
				{
					it.Next();
					fieldInfo.SetValue(document, ParseMaterialArray(it));
				}
				else if (type == typeof(List<Object>))
				{
					it.Next();
					document.Object.Add(ParseObject(it));
				}
			}

			return document;
		}

		public static System.Reflection.FieldInfo FindFieldInfo(System.Reflection.FieldInfo[] fieldInfos, string name)
		{
			foreach (var fieldInfo in fieldInfos)
			{
				if (fieldInfo.Name == name)
				{
					return fieldInfo;
				}
			}

			return null;
		}

		public static Scene ParseScene(NodeIterator it)
		{
			Scene scene = new Scene();
			System.Reflection.FieldInfo[] fieldInfos = scene.GetType().GetFields();

			for (; !it.IsEnd(); it.Next())
			{
				System.Reflection.FieldInfo fieldInfo = FindFieldInfo(scene.GetType().GetFields(), it.Name());
				if (fieldInfo == null)
				{
					continue;
				}

				it.Next();

				if (fieldInfo.FieldType == typeof(Light[]))
				{
					fieldInfo.SetValue(scene, ParseLightArray(it));
				}
				else
				{
					fieldInfo.SetValue(scene, ParseValue(fieldInfo.FieldType, it));
				}
			}

			return scene;
		}

		public static Light[] ParseLightArray(NodeIterator it)
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			NodeIterator itItem = it.GetChildren();

			Light[] lights = new Light[length];

			for (int i = 0; i < length; i++)
			{
				for (; !itItem.IsEnd(); itItem.Next())
				{
					lights[i] = ParseLight(itItem);
				}
			}

			return lights;
		}

		public static Light ParseLight(NodeIterator it)
		{
			if (it.Name() != "light")
			{
				return null;
			}

			Light light = new Light();

			NodeIterator itItem = it.GetChildren();

			for (; !itItem.IsEnd(); itItem.Next())
			{
				System.Reflection.FieldInfo fieldInfo = FindFieldInfo(light.GetType().GetFields(), itItem.Name());
				if (fieldInfo == null)
				{
					continue;
				}

				itItem.Next();
				fieldInfo.SetValue(light, ParseValue(fieldInfo.FieldType, itItem));
			}

			return light;
		}

		public static Material[] ParseMaterialArray(NodeIterator it)
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			NodeIterator itItem = it.GetChildren();

			Material[] materials = new Material[length];

			int i = 0;
			Material material = null;

			for (; !itItem.IsEnd(); itItem.Next())
			{
				string name = itItem.Name();
				if (name[0] == '"' && name[name.Length - 1] == '"')
				{
					if (material != null)
					{
						materials[i] = material;
						i++;
					}

					material = new Material();
					material.Name = name.Substring(1, name.Length - 2);
					itItem.Next();
				}

				System.Reflection.FieldInfo fieldInfo = FindFieldInfo(material.GetType().GetFields(), itItem.Name());
				if (fieldInfo == null)
				{
					continue;
				}

				fieldInfo.SetValue(material, ParseValue(fieldInfo.FieldType, itItem.GetChildren()));
			}

			materials[i] = material;

			return materials;
		}

		public static Object ParseObject(NodeIterator it)
		{
			string name = it.Name();
			Object obj = new Object();

			if (name[0] != '"' || name[name.Length - 1] != '"')
			{
				return obj;
			}

			obj.Name = name.Substring(1,name.Length-2);
			NodeIterator itItem = it.GetChildren();

			for (; !itItem.IsEnd(); itItem.Next())
			{
				System.Reflection.FieldInfo fieldInfo = FindFieldInfo(obj.GetType().GetFields(), itItem.Name());
				if (fieldInfo == null)
				{
					continue;
				}

				if (fieldInfo.FieldType == typeof(Float3[]))
				{
					itItem.Next();
					fieldInfo.SetValue(obj, ParseVertexArray(itItem));
				}
				else if (fieldInfo.FieldType == typeof(Face[]))
				{
					itItem.Next();
					fieldInfo.SetValue(obj, ParseFaceArray(itItem));
				}
				else
				{
					itItem.Next();
					fieldInfo.SetValue(obj, ParseValue(fieldInfo.FieldType, itItem));

				}
			}

			return obj;
		}

		public static Float3[] ParseVertexArray(NodeIterator it)
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			return (Float3[])ParseArrayValues(typeof(Float3[]), it.GetChildren(), length);
		}

		public static Face[] ParseFaceArray(NodeIterator it)
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			NodeIterator itItem = it.GetChildren();

			Face[] faces = new Face[length];

			int i = 0;
			Face face = null;

			for (; !itItem.IsEnd(); itItem.Next())
			{
				NodeIterator itChild = itItem.GetChildren();
				if (itChild.Nodes.Count <= 0)
				{
					if (face != null)
					{
						faces[i] = face;
						i++;
					}

					int nbVertices = 0;
					int.TryParse(itItem.Name(), out nbVertices);
					face = new Face(nbVertices);
					continue;
				}
				else
				{
					System.Reflection.FieldInfo fieldInfo = FindFieldInfo(face.GetType().GetFields(), itItem.Name());
					if (fieldInfo == null)
					{
						continue;
					}
					else
					{
						if (fieldInfo.FieldType.IsArray)
						{
							fieldInfo.SetValue(face, ParseArrayValues(fieldInfo.FieldType, itItem.GetChildren(), face.Vertices));
						}
						else
						{
							fieldInfo.SetValue(face, ParseValue(fieldInfo.FieldType, itItem.GetChildren()));
						}
					}
				}
			}

			if (0 < length)
			{
				faces[i] = face;
			}

			return faces;
		}

		public static object ParseValue(Type targetType, NodeIterator it)
		{
			if (targetType == typeof(string))
			{
				string name = it.Name();
				if (name[0] == '"' && name[name.Length - 1] == '"')
				{
					return name.Substring(1, name.Length - 2);
				}
			}
			else if (targetType == typeof(int))
			{
				int value = 0;
				int.TryParse(it.Name(), out value);
				return value;
			}
			else if (targetType == typeof(float))
			{
				float value = 0.0f;
				float.TryParse(it.Name(), out value);
				return value;
			}
			else if (targetType == typeof(Float2))
			{
				Float2 value = new Float2();
				float.TryParse(it.Name(), out value.X);
				it.Next();
				float.TryParse(it.Name(), out value.Y);
				return value;
			}
			else if (targetType == typeof(Float3))
			{
				Float3 value = new Float3();
				float.TryParse(it.Name(), out value.X);
				it.Next();
				float.TryParse(it.Name(), out value.Y);
				it.Next();
				float.TryParse(it.Name(), out value.Z);
				return value;
			}
			else if (targetType == typeof(Float4))
			{
				Float4 value = new Float4();
				float.TryParse(it.Name(), out value.X);
				it.Next();
				float.TryParse(it.Name(), out value.Y);
				it.Next();
				float.TryParse(it.Name(), out value.Z);
				it.Next();
				float.TryParse(it.Name(), out value.W);
				return value;
			}

			return null;
		}

		public static object ParseArrayValues(Type targetType, NodeIterator it, int count)
		{
			if (targetType == typeof(int[]))
			{
				int[] values = new int[count];
				for (int i = 0; i < count; i++)
				{
					int value = 0;
					int.TryParse(it.Name(), out value);
					it.Next();
					values[i] = value;
				}
				return values;
			}
			else if (targetType == typeof(Float2[]))
			{
				Float2[] values = new Float2[count];
				for (int i = 0; i < count; i++)
				{
					Float2 value = new Float2();
					float.TryParse(it.Name(), out value.X);
					it.Next();
					float.TryParse(it.Name(), out value.Y);
					it.Next();
					values[i] = value;
				}
				return values;
			}
			else if (targetType == typeof(Float3[]))
			{
				Float3[] values = new Float3[count];
				for (int i = 0; i < count; i++)
				{
					Float3 value = new Float3();
					float.TryParse(it.Name(), out value.X);
					it.Next();
					float.TryParse(it.Name(), out value.Y);
					it.Next();
					float.TryParse(it.Name(), out value.Z);
					it.Next();
					values[i] = value;
				}
				return values;
			}

			return null;
		}

		/*
		public static void ParseStructArray( out Array array, MqoAttribute attribute, Type itemType, NodeIterator it )
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			NodeIterator itItem = it.GetChildren();

			array = Array.CreateInstance(itemType, length);

			for(int i = 0 ; i < length ; i++)
			{
				if(attribute.StructArrayItemName.Length > 0 || attribute.StructArrayItemName == itItem.Name())
				{
					object item = itemType.InvokeMember("VALUE", System.Reflection.BindingFlags.CreateInstance,null,null,null);
//					Parse(item, itItem.GetChildren());
					array.SetValue(item,i);
				}
				itItem.Next();
			}
		}



		public static void ParseNamelessLinearStructArray(out Array array, MqoAttribute attribute, Type itemType, NodeIterator it)
		{
			int length = 0;
			int.TryParse(it.Name(), out length);
			NodeIterator itItem = it.GetChildren();

			array = Array.CreateInstance(itemType, length);

			int i = 0;
			object item = null;// itemType.InvokeMember("VALUE", System.Reflection.BindingFlags.CreateInstance, null, null, null);

			for (; !itItem.IsEnd(); itItem.Next())
			{
				string name = itItem.Name();
				if (name[0] == '"' && name[name.Length - 1] == '"')
				{
					if (item != null)
					{
						array.SetValue(item, i);
						i++;
					}

					item = itemType.InvokeMember("VALUE", System.Reflection.BindingFlags.CreateInstance, null, null, null);
				}

				if(item == null)
				{
					continue;
				}
	
				var fields = item.GetType().GetFields();
				foreach(var field in fields)
				{
					MqoAttribute[] attributes = (MqoAttribute[])field.GetCustomAttributes(typeof(MqoAttribute), false);

					if (attributes == null)
					{
						continue;
					}

	//				ParseField(item, field, attributes[0], itItem);
				}
			}

			array.SetValue(item, i);
		}
		 */
	}

}
