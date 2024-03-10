#pragma once

using namespace System;

/// <summary>
/// Allows for modifying and loading shaders at runtime.
/// THIS CLASS IS NOT PERSISTANT.
/// </summary>

public ref class ShaderManager
{
public:
	// load a shader into datapacks
	void LoadShaderFromFile(unsigned int shaderId, String^ vsFp, String^ fsFp)
	{
		Engine::Assets::Storage::DataPacks::singleton().AddShader(shaderId, LoadShader(CastStringToNative(vsFp).c_str(), CastStringToNative(fsFp).c_str()));
	}

	// mainly used for pushing modified shader code into the datapacks
	void LoadShaderFromMemory(unsigned int shaderId, String^ vertexShader, String^ fragmentShader)
	{
		Engine::Assets::Storage::DataPacks::singleton().AddShader(shaderId, ::LoadShaderFromMemory(CastStringToNative(vertexShader).c_str(), CastStringToNative(fragmentShader).c_str()));
	}

	// load a shader into datapacks
	void UpdateShaderFromFile(unsigned int shaderId, String^ vsFp, String^ fsFp)
	{
		Engine::Assets::Storage::DataPacks::singleton().SetShader(shaderId, LoadShader(CastStringToNative(vsFp).c_str(), CastStringToNative(fsFp).c_str()));
	}

	// mainly used for pushing modified shader code into the datapacks
	void UpdateShaderFromMemory(unsigned int shaderId, String^ vertexShader, String^ fragmentShader)
	{
		Engine::Assets::Storage::DataPacks::singleton().SetShader(shaderId, ::LoadShaderFromMemory(CastStringToNative(vertexShader).c_str(), CastStringToNative(fragmentShader).c_str()));
	}
};