#pragma once

namespace Engine::Management
{
	ref class Scene
	{
		// PROPERTIES
	public:
		int sceneId;
		System::String^ sceneName;
		System::Collections::ArrayList sceneObjects;
		
		// Methods
	public:
		void LoadScene();
		void UnloadScene();
		void AddObjectToScene(System::Object^ object);

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