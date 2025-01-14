#pragma once

#include "Includes.h"
#include "Object/Transform.h"
#include "GlIncludes.h"
#include "EngineIncludes.h"

namespace Engine::Management::MiddleLevel
{
	public ref class SceneObject
	{
	public:
		Engine::Internal::Components::ObjectType objectType;
		String^ serializedData;

		Reflectable::ReflectableType^ reflectableType;

	private:
		GameObject^ sceneObject;

	public:
		SceneObject(Engine::Internal::Components::ObjectType type, Engine::Internal::Components::GameObject^ obj, System::String^ data)
		{
			objectType = type;
			sceneObject = obj;
			serializedData = data;

			if (obj == nullptr && serializedData != "" && serializedData != nullptr)
			{
				deserialize();
			}
			else if (obj != nullptr && serializedData == "")
			{
				reflectableType = gcnew Reflectable::ReflectableType();
				reflectableType->SetType(sceneObject->GetType());

				serialize();
			}
		}

		Engine::Internal::Components::GameObject^ GetReference()
		{
			if (sceneObject == nullptr)
				return nullptr;

			return sceneObject;
		}

		void SetReference(Engine::Internal::Components::GameObject^ ref)
		{
			this->sceneObject = ref;
		}

	public:
		generic <class T>
		T serializeAs()
		{
			return sceneObject->ToObjectType<T>();
		}

	public:
		void serialize()
		{
			serializedData = Serialize(sceneObject);
		}

		void deserialize()
		{
			sceneObject = (GameObject^)Deserialize(serializedData, reflectableType->getTypeReference());
		}

	public:
		generic <class type>
		type GetValue()
		{
			return sceneObject->ToObjectType<type>();
		}

		GameObject^ GetValue()
		{
			return sceneObject;
		}

		GameObject^ SerializeOutput()
		{
			reflectableType->DeserializeType();
			deserialize();

			return GetValue();
		}
	};
}
