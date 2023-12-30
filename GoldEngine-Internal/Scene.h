#pragma once
#include "SceneObject.h"


namespace Engine::Management
{
	ref class Scene
	{
		// Privates
	private:
		System::Collections::ArrayList^ drawQueue; // Unlike Java, in C#/C++ CLR arraylists don't need to be from an specified type (This is what we're using for game rendering & updating.)

		// Properties
	public:
		System::String^ sceneName;
		System::Collections::Generic::List<System::String^>^ assetPacks;
		System::String^ sceneRequirements;
		System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneObjects; // Managed storage + object parsing.

		// Constructors
	public:
		Scene(String^ name, String^ sR, System::Collections::Generic::List<String^>^ assetP, System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneO)
		{
			this->sceneName = name;
			this->assetPacks = assetP;
			this->sceneRequirements = sR;
			this->sceneObjects = sceneO;
			drawQueue = gcnew System::Collections::ArrayList();
		}
		
		// Methods
	public:
		void LoadScene();
		void UnloadScene();
		void AddObjectToScene(Engine::Internal::Components::Object^ object);
		System::Collections::ArrayList^ GetRenderQueue() { return drawQueue; }
		Engine::Internal::Components::Object^ GetDatamodelMember(System::String^ datamodel) 
		{ 
			Engine::Internal::Components::Object^ retn = nullptr; 
			for each (auto objects in drawQueue) 
			{
				Engine::Management::MiddleLevel::SceneObject^ sceneObject = (Engine::Management::MiddleLevel::SceneObject^)objects;

				if (sceneObject->objectType == Engine::Internal::Components::Datamodel)
				{
					if (sceneObject->reference->name == datamodel)
						retn = sceneObject->reference;
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

	private protected:
		Engine::Internal::Components::Object^ AddDatamodelMember(System::String^ datamodel)
		{
			return gcnew Engine::Internal::Components::Object(datamodel,
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					1.0f,
					nullptr
				),
				Engine::Internal::Components::ObjectType::Datamodel);
		}

		// VMethods
	public:
		virtual void OnUnload();
		virtual void OnLoad();
		virtual void Draw();
		virtual void Update();
		virtual void PhysicsUpdate();
		virtual void Preload();
	};
}