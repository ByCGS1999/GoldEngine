#pragma once
#include <vector>
#include <algorithm>
#include "Includes.h"
#include "GlIncludes.h"
#include "Object/Transform.h"
#include "Packs.h"
#include "CameraPack.h"
#include "MaterialPack.h"
#include "LoggingAPI.h"

/*
	DATAPACKS
	Used for storing game contents in memory.
*/

namespace Engine::Assets::Storage
{
	public class NativeDataPack
	{
	public:
		std::vector<Engine::Assets::Storage::Types::ShaderPack> shaders;
		std::vector<Engine::Assets::Storage::Types::ModelPack> models;
		std::vector<Engine::Assets::Storage::Types::CameraPack> cameras;
		std::vector<Engine::Assets::Storage::Types::MaterialPack> materials;
		std::vector<Engine::Assets::Storage::Types::Texture2DPack> textures2d;
		NativeDataPack() {
			shaders = std::vector<Engine::Assets::Storage::Types::ShaderPack>();
			models = std::vector<Engine::Assets::Storage::Types::ModelPack>();
			cameras = std::vector<Engine::Assets::Storage::Types::CameraPack>();
			materials = std::vector<Engine::Assets::Storage::Types::MaterialPack>();
			textures2d = std::vector<Engine::Assets::Storage::Types::Texture2DPack>();
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

		bool dataPackHasAssets()
		{
			int assetCount = 0;

			assetCount += nativePacks->shaders.size();
			assetCount += nativePacks->models.size();
			assetCount += nativePacks->textures2d.size();
			assetCount += nativePacks->materials.size();
			assetCount += nativePacks->cameras.size();

			return (assetCount >= 0);
		}

		void FreeShaders()
		{
			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				sP->freealloc();
			}

			nativePacks->shaders.clear();
		}

		void FreeShader(unsigned int shaderId)
		{
			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					sP->freealloc();
				}
			}
		}

		void FreeMaterials()
		{
			/*
			for (int x = 0; x < nativePacks->materials.size(); x++)
			{
				if (&nativePacks->materials[x].MaterialReference != nullptr)
				{
					UnloadMaterial(nativePacks->materials[x].MaterialReference);
				}
			}
			*/

			nativePacks->materials.clear();
		}

		void FreeModels()
		{
			for (int x = 0; x < nativePacks->models.size(); x++)
			{
				auto sP = &nativePacks->models[x];

				if (sP->hasValue())
				{
					sP->freealloc();
				}
			}

			nativePacks->models.clear();
		}

		void FreeCameras()
		{
			nativePacks->cameras.clear();
		}

		void FreeTextures2D()
		{
			for (int x = 0; x < nativePacks->textures2d.size(); x++)
			{
				auto tP = &nativePacks->textures2d[x];

				if (tP->hasValue())
				{
					tP->freealloc();
				}
			}

			nativePacks->textures2d.clear();
		}

		void FreeAll()
		{
			if (this->nativePacks == nullptr)
				return;

			printf("Freeing and unloading loaded assets\n");

			FreeModels();
			FreeTextures2D();
			FreeMaterials();
			FreeShaders();
			FreeCameras();
		}


		Engine::Assets::Storage::Types::ModelPack* GetModelPack(unsigned int modelPackId)
		{
			Engine::Assets::Storage::Types::ModelPack* shader = nullptr;

			for (int x = 0; x < nativePacks->models.size(); x++)
			{
				auto sP = &nativePacks->models[x];

				if (sP->getId()  == modelPackId)
				{
					shader = sP;
					break;
				}
			}

			return shader;
		}


		Engine::Assets::Storage::Types::ShaderPack* GetShaderPack(unsigned int shaderId)
		{
			Engine::Assets::Storage::Types::ShaderPack* shader = nullptr;

			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					shader = sP;
					break;
				}
			}

			return shader;
		}

		::Camera3D GetCamera3D(unsigned int cameraId)
		{
			::Camera3D camera = { };

			for each (Engine::Assets::Storage::Types::CameraPack  cP in nativePacks->cameras)
			{
				if ((cP.cameraId == cameraId) && (cP.cameraType == Engine::Internal::Components::CameraType::C3D))
				{
					camera = cP.camera3D;
					break;
				}
			}

			return camera;
		}

		::Camera2D GetCamera2D(unsigned int cameraId)
		{
			Camera2D camera = { };

			for each (Engine::Assets::Storage::Types::CameraPack  cP in nativePacks->cameras)
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
			for (int x = 0; x < nativePacks->textures2d.size(); x++)
			{
				auto mP = &nativePacks->textures2d[x];

				if (mP->getId() == textureId)
				{
					return mP->getResource();
				}
			}

			AddTexture2D(textureId, LoadTexture("./Data/Engine/Textures/NotFound.png"));

			return GetTexture2D(textureId);
		}

		Material GetMaterial(unsigned int materialId)
		{
			Material material = { };

			bool hasMaterial = false;

			for each (Engine::Assets::Storage::Types::MaterialPack  sP in nativePacks->materials)
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
			Model retn = { };
			bool hasModel = false;

			for (int x = 0; x < nativePacks->models.size(); x++)
			{
				auto mP = &nativePacks->models[x];

				if (mP->getId() == modelId)
				{
					retn = mP->getResource();
					hasModel = true;
					break;
				}
			}

			if (!hasModel)
			{
				retn = LoadModel("Data/Engine/Models/Error.obj");
			}

			return retn;
		}

		Shader& GetShader(unsigned int shaderId)
		{
			Shader shader;

			bool hasShader = false;

			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					return sP->getResource();
				}
			}

			if (!hasShader)
			{
				printError(String::Format("Shader {0} does not exist, falling back on default", shaderId));
				shader = LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");
				AddShader(shaderId, shader);
			}

			return shader;
		}

		Shader* GetShaderPtr(unsigned int shaderId)
		{
			Shader* shader;

			bool hasShader = false;

			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					shader = sP->getResourcePtr();
					hasShader = true;
					break;
				}
			}

			if (!hasShader)
			{
				printError(String::Format("Shader {0} does not exist, falling back on default", shaderId));
				shader = &LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");
				AddShader(shaderId, *shader);
			}

			return shader;
		}

		void AddShader(unsigned int shaderId, Shader& shader)
		{
			bool hasShader = false;

			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					sP->setResource(shader); // overwrite shader reference
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
				nativePacks->shaders.push_back(Engine::Assets::Storage::Types::ShaderPack(shaderId, shader));
			}
		}
		void AddModel(unsigned int modelId, Model modelRef)
		{
			bool hasModel = false;

			for (int x = 0; x < nativePacks->models.size(); x++)
			{
				auto sP = &nativePacks->models[x];

				if (sP->getId() == modelId)
				{
					sP->setResource(modelRef);
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
				nativePacks->models.push_back(Engine::Assets::Storage::Types::ModelPack(modelId, modelRef));
			}
		}
		void AddCamera(unsigned int cameraId, Camera2D camera, Engine::Internal::Components::CameraType type)
		{
			bool hasCamera = false;

			for each (Engine::Assets::Storage::Types::CameraPack  cPack in nativePacks->cameras)
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
				nativePacks->cameras.push_back(Engine::Assets::Storage::Types::CameraPack(cameraId, type, &camera));
			}
		}

		void AddCamera(unsigned int cameraId, ::Camera3D camera, Engine::Internal::Components::CameraType type)
		{
			bool hasCamera = false;

			for each (Engine::Assets::Storage::Types::CameraPack  cPack in nativePacks->cameras)
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
				nativePacks->cameras.push_back(Engine::Assets::Storage::Types::CameraPack(cameraId, type, &camera));
			}
		}


		void AddMaterial(unsigned int materialID, Material material)
		{
			bool hasMaterial = false;

			for each (Engine::Assets::Storage::Types::MaterialPack  cPack in nativePacks->materials)
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
				nativePacks->materials.push_back(Engine::Assets::Storage::Types::MaterialPack(materialID, material));
			}
		}

		void AddTexture2D(unsigned int textureId, Texture2D texture)
		{
			bool hasTexture2D = false;

			for (int x = 0; x < nativePacks->textures2d.size(); x++)
			{
				auto sP = &nativePacks->textures2d[x];

				if (sP->getId() == textureId)
				{
					sP->setResource(texture); // overwrite shader reference
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
				nativePacks->textures2d.push_back(Engine::Assets::Storage::Types::Texture2DPack(textureId, texture));
			}
		}

		bool HasMaterial(unsigned int materialID)
		{
			bool hasMaterial = false;

			for each (Engine::Assets::Storage::Types::MaterialPack  cPack in nativePacks->materials)
			{
				if (cPack.MaterialId == materialID)
				{
					hasMaterial = true;
					break;
				}
			}

			return hasMaterial;
		}

		bool HasTexture2D(unsigned int textureId)
		{
			bool hasTexture2D = false;

			for (int x = 0; x < nativePacks->textures2d.size(); x++)
			{
				auto sP = &nativePacks->textures2d[x];

				if (sP->getId() == textureId)
				{
					hasTexture2D = true;
					break;
				}
				else
				{
					hasTexture2D = false;
				}
			}

			return hasTexture2D;
		}

		bool HasShader(unsigned int shaderId)
		{
			bool hasShader = false;

			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = &nativePacks->shaders[x];

				if (sP->getId() == shaderId)
				{
					hasShader = true;
					break;
				}
			}

			return hasShader;
		}

		bool HasModel(unsigned int materialID)
		{
			bool hasMaterial = false;

			for each (Engine::Assets::Storage::Types::MaterialPack  cPack in nativePacks->materials)
			{
				if (cPack.MaterialId == materialID)
				{
					hasMaterial = true;
					break;
				}
				else
				{
					hasMaterial = false;
				}
			}

			return hasMaterial;
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

}

Engine::Assets::Storage::DataPacks* Engine::Assets::Storage::DataPacks::singleton_Ref;