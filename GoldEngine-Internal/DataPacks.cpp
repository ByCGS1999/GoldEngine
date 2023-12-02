#include "DataPacks.h"

/*
	DATAPACKS
	Used for storing game contents in memory.

	
*/


std::vector<ShaderPack*> shaders;
std::vector<ModelPack*> models;
std::vector<CameraPack*> cameras;
std::vector<MaterialPack*> materials;
std::vector<Texture2DPack*> textures2d;

void DataPacks::BeginDataPacks()
{
	shaders = std::vector<ShaderPack*>();
	models = std::vector<ModelPack*>();
	cameras = std::vector<CameraPack*>();
	materials = std::vector<MaterialPack*>();
	textures2d = std::vector<Texture2DPack*>();
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
	for each (Texture2DPack * pack in textures2d)
	{
		free(pack);
	}

	textures2d.clear();
}


void DataPacks::FreeCameras()
{
	for each (CameraPack * pack in cameras)
	{
		free(pack);
	}

	cameras.clear();
}

void DataPacks::FreeMaterials()
{
	for each (MaterialPack * pack in materials)
	{
		free(pack);
	}

	materials.clear();
}

void DataPacks::FreeModels()
{
	for each (ModelPack* pack in models)
	{
		free(pack);
	}

	models.clear();
}

void DataPacks::FreeShaders()
{
	for each (ShaderPack * pack in shaders)
	{
		free(pack);
	}

	shaders.clear();
}

void DataPacks::AddShader(unsigned int shaderId, Shader shader)
{
	bool hasShader = false;

	for each (ShaderPack * s in shaders)
	{
		if (s->shaderId == shaderId)
		{
			s->shaderReference = shader; // overwrite shader reference
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
		shaders.push_back(new ShaderPack(shaderId, shader));
	}
}

void DataPacks::AddModel(unsigned int modelId, Model modelRef)
{
	bool hasModel = false;

	for each (ModelPack * model in models)
	{
		if (model->ModelId == modelId)
		{
			model->ModelReference = modelRef;
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
		models.push_back(new ModelPack(modelId, modelRef));
	}
}


Model DataPacks::GetModel(unsigned int modelId)
{
	Model retn;
	bool hasModel = false;

	for each (ModelPack * mP in models)
	{
		if (mP->ModelId == modelId)
		{
			retn = mP->ModelReference;
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

	for each (ShaderPack * sP in shaders)
	{
		if (sP->shaderId == shaderId)
		{
			shader = sP->shaderReference;
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

	for each (MaterialPack * sP in materials)
	{
		if (sP->MaterialId == materialId)
		{
			material = sP->MaterialReference;
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

	for each (Texture2DPack * sP in textures2d)
	{
		if (sP->textureId == textureId)
		{
			texture = sP->textureReference;
			hasTexture2D = true;
			break;
		}
	}

	return texture;
}

ShaderPack DataPacks::GetShaderPack(unsigned int shaderId)
{
	ShaderPack* shader = nullptr;

	for each (ShaderPack * sP in shaders)
	{
		if (sP->shaderId == shaderId)
		{
			shader = sP;
			break;
		}
	}

	return *shader;
}

Camera2D DataPacks::GetCamera2D(unsigned int cameraId)
{
	Camera2D camera;

	for each (CameraPack * cP in cameras)
	{
		if ((cP->cameraId == cameraId) && (cP->cameraType == Engine::Internal::Components::CameraType::C2D))
		{
			camera = cP->camera2D;
			break;
		}
	}

	return camera;
}
Camera3D DataPacks::GetCamera3D(unsigned int cameraId)
{
	Camera3D camera;

	for each (CameraPack * cP in cameras)
	{
		if ((cP->cameraId == cameraId) && (cP->cameraType == Engine::Internal::Components::CameraType::C3D))
		{
			camera = cP->camera3D;
			break;
		}
	}

	return camera;
}

void DataPacks::AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type)
{
	bool hasCamera = false;

	for each (CameraPack * cPack in cameras)
	{
		if (cPack->cameraId == cameraId && cPack->cameraType == type)
		{
			cPack->camera2D = camera;
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
		cameras.push_back(new CameraPack(cameraId, type, &camera));
	}
}

void DataPacks::AddCamera(unsigned int cameraId, Camera3D camera, Engine::Internal::Components::CameraType type)
{
	bool hasCamera = false;

	for each (CameraPack * cPack in cameras)
	{
		if (cPack->cameraId == cameraId && cPack->cameraType == type)
		{
			cPack->camera3D = camera;
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
		cameras.push_back(new CameraPack(cameraId, type, &camera));
	}
}

void DataPacks::AddMaterial(unsigned int materialID, Material material)
{
	bool hasMaterial = false;

	for each (MaterialPack * cPack in materials)
	{
		if (cPack->MaterialId == materialID)
		{
			cPack->MaterialReference = material;
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
		materials.push_back(new MaterialPack(materialID, material));
	}
}

void DataPacks::AddTexture2D(unsigned int textureId, Texture2D texture)
{
	bool hasTexture2D = false;

	for each (Texture2DPack * cPack in textures2d)
	{
		if (cPack->textureId == textureId)
		{
			cPack->textureReference = texture;
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
		textures2d.push_back(new Texture2DPack(textureId, texture));
	}
}