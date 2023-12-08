#pragma once
#include <vector>
#include <algorithm>
#include "Includes.h"
#include "Transform.h"
#include "ShaderPack.h"
#include "ModelPack.h"
#include "CameraPack.h"
#include "MaterialPack.h"
#include "Texture2DPack.h"

public class NativeDataPack
{
public:
	std::vector<ShaderPack> shaders;
	std::vector<ModelPack> models;
	std::vector<CameraPack> cameras;
	std::vector<MaterialPack> materials;
	std::vector<Texture2DPack> textures2d;
	NativeDataPack() {
		shaders = std::vector<ShaderPack>();
		models = std::vector<ModelPack>();
		cameras = std::vector<CameraPack>();
		materials = std::vector<MaterialPack>();
		textures2d = std::vector<Texture2DPack>();
	}
};

public class DataPacks
{
private:
	NativeDataPack* nativePacks;
	// singleton

public:
	DataPacks();
	void FreeShaders();
	void FreeMaterials();
	void FreeModels();
	void FreeCameras();
	void FreeTextures2D();
	void FreeAll();
	ModelPack GetModelPack(unsigned int modelPackId);
	ShaderPack GetShaderPack(unsigned int shaderPackId);
	Camera3D GetCamera3D(unsigned int cameraId);
	Camera2D GetCamera2D(unsigned int cameraId);
	Texture2D GetTexture2D(unsigned int textureId);
	Material GetMaterial(unsigned int materialId);
	Model GetModel(unsigned int modelPackId);
	Shader GetShader(unsigned int shaderPackId);
	void AddShader(unsigned int shaderId, Shader shader);
	void AddModel(unsigned int modelId, Model model);
	void AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type);
	void AddCamera(unsigned int cameraId, Camera3D camera, Engine::Internal::Components::CameraType type);
	void AddMaterial(unsigned int materialId, Material material);
	void AddTexture2D(unsigned int textureId, Texture2D texture);
	static DataPacks singleton();
	NativeDataPack* GetNativeDataPack();
};