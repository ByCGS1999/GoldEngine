#pragma once
#include "DataPacks.h"
#include "DataPack.h"
#include "FileManager.h"
#include "SceneObject.h"
#include "AsmLoader.h"
#include "PreloadScript.h"

using namespace Engine::Assets::IO;
using namespace Engine::Assets::Storage;
using namespace Engine::Assets::Management;

namespace Engine::Management
{
	public delegate void OnSceneLoaded();

	public ref class Scene
	{
		// Privates
	private:
		System::Collections::ArrayList^ drawQueue;
		System::Collections::Generic::List<EngineAssembly^>^ assemblies; // loaded assemblies -> passcall from scene_assemblies -> get assemblies for preloading -> tbh idk what more.
		DataPack^ sceneDatapack;
		bool sceneFinishedLoading;
		unsigned int password;
		static Scene^ singleton;
		event OnSceneLoaded^ onLoadedScene;

		// Properties
	public:
		System::String^ sceneName;
		System::Collections::Generic::List<System::String^>^ assetPacks;
		System::String^ sceneRequirements;
		unsigned long skyColor;
		System::Collections::Generic::List<System::String^>^ scene_assemblies;
		System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneObjects; // Managed storage + object parsing.

		// Constructors
	public:
		Scene(String^ name, String^ sR, System::Collections::Generic::List<String^>^ assetP, System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneO, unsigned long skyTint, System::Collections::Generic::List<System::String^>^ assemblies)
		{
			Singleton<Scene^>::Create(this);

			singleton = this;

			this->sceneName = name;
			this->assetPacks = assetP;
			this->sceneRequirements = sR;
			this->sceneObjects = sceneO;
			this->skyColor = skyTint;
			scene_assemblies = assemblies;
			this->sceneDatapack = gcnew DataPack(sceneRequirements);
			drawQueue = gcnew System::Collections::ArrayList();
			this->sceneFinishedLoading = false;
		}

	public:
		void setPassword(unsigned int passwd)
		{
			password = passwd;
		}

		// Methods
	public:
		DataPack^ getSceneDataPack()
		{
			return sceneDatapack;
		}

		bool sceneLoaded() { return sceneFinishedLoading; }

		void flagSceneLoaded(bool value) { sceneFinishedLoading = value; }

		static Scene^ getLoadedScene()
		{
			return singleton;
		}

		void LoadScene()
		{
			printConsole("Loading scene " + sceneName);

			if(DataPacks::singleton().dataPackHasAssets())
				DataPacks::singleton().FreeAll(); // free all the assets

			for each (auto packRoute in assetPacks)
			{
				printConsole("Loading asset -> " + packRoute);

				Engine::Assets::IO::FileManager::ReadCustomFileFormat(packRoute, Engine::Config::EngineSecrets::singleton()->encryptionPassword);
			}

			sceneDatapack->setFile(sceneRequirements);
			sceneDatapack->ReadFromFile(sceneRequirements, password);

			printConsole("All AssetsPacks have been unpacked and loaded into memory");

			sceneFinishedLoading = true;

			if(Directory::Exists("Data/unpacked/"))
				Directory::Delete("Data/unpacked/", true);
				
			OnLoad();

		}

		void UnloadScene()
		{
			sceneFinishedLoading = false;

			printConsole("Unloading scene");

			OnUnload();
			sceneObjects->Clear();
			drawQueue->Clear();
			DataPacks::singleton().FreeAll();
			System::GC::Collect();
		}

		void RemoveObjectFromScene(Engine::Management::MiddleLevel::SceneObject^ object)
		{
			if (sceneObjects->Contains(object))
			{
				sceneObjects->Remove(object);
			}

			if (drawQueue->Contains(object))
			{
				drawQueue->Remove(object);
			}
		}

		void AddObjectToScene(Engine::Internal::Components::GameObject^ object)
		{
			auto tmp = gcnew Engine::Management::MiddleLevel::SceneObject
			(
				object->type,
				object,
				""
			);

			msclr::lock^ _lock = gcnew msclr::lock(sceneObjects);
			_lock->acquire();
			{
				sceneObjects->Add(tmp);
			}
			_lock->release();
		}

		System::Collections::ArrayList^ GetRenderQueue() { return drawQueue; }

		bool ExistsDatamodelMember(System::String^ datamodel)
		{
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->objectType == Engine::Internal::Components::ObjectType::Datamodel)
				{
					if (sceneObject->GetReference()->name == datamodel)
						return true;
				}
			}

			return false;
		}


		bool ExistsMember(System::String^ datamodel)
		{
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->GetReference()->name == datamodel)
					return true;
			}

			return false;
		}

		Engine::Internal::Components::GameObject^ GetMember(System::String^ datamodel)
		{
			Engine::Internal::Components::GameObject^ retn = nullptr;
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->GetReference()->name == datamodel)
					return sceneObject->GetReference();
			}

			return nullptr;
		}

		Engine::Internal::Components::GameObject^ GetDatamodelMember(System::String^ datamodel)
		{
			Engine::Internal::Components::GameObject^ retn = nullptr;
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->objectType == Engine::Internal::Components::ObjectType::Datamodel)
				{
					if (sceneObject->GetReference()->name == datamodel)
						return sceneObject->GetReference();
				}
			}

			return retn;
		}


		Engine::Internal::Components::GameObject^ GetDatamodelMember(System::String^ datamodel, bool create)
		{
			Engine::Internal::Components::GameObject^ retn = nullptr;
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->objectType == Engine::Internal::Components::ObjectType::Datamodel)
				{
					if (sceneObject->GetReference()->name == datamodel)
						return sceneObject->GetReference();
				}
			}

			if (retn == nullptr && create)
			{
				auto newMember = AddDatamodelMember(datamodel);
				auto newObject = gcnew Engine::Management::MiddleLevel::SceneObject(Engine::Internal::Components::ObjectType::Datamodel, newMember, "");

				retn = newMember;

				AddObjectToScene(newMember);
				drawQueue->Add(newObject);
			}

			return retn;
		}

		void CopyRenderQueueToSceneObjects()
		{
			sceneObjects->Clear();
			// serialize all the objects
			for each (Engine::Management::MiddleLevel::SceneObject ^ object in drawQueue)
			{
				object->deserialize();
				sceneObjects->Add(object);
			}
		}

		void PushToRenderQueue(Engine::Internal::Components::GameObject^ object)
		{
			msclr::lock^ l = gcnew msclr::lock(drawQueue);
			l->acquire();
			{
				drawQueue->Add(gcnew Engine::Management::MiddleLevel::SceneObject(
					object->type,
					object,
					""));

				if (!sceneFinishedLoading)
				{
					onLoadedScene += gcnew OnSceneLoaded(object, &Engine::Internal::Components::GameObject::Setup);
					onLoadedScene += gcnew OnSceneLoaded(object, &Engine::Internal::Components::GameObject::Init);
					onLoadedScene += gcnew OnSceneLoaded(object, &Engine::Internal::Components::GameObject::Start);
				}
				else
				{
					object->Setup();
					object->Init();
					object->Start();
				}
			}
			l->release();
		}


		void PushToRenderQueue(Engine::Management::MiddleLevel::SceneObject^ object)
		{
			msclr::lock^ l = gcnew msclr::lock(drawQueue);
			{
				drawQueue->Add(object);

				if (!sceneFinishedLoading)
				{
					onLoadedScene += gcnew OnSceneLoaded(object->GetReference(), &Engine::Internal::Components::GameObject::Setup);
					onLoadedScene += gcnew OnSceneLoaded(object->GetReference(), &Engine::Internal::Components::GameObject::Init);
					onLoadedScene += gcnew OnSceneLoaded(object->GetReference(), &Engine::Internal::Components::GameObject::Start);
				}
				else
				{
					object->GetReference()->Setup();
					object->GetReference()->Init();
					object->GetReference()->Start(); // call start method (otherwise by serialization it won't be called.)
				}
			}
			l->release();
		}


		Engine::Internal::Components::GameObject^ GetObjectByNameFromDrawQueue(System::String^ name)
		{
			Engine::Internal::Components::GameObject^ object = nullptr;

			for each (Engine::Management::MiddleLevel::SceneObject^ obj in GetRenderQueue())
			{
				if (obj->GetReference()->name == name)
				{
					object = obj->GetReference();
					break;
				}
			}

			return object;
		}

	private protected:
		Engine::Internal::Components::GameObject^ AddDatamodelMember(System::String^ datamodel)
		{
			return gcnew Engine::Internal::Components::GameObject(datamodel,
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(1, 1, 1),
					nullptr
				),
				Engine::Internal::Components::ObjectType::Datamodel, 
				"", 
				Engine::Scripting::LayerManager::GetLayerFromId(0)
			);
		}

	public:
		void HookSceneInit()
		{
			onLoadedScene();
		}

		// VMethods
	public:
		virtual void OnUnload()
		{

		}
		virtual void OnLoad()
		{

		}

		virtual void Draw()
		{

		}
		virtual void Update() 
		{

		}
		virtual void PhysicsUpdate()
		{

		}
		virtual void Preload(List<EngineAssembly^>^ asms)
		{
			assemblies = asms;

			for each (auto t in scene_assemblies)
			{
				auto newAssembly = gcnew EngineAssembly(t);
				assemblies->Add(newAssembly);
			}

			for each (EngineAssembly ^ assm in assemblies)
			{
				for each (Type ^ asmType in assm->getPreloadScripts())
				{
					try
					{
						if (((String^)asmType->GetMethod("GetTarget")->Invoke(nullptr, nullptr))->Equals(this->sceneName) || asmType->GetMethod("GetTarget")->Invoke(nullptr, nullptr)->Equals("*"))
						{
							asmType->GetMethod("Preload")->Invoke(nullptr, nullptr);
						}
					}
					catch (Exception^ ex)
					{
						printError(ex->Message);
						printError(ex->StackTrace);
					}
				}
			}

			LoadScene();
		}
	};
}
