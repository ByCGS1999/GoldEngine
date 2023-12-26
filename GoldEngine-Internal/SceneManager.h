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

		static void LoadSceneFromFile(System::String^ fN, Engine::Management::Scene^ loadedScene)
		{
			if (AssetExists(fN))
			{
				auto parsedScene = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Management::Scene^>(System::IO::File::ReadAllText("Data/" + fN + ".scn"));

				loadedScene->sceneName = parsedScene->sceneName;
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

					auto renderQueue = loadedScene->GetRenderQueue();

					switch (objectType)
					{
					case Engine::Internal::Components::Datamodel:
						renderQueue->Add(
							gcnew Engine::Management::MiddleLevel::SceneObject(
								objectType,
								gcnew Engine::Internal::Components::Object(referenceObject->name, referenceObject->transform, objectType, referenceObject->parent)
							)
						);
						break;
					case Engine::Internal::Components::Skybox:
						instancedSkybox = gcnew Engine::EngineObjects::Skybox(referenceObject);
						renderQueue->Add(
							gcnew Engine::Management::MiddleLevel::SceneObject(
								objectType,
								instancedSkybox
							)
						);
						instancedSkybox->Init(instancedSkybox->material, instancedSkybox->texture);
						break;
					case Engine::Internal::Components::ModelRenderer:
						instancedModelRenderer = gcnew Engine::EngineObjects::ModelRenderer(referenceObject);
						renderQueue->Add(
							gcnew Engine::Management::MiddleLevel::SceneObject(
								objectType,
								instancedModelRenderer
							)
						);
						instancedModelRenderer->Init(instancedModelRenderer->model, instancedModelRenderer->material, instancedModelRenderer->texture, instancedModelRenderer->tint);
						break;
					}
				}
			}
			else

				loadedScene = gcnew Engine::Management::Scene(fN, "", gcnew System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>());

			if (loadedScene == nullptr)
				TraceLog(LOG_FATAL, "FAILED OPENING SCENE");
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