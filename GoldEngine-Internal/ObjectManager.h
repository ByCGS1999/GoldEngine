#pragma once

// Engine namespaces
using namespace Engine;
using namespace Engine::Drawing;
using namespace Engine::EngineObjects;
using namespace Engine::EngineObjects::Native;
using namespace Engine::Internal;
using namespace Engine::Internal::Components;
using namespace Engine::Management;
using namespace Engine::Management::MiddleLevel;
using namespace Engine::Managers;

// System namespaces
using namespace System::Collections::Generic;


namespace Engine::Scripting
{
	public enum SceneTarget
	{
		RenderQueue,
		SceneObjects
	};

	/// <summary>
	/// Middle level class, allows for finding objects into the scene, and doing operations with them.
	/// Getting parents, etc.
	/// </summary>
	public ref class ObjectManager
	{
		// singleton
	private:
		static ObjectManager^ self;

	public:
		static ObjectManager^ singleton()
		{
			return self;
		}

	private:
		ArrayList^ sceneObjects;
		Scene^ loadedScene;
		Engine::Scripting::SceneTarget target = Engine::Scripting::RenderQueue; // by default hook RenderQueue

	public:
		ObjectManager(Scene^ loadedScene)
		{
			if (loadedScene != nullptr)
			{
				target = Engine::Scripting::RenderQueue;
				this->loadedScene = loadedScene;

				this->sceneObjects = gcnew ArrayList();
				this->sceneObjects = this->loadedScene->GetRenderQueue();
			}

			self = this;
		}

		// methods (dummy for now)

	public:
		void SetTarget(int newTarget)
		{
			target = (SceneTarget)newTarget;

			sceneObjects->Clear();

			switch ((SceneTarget)newTarget)
			{
			case Engine::Scripting::RenderQueue:
			{
				for each (auto T in loadedScene->GetRenderQueue())
				{
					sceneObjects->Add(T);
				}
			}
			break;
			case Engine::Scripting::SceneObjects:
			{
				for each (auto T in loadedScene->sceneObjects)
				{
					sceneObjects->Add(T);
				}
			}
			break;
			}
		}
		void SetTarget(Engine::Scripting::SceneTarget newTarget)
		{
			target = newTarget;

			sceneObjects->Clear();

			switch (newTarget) 
			{
			case Engine::Scripting::RenderQueue:
			{
				for each (auto T in loadedScene->GetRenderQueue())
				{
					sceneObjects->Add(T);
				}
			}
			break;
			case Engine::Scripting::SceneObjects:
			{
				for each (auto T in loadedScene->sceneObjects)
				{
					sceneObjects->Add(T);
				}
			}
			break;
			}
		}

	public:
		Scene^ GetLoadedScene()
		{
			return loadedScene;
		}

		List<Engine::Internal::Components::Object^>^ GetGameObjectsByName(System::String^ name)
		{
			auto objects = gcnew List<Engine::Internal::Components::Object^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->name == name)
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		List<Engine::Internal::Components::Object^>^ GetGameObjectsByType(Type^ type)
		{
			return gcnew List<Engine::Internal::Components::Object^>();
		}

		Engine::Internal::Components::Object^ GetFirstGameObjectByName(System::String^ name)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->name == name)
				{
					return t->GetReference();
				}
			}

			return nullptr;
		}

		Engine::Internal::Components::Object^ GetFirstGameObjectByType(Type^ name)
		{
			return nullptr;
		}

		Engine::Internal::Components::Object^ GetObjectFromTransform(Engine::Internal::Components::Transform^ transform)
		{
			return nullptr;
		}
	};
}