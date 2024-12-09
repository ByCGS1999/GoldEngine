#pragma once
#include "Includes.h"

#include "imgui/imgui.h"
#include "CastToNative.h"

#include <vector>
#include "Object/Transform.h"
#include "DataPack.h"
#include "DataPacks.h"

namespace Engine::Internal
{
	/// <summary>
	/// Binding to the raylib abstraction layer
	/// </summary>
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class DataManager abstract
	{
	public:
		inline static GLWrapper::Shader^% HL_GetShader(unsigned int shaderId)
		{
			GLWrapper::Shader^ shader = gcnew GLWrapper::Shader();
			Shader& shaderRef = DataPacks::singleton().GetShader(shaderId);

			shader->Id = shaderRef.id;
			shader->Locs = shaderRef.locs;

			return shader;
		}

		// LOADING \\
		
		// Load a shader into the datapacks
		inline static void HL_LoadShader(unsigned int shaderId, System::String^ vertexShaderFile, System::String^ fragmentShaderFile)
		{
			Engine::Assets::Management::DataPack::singleton()->AddShader(shaderId, vertexShaderFile, fragmentShaderFile);
		}

		// Load a model into the datapacks
		inline static void HL_LoadModel(unsigned int modelId, System::String^ fileName)
		{
			Engine::Assets::Management::DataPack::singleton()->AddModel(modelId, fileName);
		}

		// Load a texture into the datapacks
		inline static void HL_LoadTexture2D(unsigned int textureId, System::String^ fileName)
		{
			Engine::Assets::Management::DataPack::singleton()->AddTextures2D(textureId, fileName);
		}

		// FREEING \\

		inline static void HL_FreeModels()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeModels();
		}

		inline static void HL_FreeShaders()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeShaders();
		}

		inline static void HL_FreeAll()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeAll();
		}

		inline static void HL_FreeShader(unsigned int shaderId)
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeShader(shaderId);
		}

	};
}