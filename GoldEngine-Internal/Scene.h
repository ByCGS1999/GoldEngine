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
	public ref class Scene
	{
		// Privates
	private:
		System::Collections::ArrayList^ drawQueue; // Unlike Java, in C#/C++ CLR arraylists don't need to be from an specified type (This is what we're using for game rendering & updating.)
		System::Collections::Generic::List<EngineAssembly^>^ assemblies; // loaded assemblies -> passcall from scene_assemblies -> get assemblies for preloading -> tbh idk what more.
		DataPack^ sceneDatapack;
		bool sceneFinishedLoading;
		unsigned int password;

		// Properties
	public:
		System::String^ sceneName;
		System::Collections::Generic::List<System::String^>^ assetPacks;
		System::String^ sceneRequirements;
		unsigned long skyColor;
		System::Collections::Generic::List<System::String^>^ scene_assemblies;
		System::Collections::Generic::List<System::String^>^ preload_scripts;
		System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneObjects; // Managed storage + object parsing.

		// Constructors
	public:
		Scene(String^ name, String^ sR, System::Collections::Generic::List<String^>^ assetP, System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneO, unsigned long skyTint, System::Collections::Generic::List<System::String^>^ assemblies, System::Collections::Generic::List<String^>^ preloadCode)
		{
			this->sceneName = name;
			this->assetPacks = assetP;
			this->sceneRequirements = sR;
			this->sceneObjects = sceneO;
			this->preload_scripts = preloadCode;
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

		void LoadScene()
		{
			printf("Loading scene\n");

			if(DataPacks::singleton().dataPackHasAssets())
				DataPacks::singleton().FreeAll(); // free all the assets

			for each (auto packRoute in assetPacks)
			{
				Engine::Assets::IO::FileManager::ReadCustomFileFormat(packRoute, "ThreadcallNull");
			}

			sceneDatapack->ReadFromFile(sceneRequirements, password);

			Directory::Delete("Data/tmp/", true);

			sceneFinishedLoading = true;

			OnLoad();
		}

		void UnloadScene()
		{
			printf("Unloading scene\n");

			//OnUnload();
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

		void AddObjectToScene(Engine::Internal::Components::Object^ object)
		{
			auto tmp = gcnew Engine::Management::MiddleLevel::SceneObject
			(
				object->type,
				object,
				""
			);

			sceneObjects->Add(tmp);
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

		Engine::Internal::Components::Object^ GetDatamodelMember(System::String^ datamodel)
		{
			Engine::Internal::Components::Object^ retn = nullptr;
			for each (auto objects in drawQueue)
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->objectType == Engine::Internal::Components::ObjectType::Datamodel)
				{
					if (sceneObject->GetReference()->name == datamodel)
						retn = sceneObject->GetReference();
				}
			}

			if (retn == nullptr)
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

		void PushToRenderQueue(Engine::Internal::Components::Object^ object)
		{
			msclr::lock^ l = gcnew msclr::lock(drawQueue);
			{
				drawQueue->Add(gcnew Engine::Management::MiddleLevel::SceneObject(
					object->type,
					object,
					""));
			}
			l->release();
		}

		Engine::Internal::Components::Object^ GetObjectByNameFromDrawQueue(System::String^ name)
		{
			Engine::Internal::Components::Object^ object = nullptr;

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
		Engine::Internal::Components::Object^ AddDatamodelMember(System::String^ datamodel)
		{
			return gcnew Engine::Internal::Components::Object(datamodel,
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					0.0f,
					gcnew Engine::Internal::Components::Vector3(1, 1, 1),
					nullptr
				),
				Engine::Internal::Components::ObjectType::Datamodel, "");
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
		virtual void Preload()
		{
			for each (auto t in scene_assemblies)
			{
				auto newAssembly = gcnew EngineAssembly(t);
				assemblies->Add(newAssembly);
			}

			sceneDatapack->ReadFromFile(sceneRequirements, password);

			for each (auto p in preload_scripts)
			{
				for each (auto a in assemblies)
				{
					if (a->hasType(p))
					{
						auto newInst = a->Create<Engine::Preload::PreloadScript^>(p);
						newInst->Preload();
					}
				}
			}

			LoadScene();
		}
	};
}