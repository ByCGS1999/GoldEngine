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
		System::String^ assetPack;
		System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneObjects; // Managed storage + object parsing.

		// Constructors
	public:
		Scene(String^ name, String^ assetP, System::Collections::Generic::List<Engine::Management::MiddleLevel::SceneObject^>^ sceneO)
		{
			this->sceneName = name;
			this->assetPack = assetP;
			this->sceneObjects = sceneO;
			drawQueue = gcnew System::Collections::ArrayList();
		}
		
		// Methods
	public:
		void LoadScene();
		void UnloadScene();
		void AddObjectToScene(Engine::Internal::Components::Object^ object);
		System::Collections::ArrayList^ GetRenderQueue() { return drawQueue; }

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