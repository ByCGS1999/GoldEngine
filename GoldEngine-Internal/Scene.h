#pragma once
#include "DataPacks.h"
#include "DataPack.h"
#include "FileManager.h"
#include "SceneObject.h"


namespace Engine::Management
{
	public ref class Scene
	{
		// Privates
	private:
		System::Collections::ArrayList^ drawQueue; // Unlike Java, in C#/C++ CLR arraylists don't need to be from an specified type (This is what we're using for game rendering & updating.)

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
			this->sceneName = name;
			this->assetPacks = assetP;
			this->sceneRequirements = sR;
			this->sceneObjects = sceneO;
			this->skyColor = skyTint;
			scene_assemblies = assemblies;
			drawQueue = gcnew System::Collections::ArrayList();
		}


		// Methods
	public:
		void LoadScene()
		{
			DataPack^ pack = gcnew DataPack();
			DataPacks::singleton().FreeAll(); // free all the assets

			for each (auto packRoute in assetPacks)
			{
				FileManager::ReadCustomFileFormat(packRoute, "ThreadcallNull");
			}

			pack->ReadFromFile(sceneRequirements, 1234);

			Directory::Delete("Data/tmp/", true);

			OnLoad();
		}

		void UnloadScene()
		{
			//OnUnload();
			sceneObjects->Clear();
			drawQueue->Clear();
			DataPacks::singleton().FreeAll();
			System::GC::Collect();
		}

		void AddObjectToScene(Engine::Internal::Components::Object^ object)
		{
			auto tmp = gcnew Engine::Management::MiddleLevel::SceneObject(
				object->type,
				object,
				"");

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
			drawQueue->Add(gcnew Engine::Management::MiddleLevel::SceneObject(
				object->type,
				object,
				""));
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
				Engine::Internal::Components::ObjectType::Datamodel);
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

		}
	};
}