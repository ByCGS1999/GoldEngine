#pragma once
#include <vector>
#include <algorithm>
#include "Includes.h"
#include "GlIncludes.h"
#include "Transform.h"
#include "ShaderPack.h"
#include "ModelPack.h"
#include "CameraPack.h"
#include "MaterialPack.h"
#include "Texture2DPack.h"

/*
	DATAPACKS
	Used for storing game contents in memory.
*/

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
	static DataPacks* singleton_Ref;

public:
	DataPacks()
	{
		nativePacks = new NativeDataPack();
		singleton_Ref = this;
	}

	void FreeShaders()
	{
		nativePacks->shaders.clear();
	}

	void FreeMaterials()
	{
		nativePacks->materials.clear();
	}

	void FreeModels()
	{
		nativePacks->models.clear();
	}

	void FreeCameras()
	{
		nativePacks->cameras.clear();
	}

	void FreeTextures2D()
	{
		nativePacks->textures2d.clear();
	}

	void FreeAll()
	{
		FreeModels();
		FreeTextures2D();
		FreeMaterials();
		FreeShaders();
		FreeCameras();
	}


	ModelPack GetModelPack(unsigned int modelPackId)
	{
		ModelPack shader = ModelPack(-1, { 0 });

		for each (ModelPack sP in nativePacks->models)
		{
			if (sP.ModelId == modelPackId)
			{
				shader = sP;
				break;
			}
		}

		return shader;
	}


	ShaderPack GetShaderPack(unsigned int shaderId)
	{
		ShaderPack shader = ShaderPack(-1, { 0, {0} });

		for each (ShaderPack  sP in nativePacks->shaders)
		{
			if (sP.shaderId == shaderId)
			{
				shader = sP;
				break;
			}
		}

		return shader;
	}

	Camera3D GetCamera3D(unsigned int cameraId)
	{
		Camera3D camera = { };

		for each (CameraPack  cP in nativePacks->cameras)
		{
			if ((cP.cameraId == cameraId) && (cP.cameraType == Engine::Internal::Components::CameraType::C3D))
			{
				camera = cP.camera3D;
				break;
			}
		}

		return camera;
	}

	Camera2D GetCamera2D(unsigned int cameraId)
	{
		Camera2D camera = { };

		for each (CameraPack  cP in nativePacks->cameras)
		{
			if ((cP.cameraId == cameraId) && (cP.cameraType == Engine::Internal::Components::CameraType::C2D))
			{
				camera = cP.camera2D;
				break;
			}
		}

		return camera;
	}

	Texture2D GetTexture2D(unsigned int textureId)
	{
		Texture2D texture = { };

		bool hasTexture2D = false;

		for each (Texture2DPack  sP in nativePacks->textures2d)
		{
			if (sP.textureId == textureId)
			{
				texture = sP.textureReference;
				hasTexture2D = true;
				break;
			}
		}

		return texture;
	}

	Material GetMaterial(unsigned int materialId)
	{
		Material material = { };

		bool hasMaterial = false;

		for each (MaterialPack  sP in nativePacks->materials)
		{
			if (sP.MaterialId == materialId)
			{
				material = sP.MaterialReference;
				hasMaterial = true;
				break;
			}
		}

		if (!hasMaterial)
		{
			material = LoadMaterialDefault();
		}

		return material;
	}


	Model GetModel(unsigned int modelId)
	{
		Model retn;
		bool hasModel = false;

		for each (ModelPack  mP in nativePacks->models)
		{
			if (mP.ModelId == modelId)
			{
				retn = mP.ModelReference;
				hasModel = true;
				break;
			}
		}

		if (!hasModel)
		{
			retn = LoadModel("Data/Engine/Models/Error.obj");
			Material defaultMaterial = LoadMaterialDefault();
			retn.materials = &defaultMaterial;
		}

		return retn;
	}

	Shader GetShader(unsigned int shaderId)
	{
		Shader shader;

		bool hasShader = false;

		for each (ShaderPack  sP in nativePacks->shaders)
		{
			if (sP.shaderId == shaderId)
			{
				shader = sP.shaderReference;
				hasShader = true;
				break;
			}
		}

		if (!hasShader)
		{
			shader = LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");
		}

		return shader;
	}

	void AddShader(unsigned int shaderId, Shader shader)
	{
		bool hasShader = false;

		for each (ShaderPack  s in nativePacks->shaders)
		{
			if (s.shaderId == shaderId)
			{
				s.shaderReference = shader; // overwrite shader reference
				hasShader = true;
				break;
			}
			else
			{
				hasShader = false;
			}
		}

		if (!hasShader)
		{
			nativePacks->shaders.push_back(ShaderPack(shaderId, shader));
		}
	}
	void AddModel(unsigned int modelId, Model modelRef)
	{
		bool hasModel = false;

		for each (ModelPack  model in nativePacks->models)
		{
			if (model.ModelId == modelId)
			{
				model.ModelReference = modelRef;
				hasModel = true;
				break;
			}
			else
			{
				hasModel = false;
			}
		}

		if (!hasModel)
		{
			nativePacks->models.push_back(ModelPack(modelId, modelRef));
		}
	}
	void AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type)
	{
		bool hasCamera = false;

		for each (CameraPack  cPack in nativePacks->cameras)
		{
			if (cPack.cameraId == cameraId && cPack.cameraType == type)
			{
				cPack.camera2D = camera;
				hasCamera = true;
				break;
			}
			else
			{
				hasCamera = false;
			}
		}

		if (!hasCamera)
		{
			nativePacks->cameras.push_back(CameraPack(cameraId, type, &camera));
		}
	}

	void AddCamera(unsigned int cameraId, Camera3D camera, Engine::Internal::Components::CameraType type)
	{
		bool hasCamera = false;

		for each (CameraPack  cPack in nativePacks->cameras)
		{
			if (cPack.cameraId == cameraId && cPack.cameraType == type)
			{
				cPack.camera3D = camera;
				hasCamera = true;
				break;
			}
			else
			{
				hasCamera = false;
			}
		}

		if (!hasCamera)
		{
			nativePacks->cameras.push_back(CameraPack(cameraId, type, &camera));
		}
	}


	void AddMaterial(unsigned int materialID, Material material)
	{
		bool hasMaterial = false;

		for each (MaterialPack  cPack in nativePacks->materials)
		{
			if (cPack.MaterialId == materialID)
			{
				cPack.MaterialReference = material;
				hasMaterial = true;
				break;
			}
			else
			{
				hasMaterial = false;
			}
		}

		if (!hasMaterial)
		{
			nativePacks->materials.push_back(MaterialPack(materialID, material));
		}
	}


	void AddTexture2D(unsigned int textureId, Texture2D texture)
	{
		bool hasTexture2D = false;

		for each (Texture2DPack  cPack in nativePacks->textures2d)
		{
			if (cPack.textureId == textureId)
			{
				cPack.textureReference = texture;
				hasTexture2D = true;
				break;
			}
			else
			{
				hasTexture2D = false;
			}
		}

		if (!hasTexture2D)
		{
			nativePacks->textures2d.push_back(Texture2DPack(textureId, texture));
		}
	}

	void SetShader(unsigned int shaderId, Shader value)
	{
		nativePacks->shaders[shaderId].shaderReference = value;
	}

	static DataPacks singleton()
	{
		return *singleton_Ref;
	}

	NativeDataPack* GetNativeDataPack()
	{
		return nativePacks;
	}
};

DataPacks* DataPacks::singleton_Ref;