using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

#include "Macros.h"
#include "Includes.h"
#include "CastToNative.h"
#include "CypherLib.h"
#include "DataPacks.h"
#include "DataPack.h"

DataPack::DataPack()
{
	shaders = gcnew Dictionary<unsigned int, String^>();
	models = gcnew Dictionary<unsigned int, String^>();
	materials = gcnew Dictionary<unsigned int, unsigned int>();
	textures2d = gcnew Dictionary<unsigned int, String^>();
}

bool DataPack::AssetExists(System::String^ fN)
{
	return File::Exists("Data/" + fN + ".asset");
}

void DataPack::WriteToFile(System::String ^fileName, unsigned int password)
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

Shader DataPack::AddShader(unsigned int id, const char* vertexShader, const char* fragmentShader)
{
	shaders->Add(id, gcnew String(vertexShader) + ":^:" + gcnew String(fragmentShader));
	Shader s = LoadShader(vertexShader, fragmentShader);
	DataPacks::singleton().AddShader(id, s);
	return s;
}

Model DataPack::AddModel(unsigned int id, const char* path)
{
	Model m = LoadModel(path);
	DataPacks::singleton().AddModel(id, m);
	models->Add(id, gcnew String(path));
	return m;
}

Texture2D DataPack::AddTextures2D(unsigned int id, const char* path)
{
	Texture2D tex = LoadTexture(path);
	textures2d->Add(id, gcnew String(path));
	DataPacks::singleton().AddTexture2D(id, tex);
	return tex;
}

Material DataPack::AddMaterials(unsigned int id, unsigned int shaderId)
{
	Material m;
	m.shader = DataPacks::singleton().GetShader(shaderId);
	materials->Add(id, shaderId);
	DataPacks::singleton().AddMaterial(id, m);
	return m;
}

void DataPack::ParseContentData()
{
	for each (unsigned int shader_id in shaders->Keys)
	{
		System::String^ filePath = shaders[shader_id];
		auto splitted = filePath->Replace(":^:", ":")->Split(':');
		
		auto vs = splitted[0];
		auto fs = splitted[1];

		Shader s = LoadShader(CastToNative(vs), CastToNative(fs));
		DataPacks::singleton().AddShader(shader_id, s);
	}

	for each (unsigned int modelId in models->Keys)
	{
		System::String^ filePath = models[modelId];

		Model s = LoadModel(CastToNative(filePath));
		DataPacks::singleton().AddModel(modelId, s);
	}

	for each (unsigned int textureId in textures2d->Keys)
	{
		System::String^ filePath = textures2d[textureId];

		Texture2D s = LoadTexture(CastToNative(filePath));
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

void DataPack::CloneDataPack(DataPack^ newPack)
{
	shaders = gcnew Dictionary<unsigned int, String^>(newPack->shaders);
	models = gcnew Dictionary<unsigned int, String^>(newPack->models);
	textures2d = gcnew Dictionary<unsigned int, String^>(newPack->textures2d);
	materials = gcnew Dictionary<unsigned int, unsigned int>(newPack->materials);

	ParseContentData();
}


void DataPack::ReadFromFile(System::String^ fileName, unsigned int password)
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
		TraceLog(LOG_FATAL, "[CORE]: I/O OPERATION FAILED, FILE NOT FOUND");
	}
}