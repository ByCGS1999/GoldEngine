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

public class DataPacks
{
public:
	static void BeginDataPacks();
	static void FreeShaders();
	static void FreeMaterials();
	static void FreeModels();
	static void FreeCameras();
	static void FreeTextures2D();
	static void FreeAll();
	static ModelPack GetModelPack(unsigned int modelPackId);
	static ShaderPack GetShaderPack(unsigned int shaderPackId);
	static Camera3D GetCamera3D(unsigned int cameraId);
	static Camera2D GetCamera2D(unsigned int cameraId);
	static Texture2D GetTexture2D(unsigned int textureId);
	static Material GetMaterial(unsigned int materialId);
	static Model GetModel(unsigned int modelPackId);
	static Shader GetShader(unsigned int shaderPackId);
	static void AddShader(unsigned int shaderId, Shader shader);
	static void AddModel(unsigned int modelId, Model model);
	static void AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type);
	static void AddCamera(unsigned int cameraId, Camera3D camera, Engine::Internal::Components::CameraType type);
	static void AddMaterial(unsigned int materialId, Material material);
	static void AddTexture2D(unsigned int textureId, Texture2D texture);
};