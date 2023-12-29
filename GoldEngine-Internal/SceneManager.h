#pragma once

#include "Skybox.h"
#include "ModelRenderer.h"

namespace Engine::Managers
{
	public ref class SceneManager
	{
	private:

	public:
		static bool AssetExists(System::String^ fN)
		{
			return System::IO::File::Exists("Data/" + fN + ".scn");
		}

		static Engine::Management::Scene^ LoadSceneFromFile(System::String^ fN, Engine::Management::Scene^ loadedScene)
		{
			if (AssetExists(fN))
			{
				auto parsedScene = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Management::Scene^>(System::IO::File::ReadAllText("Data/" + fN + ".scn"));

				loadedScene->sceneName = fN;
				loadedScene->assetPack = parsedScene->assetPack;
				loadedScene->sceneObjects = parsedScene->sceneObjects;

				for each (auto t in parsedScene->sceneObjects)
				{
					// TYPE DEF \\

					Engine::EngineObjects::Skybox^ instancedSkybox;
					Engine::EngineObjects::ModelRenderer^ instancedModelRenderer;

					// PARAMS \\

					auto referenceObject = (Engine::Internal::Components::Object^)t->reference;
					auto objectType = t->objectType;
					auto deserializedData = t->deserializedData;

					auto renderQueue = loadedScene->GetRenderQueue();

					switch (objectType)
					{
					case Engine::Internal::Components::Datamodel:
					{
						auto sceneObject = gcnew Engine::Management::MiddleLevel::SceneObject(
							objectType,
							nullptr,
							deserializedData
						);

						renderQueue->Add(
							sceneObject
						);
					}
					break;
					case Engine::Internal::Components::ModelRenderer:
					{
						auto sceneObject = gcnew Engine::Management::MiddleLevel::SceneObject(
							objectType,
							nullptr,
							deserializedData
						);

						renderQueue->Add(
							sceneObject
						);

						instancedModelRenderer = sceneObject->GetValue< Engine::EngineObjects::ModelRenderer^ >();
						Model model = DataPacks::singleton().GetModel(instancedModelRenderer->model);
						Material material = DataPacks::singleton().GetMaterial(instancedModelRenderer->material);
						Texture texture = DataPacks::singleton().GetTexture2D(instancedModelRenderer->texture);
						unsigned int hex = instancedModelRenderer->tint;

						instancedModelRenderer->SetNativeRenderer(new Engine::EngineObjects::Native::NativeModelRenderer(model, material, texture, hex));
					}
					break;
					case Engine::Internal::Components::Skybox:
					{
						auto sceneObject = gcnew Engine::Management::MiddleLevel::SceneObject(
							objectType,
							nullptr,
							deserializedData
						);

						renderQueue->Add(
							sceneObject
						);

						instancedSkybox = sceneObject->GetValue<Engine::EngineObjects::Skybox^>();
						instancedSkybox->Init(instancedSkybox->model, instancedSkybox->material, instancedSkybox->texture);
					}
					break;
					}
				}
			}
			else

				loadedScene = gcnew Engine::Management::Scene(fN, "", gcnew System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>());

			if (loadedScene == nullptr)
				TraceLog(LOG_FATAL, "FAILED OPENING SCENE");

			return loadedScene;
		}

		static Engine::Management::Scene^ CreateScene(System::String^ sceneName)
		{
			if (sceneName->Equals(""))
				return gcnew Engine::Management::Scene("Level0", "", gcnew System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>());
			else
				return gcnew Engine::Management::Scene(sceneName, "", gcnew System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>());
		}

		static void SaveSceneToFile(Engine::Management::Scene^ scene, unsigned int password)
		{
			if (scene->sceneName)
			{
				String^ serializedData = Newtonsoft::Json::JsonConvert::SerializeObject(scene, Newtonsoft::Json::Formatting::Indented);
				String^ cipheredContents = CypherLib::EncryptFileContents(serializedData, password);

				//System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", System::Convert::ToBase64String(Encoding::UTF8->GetBytes(cipheredContents)));
				System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", serializedData);
			}
			else
			{
				// create file and rerun save
				System::IO::File::Create("Data/" + scene->sceneName + ".scn")->Close();
				SaveSceneToFile(scene, password);
			}
		}

		static void UnloadScene(Engine::Management::Scene^ loadedScene)
		{
			loadedScene->UnloadScene();
		}
	};
}