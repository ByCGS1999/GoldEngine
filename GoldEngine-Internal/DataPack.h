#pragma once
#include "Includes.h"
#include "DataPacks.h"

using namespace System;
using namespace System::Collections::Generic;

public ref class DataPack
{
private:
	void ParseContentData();
	bool AssetExists(System::String^ fN);
	void CloneDataPack(DataPack^ pack);

public:
	System::Collections::Generic::Dictionary<unsigned int, String^>^ shaders;
	System::Collections::Generic::Dictionary<unsigned int, String^>^ models;
	System::Collections::Generic::Dictionary<unsigned int, unsigned int>^ materials;
	System::Collections::Generic::Dictionary<unsigned int, String^>^ textures2d;

public:
	DataPack();
	Shader AddShader(unsigned int id, const char* vertexShader, const char* fragmentShader);
	Model AddModel(unsigned int id, const char* path);
	Material AddMaterials(unsigned int id, unsigned int shaderId);
	Texture2D AddTextures2D(unsigned int id, const char* path);
	void WriteToFile(System::String^ fileName, unsigned int password);
	void ReadFromFile(System::String^ fileName, unsigned int password);
};

