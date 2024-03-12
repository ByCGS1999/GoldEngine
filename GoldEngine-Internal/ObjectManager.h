#pragma once

// System namespaces
using namespace System;
using namespace System::Collections;
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
		Engine::Management::Scene^ loadedScene;
		Engine::Scripting::SceneTarget target = Engine::Scripting::RenderQueue; // by default hook RenderQueue

	public:
		ObjectManager(Engine::Management::Scene^ loadedScene)
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

	private:
		Engine::Internal::Components::Object^ GetObjectFromScene(Engine::Management::MiddleLevel::SceneObject^ sceneObject)
		{
			return sceneObject->GetReference();
		}

	public:
		Engine::Management::Scene^ GetLoadedScene()
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

		List<Engine::Internal::Components::Object^>^ GetObjectsOfType(Engine::Internal::Components::ObjectType type)
		{
			auto objects = gcnew List<Engine::Internal::Components::Object^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->objectType == type)
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		Engine::Internal::Components::Object^ GetFirstObjectOfName(System::String^ name)
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

		Engine::Internal::Components::Object^ GetFirstObjectOfType(System::String^ assemblyType)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetValue<Engine::EngineObjects::Script^>()->assemblyReference == assemblyType)
				{
					return t->GetReference();
				}
			}
		}

		Engine::Internal::Components::Object^ GetFirstObjectOfType(Engine::Internal::Components::ObjectType type)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->objectType == type)
				{
					return t->GetReference();
				}
			}
		}

		List<Engine::Internal::Components::Object^>^ GetChildrenOf(Engine::Internal::Components::Object^ parent)
		{
			List<Engine::Internal::Components::Object^>^ newList = gcnew List<Engine::Internal::Components::Object^>();

			for each (Engine::Management::MiddleLevel::SceneObject^ object in sceneObjects)
			{
				auto v = GetObjectFromScene(object);
				if (v->transform->parent != nullptr)
				{
					if (v->transform->GetParent()->uid == parent->transform->uid)
					{
						newList->Add(v);
					}
				}
			}

			return newList;
		}

		Engine::Internal::Components::Object^ GetGameObjectByUid(System::String^ uid)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->GetTransform()->uid == uid)
				{
					return t->GetReference();
				}
			}

			return nullptr;
		}

		Engine::Internal::Components::Object^ GetObjectFromTransform(Engine::Internal::Components::Transform^ transform)
		{
			return nullptr;
		}

		Engine::Internal::Components::Object^ Instantiate(Engine::Internal::Components::Object^ newObject)
		{
			loadedScene->AddObjectToScene(newObject);
			loadedScene->PushToRenderQueue(newObject);

			return newObject;
		}

		void Destroy(Engine::Internal::Components::Object^ object)
		{
			for each (Engine::Management::MiddleLevel::SceneObject^ objTmp in sceneObjects)
			{
				auto v = GetObjectFromScene(objTmp);

				if (v != nullptr)
				{
					if (v == object)
					{
						auto type = v->type;

						if (type == Engine::Internal::Components::ObjectType::Datamodel || type == Engine::Internal::Components::ObjectType::Daemon || type == Engine::Internal::Components::ObjectType::LightManager)
							return;

						// REPARENT ALL THE CHILDREN TO NULL (SET AS UNPARENTED).
						List<Engine::Internal::Components::Object^>^ objectList = ObjectManager::singleton()->GetChildrenOf(object);

						for each (auto obj in objectList)
						{
							obj->transform->SetParent(nullptr);
						}

						// PURGE THE OBJECT FROM THE SCENE
						loadedScene->RemoveObjectFromScene(objTmp);

						// call destroy method (for self impl)
						object->Destroy();
						break;
					}
				}
			}
		}
	};
}