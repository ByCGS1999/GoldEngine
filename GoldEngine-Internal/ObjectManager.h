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
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
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
				this->sceneObjects = (System::Collections::ArrayList^)this->loadedScene->GetRenderQueue();
			}

			Singleton<ObjectManager^>::Create(this);

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
		Engine::Internal::Components::GameObject^ GetObjectFromScene(Engine::Management::MiddleLevel::SceneObject^ sceneObject)
		{
			return sceneObject->GetReference();
		}

	private:
		bool topReferenceIsDatamodel(Engine::Internal::Components::GameObject^ object, String^ dataModelName)
		{
			if (object->transform->parent != nullptr)
				return topReferenceIsDatamodel((Engine::Internal::Components::GameObject^)object->transform->parent->getGameObject(), dataModelName);
			else
			{
				if (object->name == dataModelName)
					return true;
				else
					return false;
			}
		}

	public:

	#ifndef PRODUCTION_BUILD
		GameObject^ GetSelectedObject()
		{
			return (GameObject^)Singleton<Window^>::Instance->GetSelectedObject();
		}
	#endif

		Engine::Management::Scene^ GetLoadedScene()
		{
			return loadedScene;
		}

		GameObject^ GetDatamodel(String^ dataModelName)
		{
			return loadedScene->GetDatamodelMember(dataModelName, false);
		}

		GameObject^ GetDatamodel(String^ dataModelName, bool createDataModel)
		{
			return loadedScene->GetDatamodelMember(dataModelName, createDataModel);
		}

		List<Engine::Internal::Components::GameObject^>^ GetObjectsFromDatamodel(System::String^ datamodel)
		{
			auto objects = gcnew List<Engine::Internal::Components::GameObject^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (topReferenceIsDatamodel(t->GetReference(), datamodel))
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		List<Engine::Internal::Components::GameObject^>^ GetObjectsByTag(System::String^ tag)
		{
			auto objects = gcnew List<Engine::Internal::Components::GameObject^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->GetTag() == tag)
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		List<Engine::Internal::Components::GameObject^>^ GetObjectsByName(System::String^ name)
		{
			auto objects = gcnew List<Engine::Internal::Components::GameObject^>();

			if (sceneObjects == nullptr)
				return nullptr;

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->name == name)
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		List<Engine::Internal::Components::GameObject^>^ GetObjects()
		{
			auto objects = gcnew List<Engine::Internal::Components::GameObject^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if(t != nullptr && t->GetReference() != nullptr)
					objects->Add(t->GetReference());
			}

			return objects;
		}

		List<Engine::Internal::Components::GameObject^>^ GetObjectsOfType(Engine::Internal::Components::ObjectType type)
		{
			auto objects = gcnew List<Engine::Internal::Components::GameObject^>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->objectType == type)
				{
					objects->Add(t->GetReference());
				}
			}

			return objects;
		}

		generic <class T>
		List<T>^ GetObjectsOfType()
		{
			auto objects = gcnew List<T>();

			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetType() == T::typeid)
				{
					objects->Add(t->GetReference()->ToObjectType<T>());
				}
			}

			return objects;
		}

		Engine::Internal::Components::GameObject^ GetFirstObjectByTag(System::String^ tag)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->GetTag() == tag)
				{
					return t->GetReference();
				}
			}

			return nullptr;
		}

		Engine::Internal::Components::GameObject^ GetFirstObjectByName(System::String^ name)
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

		Engine::Internal::Components::GameObject^ GetFirstObjectOfType(System::String^ assemblyType)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetValue<Engine::EngineObjects::ScriptBehaviour^>()->assemblyReference == assemblyType)
				{
					return t->GetReference();
				}
			}
		}

		generic <class T>
		T GetFirstObjectOfType()
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t == nullptr)
					continue;

				if (t->GetReference()->GetType()->Equals(T::typeid) || t->GetReference()->GetType()->Equals(T::typeid->BaseType))
				{
					return t->GetReference()->ToObjectType<T>();
				}
			}
		}

		Engine::Internal::Components::GameObject^ GetFirstObjectOfType(System::Type^ type)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t == nullptr)
					continue;

				if (t->GetReference()->GetType()->Equals(type) || t->GetReference()->GetType()->IsSubclassOf(type))
				{
					return t->GetReference();
				}
			}
		}

		Engine::Internal::Components::GameObject^ GetFirstObjectOfType(Engine::Internal::Components::ObjectType type)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t == nullptr)
					continue;

				if (t->objectType == type)
				{
					return t->GetReference();
				}
			}
		}

		List<Engine::Internal::Components::GameObject^>^ GetChildrenOf(Engine::Internal::Components::GameObject^ parent)
		{
			List<Engine::Internal::Components::GameObject^>^ newList = gcnew List<Engine::Internal::Components::GameObject^>();

			for each (Engine::Management::MiddleLevel::SceneObject^ object in sceneObjects)
			{
				auto v = GetObjectFromScene(object);

				if (v->getTransform() == nullptr)
					continue;

				if (v->getTransform()->parent != nullptr)
				{
					if (v->getTransform()->GetParent()->GetUID() == parent->getTransform()->GetUID())
					{
						newList->Add(v);
					}
				}
			}

			return newList;
		}

		Engine::Internal::Components::GameObject^ GetObjectByUid(System::String^ uid)
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->getTransform()->GetUID() == uid)
				{
					return t->GetReference();
				}
			}

			return nullptr;
		}

		Engine::Internal::Components::GameObject^ GetObjectFromTransform(Engine::Internal::Components::Transform^ transform)
		{
			return nullptr;
		}

		Engine::EngineObjects::Camera^ GetMainCamera()
		{
			for each (Engine::Management::MiddleLevel::SceneObject ^ t in sceneObjects)
			{
				if (t->GetReference()->GetType()->IsSubclassOf(Engine::EngineObjects::Camera::typeid))
				{
					if (((Engine::EngineObjects::Camera^)t->GetReference())->attributes->getAttribute("IsMainCamera")->getValue<bool>() == true)
					{
						return (Engine::EngineObjects::Camera^)t->GetReference();
					}
				}
			}

			return nullptr;
		}

		void Instantiate(Engine::Internal::Components::GameObject^ newObject)
		{
			loadedScene->AddObjectToScene(newObject);
			loadedScene->PushToRenderQueue(newObject);
		}

		void Destroy(Engine::Internal::Components::GameObject^ object)
		{
			for each (Engine::Management::MiddleLevel::SceneObject^ objTmp in sceneObjects)
			{
				auto v = GetObjectFromScene(objTmp);

				if (v != nullptr)
				{
					if (v == object)
					{
						auto type = v->type;

						if (type == Engine::Internal::Components::ObjectType::Datamodel || type == Engine::Internal::Components::ObjectType::Daemon || type == Engine::Internal::Components::ObjectType::LightManager || v->isProtected())
							return;

						// REPARENT ALL THE CHILDREN TO NULL (SET AS UNPARENTED).
						List<Engine::Internal::Components::GameObject^>^ objectList = ObjectManager::singleton()->GetChildrenOf(object);

						for each (auto obj in objectList)
						{
							obj->getTransform()->SetParent(nullptr);
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


		// patching attributes


	public:
		void PatchAttributeObject(Attribute^ attribute)
		{
			if (attribute == nullptr)
				return;

			if (attribute->getValue() != nullptr)
			{
				if ((attribute->getValue()->GetType()->Equals(Engine::Internal::Components::GameObject::typeid) || attribute->getValue()->GetType()->IsSubclassOf(Engine::Internal::Components::GameObject::typeid)))
				{
					String^ uid = ((Engine::Internal::Components::GameObject^)attribute->getValue())->getTransform()->GetUID();

					attribute->setValueForce(GetObjectByUid(uid), false);
				}
			}
			else
			{
				attribute->setValueForce(nullptr, false);
			}
		}

	};
}