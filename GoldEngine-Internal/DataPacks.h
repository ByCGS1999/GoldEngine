#pragma once
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include "Includes.h"
#include "GlIncludes.h"
#include "Object/Transform.h"
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
		std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Shader>*> shaders;
		std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Model>*> models;
		//std::map<unsigned int, Engine::Assets::Storage::Types::CameraPack> cameras;
		//std::map<unsigned int, Engine::Assets::Storage::Types::MaterialPack> materials;
		std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Texture2D>*> textures2d;
		std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Sound>*> sounds;
		std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Music>*> musics;

		Engine::Native::EnginePtr<RAYLIB::Shader>* fallbackShader = nullptr;
		Engine::Native::EnginePtr<RAYLIB::Model>* fallbackModel = nullptr;
		Engine::Native::EnginePtr<RAYLIB::Texture2D>* fallbackTexture = nullptr;

		NativeDataPack();
	};

	public class DataPacks
	{
	private:
		NativeDataPack* nativePacks;

	public:
		DataPacks();

		void LoadDefaultAssets();

		bool dataPackHasAssets()
		{
			int assetCount = 0;

			assetCount += nativePacks->shaders.size();
			assetCount += nativePacks->models.size();
			assetCount += nativePacks->textures2d.size();
			//assetCount += nativePacks->materials.size();
			//assetCount += nativePacks->cameras.size();
			assetCount += nativePacks->sounds.size();
			assetCount += nativePacks->musics.size();

			return (assetCount > 0);
		}

		void FreeShaders()
		{
			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = nativePacks->shaders[x];

				sP->release();
			}

			nativePacks->shaders.clear();
		}

		void FreeMusics()
		{
			for (int x = 0; x < nativePacks->musics.size(); x++)
			{
				auto sP = nativePacks->musics[x];

				sP->release();
			}

			nativePacks->musics.clear();
		}


		void FreeSounds()
		{
			for (int x = 0; x < nativePacks->sounds.size(); x++)
			{
				auto sP = nativePacks->sounds[x];

				sP->release();
			}

			nativePacks->sounds.clear();
		}


		void FreeShader(unsigned int shaderId)
		{
			for (int x = 0; x < nativePacks->shaders.size(); x++)
			{
				auto sP = nativePacks->shaders[x];

				sP->release();
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

			//nativePacks->materials.clear();
		}

		void FreeModels()
		{
			for (int x = 0; x < nativePacks->models.size(); x++)
			{
				auto sP = nativePacks->models[x];

				sP->release();
			}

			nativePacks->models.clear();
		}

		void FreeCameras()
		{
			//nativePacks->cameras.clear();
		}

		void FreeTextures2D()
		{
			for (int x = 0; x < nativePacks->textures2d.size(); x++)
			{
				auto tP = nativePacks->textures2d[x];

				tP->release();
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
			FreeSounds();
			FreeMusics();
		}

		RAYLIB::Camera3D GetCamera3D(unsigned int cameraId)
		{
			RAYLIB::Camera3D camera = { };
			/*
			for each (Engine::Assets::Storage::Types::CameraPack  cP in nativePacks->cameras)
			{
				if ((cP.cameraId == cameraId) && (cP.cameraType == Engine::Internal::Components::CameraType::C3D))
				{
					camera = cP.camera3D;
					break;
				}
			}
			*/
			return camera;
		}

		RAYLIB::Camera2D GetCamera2D(unsigned int cameraId)
		{
			RAYLIB::Camera2D camera = { };
			/*
			auto cP = nativePacks->cameras[cameraId];
			if ((cP.cameraId == cameraId) && (cP.cameraType == Engine::Internal::Components::CameraType::C2D))
			{
				camera = cP.camera2D;
			}
			*/
			return camera;
		}

		Texture2D& GetTexture2D(unsigned int textureId);

		Sound& GetSound(unsigned int soundId);
		Sound* GetSoundPtr(unsigned int soundId);
		void AddSound(unsigned int soundId, Sound sound);

		Music& GetMusic(unsigned int musicId);
		Music* GetMusicPtr(unsigned int musicId);
		void AddMusic(unsigned int musicId, Music music);

		Model& GetModel(unsigned int modelId);
		RAYLIB::Shader& GetShader(unsigned int shaderId);

		void AddShader(unsigned int shaderId, RAYLIB::Shader& shader);
		void AddModel(unsigned int modelId, Model modelRef);
		void AddTexture2D(unsigned int textureId, Texture2D texture);

		bool HasTexture2D(unsigned int textureId)
		{
			bool hasTexture2D = false;


			auto sP = &nativePacks->textures2d[textureId];

			if (sP != nullptr)
				hasTexture2D = true;

			return hasTexture2D;
		}

		bool HasShader(unsigned int shaderId)
		{
			bool hasShader = false;

			auto sP = &nativePacks->shaders[shaderId];

			if (sP != nullptr)
			{
				hasShader = true;
			}

			return hasShader;
		}

		bool HasModel(unsigned int materialID)
		{
			bool hasMaterial = false;

			auto sP = &nativePacks->models[materialID];

			if (sP != nullptr)
				hasMaterial = true;


			return hasMaterial;
		}

		static DataPacks* instance;

		static DataPacks singleton();

		NativeDataPack* GetNativeDataPack();
	};

}