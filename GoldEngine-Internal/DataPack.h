#pragma once
#include "Includes.h"
#include "CastToNative.h"
#include "CypherLib.h"
#include "DataPacks.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

namespace Engine::Assets::Management
{
	typedef enum assetType { _Model, _Shader, _Texture2D, _Material };

	public ref class DataPack
	{
	private:
		String^ fileTarget;
		static DataPack^ singletonRef = nullptr;

	private:
		void ParseContentData()
		{
			for each (unsigned int shader_id in shaders->Keys)
			{
				System::String^ filePath = shaders[shader_id];
				auto splitted = filePath->Replace(":^:", ":")->Split(':');

				auto vs = splitted[0];
				auto fs = splitted[1];

				Shader s = LoadShader(CastStringToNative(vs).c_str(), CastStringToNative(fs).c_str());
				DataPacks::singleton().AddShader(shader_id, s);
			}

			for each (unsigned int modelId in models->Keys)
			{
				System::String^ filePath = models[modelId];

				Model s = LoadModel(CastStringToNative(filePath).c_str());
				DataPacks::singleton().AddModel(modelId, s);
			}

			for each (unsigned int textureId in textures2d->Keys)
			{
				System::String^ filePath = textures2d[textureId];

				Texture2D s = LoadTexture(CastStringToNative(filePath).c_str());
				DataPacks::singleton().AddTexture2D(textureId, s);
			}

			for each (unsigned int material_id in materials->Keys)
			{
				unsigned int shaderId = materials[material_id];

				Material m;
				m.shader = DataPacks::singleton().GetShader(shaderId);
				DataPacks::singleton().AddMaterial(material_id, m);
			}
		}

		bool AssetExists(System::String^ fN)
		{
			return File::Exists("Data/" + fN + ".asset");
		}

		void CloneDataPack(DataPack^ newPack)
		{
			shaders = gcnew Dictionary<unsigned int, String^>(newPack->shaders);
			models = gcnew Dictionary<unsigned int, String^>(newPack->models);
			textures2d = gcnew Dictionary<unsigned int, String^>(newPack->textures2d);
			materials = gcnew Dictionary<unsigned int, unsigned int>(newPack->materials);

			ParseContentData();
		}

		void Create()
		{
			shaders = gcnew Dictionary<unsigned int, String^>();
			models = gcnew Dictionary<unsigned int, String^>();
			materials = gcnew Dictionary<unsigned int, unsigned int>();
			textures2d = gcnew Dictionary<unsigned int, String^>();
		}

	public:
		System::Collections::Generic::Dictionary<unsigned int, String^>^ shaders;
		System::Collections::Generic::Dictionary<unsigned int, String^>^ models;
		System::Collections::Generic::Dictionary<unsigned int, unsigned int>^ materials;
		System::Collections::Generic::Dictionary<unsigned int, String^>^ textures2d;

	public:
		DataPack(String^ fileName)
		{
			shaders = gcnew Dictionary<unsigned int, String^>();
			models = gcnew Dictionary<unsigned int, String^>();
			materials = gcnew Dictionary<unsigned int, unsigned int>();
			textures2d = gcnew Dictionary<unsigned int, String^>();
			singletonRef = this;
			fileTarget = fileName;
		}

		static void SetSingletonReference(DataPack^ reference)
		{
			singletonRef = reference;
		}

		Shader AddShader(unsigned int id, String^ vs, String^ fs)
		{
			if (!shaders->ContainsKey(id))
			{
				shaders->Add(id, vs + ":^:" + fs);

				std::string vertexShader = "";
				std::string fragmentShader = "";

				vertexShader = CastStringToNative(vs);
				fragmentShader = CastStringToNative(vs);

				Shader s = LoadShader(vertexShader.c_str(), fragmentShader.c_str());
				DataPacks::singleton().AddShader(id, s);

				return s;
			}
			else
			{
				return DataPacks::singleton().GetShader(id);
			}
		}

		Model AddModel(unsigned int id, const char* path)
		{
			if (!models->ContainsKey(id))
			{
				models->Add(id, gcnew String(path));
				Model m = LoadModel(path);

				DataPacks::singleton().AddModel(id, m);

				return m;
			}
			else
			{
				return DataPacks::singleton().GetModel(id);
			}
		}

		Material AddMaterials(unsigned int id, unsigned int shaderId)
		{
			Material m;
			m.shader = DataPacks::singleton().GetShader(shaderId);
			materials->Add(id, shaderId);
			DataPacks::singleton().AddMaterial(id, m);
			return m;
		}

		Texture2D AddTextures2D(unsigned int id, String^ tex)
		{
			if (!textures2d->ContainsKey(id))
			{
				textures2d->Add(id, tex);

				std::string text = "";

				text = CastStringToNative(tex);

				Texture2D tex = LoadTexture(text.c_str());
				DataPacks::singleton().AddTexture2D(id, tex);

				DataPacks::singleton().AddTexture2D(id, tex);

				return tex;
			}
			else
			{
				return DataPacks::singleton().GetTexture2D(id);
			}
		}

		bool hasAsset(assetType aTyp, unsigned int value)
		{
			switch (aTyp)
			{
			case _Shader:
				for each (auto T in shaders)
				{
					if (T.Key == value) 
					{
						return true;
					}
				}
				return false;
				break;
			case _Material:
				for each (auto T in materials)
				{
					if (T.Key == value)
					{
						return true;
					}
				}
				return false;
				break;
			case _Model:
				for each (auto T in models)
				{
					if (T.Key == value)
					{
						return true;
					}
				}
				return false;
				break;
			case _Texture2D:
				for each (auto T in textures2d)
				{
					if (T.Key == value)
					{
						return true;
					}
				}
				return false;
				break;
			}
		}

		std::tuple<bool, int> hasAsset(assetType aTyp, String^ value)
		{
			switch (aTyp)
			{
			case _Shader:
				for each (auto T in shaders)
				{
					if (T.Value == value)
					{
						return std::tuple<bool, int>(true, T.Key);
					}
				}
				return std::tuple<bool, int>(false, 0);
				break;
			case _Material:
				for each (auto T in materials)
				{
					if (T.Value == std::atoi(CastStringToNative(value).c_str()))
					{
						return std::tuple<bool, int>(true, T.Key);
					}
				}
				return std::tuple<bool, int>(false, 0);
				break;
			case _Model:
				for each (auto T in models)
				{
					if (T.Value == value)
					{
						return std::tuple<bool, int>(true, T.Key);
					}
				}
				return std::tuple<bool, int>(false, 0);
				break;
			case _Texture2D:
				for each (auto T in textures2d)
				{
					if (T.Value == value)
					{
						return std::tuple<bool, int>(true, T.Key);
					}
				}
				return std::tuple<bool, int>(false, 0);
				break;
			}
		}

		unsigned int GetAssetID(assetType type)
		{
			unsigned int assetId = 0;

			switch (type)
			{
			case _Material:
				for each (auto T in materials)
				{
					assetId = Math::Max(assetId, T.Key + 1);
				}
				break;
			case _Model:
				for each (auto T in models)
				{
					assetId = Math::Max(assetId, T.Key + 1);
				}
				break;
			case _Shader:
				for each (auto T in shaders)
				{
					assetId = Math::Max(assetId, T.Key + 1);
				}
				break;
			case _Texture2D:
				for each (auto T in textures2d)
				{
					assetId = Math::Max(assetId, T.Key + 1);
				}
				break;
			}

			return assetId;
		}

		String^ getFile()
		{
			return fileTarget;
		}

		String^ getFileContents()
		{
			return File::ReadAllText(getFile());
		}

		void WriteToFile(System::String^ fileName, unsigned int password)
		{
			if (AssetExists(fileName))
			{
				String^ serializedData = Newtonsoft::Json::JsonConvert::SerializeObject(this, Newtonsoft::Json::Formatting::Indented);
				String^ cipheredContents = CypherLib::EncryptFileContents(serializedData, password);

				//System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", System::Convert::ToBase64String(Encoding::UTF8->GetBytes(cipheredContents)));
				System::IO::File::WriteAllText("Data/" + fileName + ".asset", serializedData);
			}
			else
			{
				// create file and rerun save
				System::IO::File::Create("Data/" + fileName + ".asset")->Close();
				WriteToFile(fileName, password);
			}
		}

		void ReadFromFile(System::String^ fileName, unsigned int password)
		{
			if (AssetExists(fileName))
			{
				DataPack^ pack = Newtonsoft::Json::JsonConvert::DeserializeObject<DataPack^>(File::ReadAllText("Data/" + fileName + ".asset"));
				String^ serializedData = Newtonsoft::Json::JsonConvert::SerializeObject(this, Newtonsoft::Json::Formatting::Indented);
				String^ cipheredContents = CypherLib::EncryptFileContents(serializedData, password);

				CloneDataPack(pack);
				//System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", System::Convert::ToBase64String(Encoding::UTF8->GetBytes(cipheredContents)));
			}
			else
			{
				// create file and rerun save
				printf("[CORE]: I/O OPERATION FAILED, FILE %s NOT FOUND", CastStringToNative(fileName).c_str());
				System::IO::File::Create("Data/" + fileName + ".asset")->Close();
				WriteToFile(fileName, password);

				ReadFromFile(fileName, password);
			}
		}

		static DataPack^ singleton()
		{
			return singletonRef;
		}
	};

}