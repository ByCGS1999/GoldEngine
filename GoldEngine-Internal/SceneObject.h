#pragma once

#include "Cast.h"
#include "Skybox.h"
#include "CubeRenderer.h"
#include "GridRenderer.h"
#include "ModelRenderer.h"
#include "Script.h"

namespace Engine::Management::MiddleLevel
{
	public ref class SceneObject
	{
	public:
		Engine::Internal::Components::ObjectType objectType;
		System::String^ deserializedData;
	private:
		Engine::Internal::Components::Object^ reference;

	public:
		SceneObject(Engine::Internal::Components::ObjectType type, Engine::Internal::Components::Object^ obj, System::String^ data)
		{
			objectType = type;
			reference = obj;
			deserializedData = data;

			if (obj == nullptr && deserializedData != "")
			{
				serialize();
			}
			else if(obj != nullptr && deserializedData == "")
			{
				deserialize();
			}
		}

		Engine::Internal::Components::Object^ GetReference()
		{
			return reference;
		}

	public:
		void serialize()
		{
			switch (objectType)
			{
			case Engine::Internal::Components::ObjectType::Skybox:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Skybox^>(deserializedData);
			}
			break;
			
			case Engine::Internal::Components::ObjectType::Generic:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Internal::Components::Object^>(deserializedData);
			}
			break;

			case Engine::Internal::Components::ObjectType::Datamodel:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Internal::Components::Object^>(deserializedData);
			}
			break;

			case Engine::Internal::Components::ObjectType::ModelRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::ModelRenderer^>(deserializedData);
			}
			break;

			case Engine::Internal::Components::ObjectType::CubeRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::CubeRenderer^>(deserializedData);
			}
			break;

			case Engine::Internal::Components::ObjectType::GridRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::GridRenderer^>(deserializedData);
			}
			break;

			case Engine::Internal::Components::ObjectType::Script:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Script^>(deserializedData);
			}
			break;

			}
		}
		void deserialize()
		{
			switch (objectType)
			{
			case Engine::Internal::Components::Skybox:
			{
				Engine::EngineObjects::Skybox^ skybox = (Engine::EngineObjects::Skybox^)reference;
				deserializedData = ToJSON(skybox);
			}
			break;
			
			case Engine::Internal::Components::Generic:
			{
				Engine::Internal::Components::Object^ genericType = (Engine::Internal::Components::Object^)reference;
				deserializedData = ToJSON(genericType);
			}
			break;

			case Engine::Internal::Components::ModelRenderer:
			{
				Engine::EngineObjects::ModelRenderer^ modelRenderer = (Engine::EngineObjects::ModelRenderer^)reference;
				deserializedData = ToJSON(modelRenderer);
			}
			break;

			case Engine::Internal::Components::Datamodel:
			{
				Engine::Internal::Components::Object^ genericType = (Engine::Internal::Components::Object^)reference;
				deserializedData = ToJSON(genericType);
			}
			break;

			case Engine::Internal::Components::CubeRenderer:
			{
				Engine::EngineObjects::CubeRenderer^ modelRenderer = (Engine::EngineObjects::CubeRenderer^)reference;
				deserializedData = ToJSON(modelRenderer);
			}
			break;

			case Engine::Internal::Components::GridRenderer:
			{
				Engine::EngineObjects::GridRenderer^ modelRenderer = (Engine::EngineObjects::GridRenderer^)reference;
				deserializedData = ToJSON(modelRenderer);
			}
			break;

			default:
			{
				Engine::EngineObjects::Script^ genericType = (Engine::EngineObjects::Script^)reference;
				deserializedData = ToJSON(genericType);
			}
			break;
			}
		}

	public:
		template <class type>
		type GetValue()
		{
			return (type)reference;
		}
	};
}
