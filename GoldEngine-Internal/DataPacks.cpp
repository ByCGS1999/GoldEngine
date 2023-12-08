#include "DataPack.h"
#include "DataPacks.h"
#include <vector>
#include <algorithm>
#include "Includes.h"
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

DataPacks* singleton_Ref;

DataPacks::DataPacks()
{
	nativePacks = new NativeDataPack();
	singleton_Ref = this;
}

DataPacks DataPacks::singleton()
{
	return *singleton_Ref;
}

void DataPacks::FreeAll()
{
	FreeModels();
	FreeTextures2D();
	FreeMaterials();
	FreeShaders();
	FreeCameras();
}

void DataPacks::FreeTextures2D()
{
	nativePacks->textures2d.clear();
}


void DataPacks::FreeCameras()
{
	nativePacks->cameras.clear();
}

void DataPacks::FreeMaterials()
{
	nativePacks->materials.clear();
}

void DataPacks::FreeModels()
{
	nativePacks->models.clear();
}

void DataPacks::FreeShaders()
{
	nativePacks->shaders.clear();
}

void DataPacks::AddShader(unsigned int shaderId, Shader shader)
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

void DataPacks::AddModel(unsigned int modelId, Model modelRef)
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


Model DataPacks::GetModel(unsigned int modelId)
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

Shader DataPacks::GetShader(unsigned int shaderId)
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

Material DataPacks::GetMaterial(unsigned int materialId)
{
	Material material;

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

Texture2D DataPacks::GetTexture2D(unsigned int textureId)
{
	Texture2D texture;

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

ShaderPack DataPacks::GetShaderPack(unsigned int shaderId)
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

Camera2D DataPacks::GetCamera2D(unsigned int cameraId)
{
	Camera2D camera;

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
Camera3D DataPacks::GetCamera3D(unsigned int cameraId)
{
	Camera3D camera;

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

void DataPacks::AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type)
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

void DataPacks::AddCamera(unsigned int cameraId, Camera3D camera, Engine::Internal::Components::CameraType type)
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

void DataPacks::AddMaterial(unsigned int materialID, Material material)
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

void DataPacks::AddTexture2D(unsigned int textureId, Texture2D texture)
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

NativeDataPack* DataPacks::GetNativeDataPack()
{
	return nativePacks;
}