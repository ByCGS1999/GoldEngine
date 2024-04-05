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
		System::String^ deserializedData;
	private:
		Engine::Internal::Components::Object^ reference;

	public:
		SceneObject(Engine::Internal::Components::ObjectType type, Engine::Internal::Components::Object^ obj, System::String^ data)
		{
			objectType = type;
			reference = obj;
			deserializedData = data;

			if (obj == nullptr && deserializedData != "" && deserializedData != nullptr)
			{
				serialize();
			}
			else if (obj != nullptr && deserializedData == "")
			{
				deserialize();
			}
		}

		Engine::Internal::Components::Object^ GetReference()
		{
			return reference;
		}

		void SetReference(Engine::Internal::Components::Object^ ref)
		{
			this->reference = ref;
		}

	public:
		generic <class T>
		T serializeAs()
		{
			return Newtonsoft::Json::JsonConvert::DeserializeObject<T>(deserializedData);
		}

	public:
		void serialize()
		{
			switch ((Engine::Internal::Components::ObjectType)objectType)
			{
			case Engine::Internal::Components::ObjectType::Skybox:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Skybox^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::Generic:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Internal::Components::Object^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::Datamodel:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::Internal::Components::Object^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::ModelRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::ModelRenderer^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::CubeRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::CubeRenderer^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::GridRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::GridRenderer^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::PBR_ModelRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::PBRModelRenderer^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::LightManager:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::LightManager^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::LightSource:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::LightSource^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::Script:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Script^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::BoundingBoxRenderer:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::BoundingBoxRenderer^>(deserializedData);
				break;
			}

			case Engine::Internal::Components::ObjectType::Daemon:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Daemon^>(deserializedData);
				break;
			}

			default:
			{
				reference = Newtonsoft::Json::JsonConvert::DeserializeObject<Engine::EngineObjects::Script^>(deserializedData);
				break;
			}
			}
		}
		void deserialize()
		{
			switch (objectType)
			{
			case Engine::Internal::Components::ObjectType::Skybox:
			{
				Engine::EngineObjects::Skybox^ skybox = (Engine::EngineObjects::Skybox^)reference;
				deserializedData = Serialize(skybox);
				break;
			}

			case Engine::Internal::Components::ObjectType::Generic:
			{
				Engine::Internal::Components::Object^ genericType = (Engine::Internal::Components::Object^)reference;
				deserializedData = Serialize(genericType);
				break;
			}

			case Engine::Internal::Components::ObjectType::ModelRenderer:
			{
				Engine::EngineObjects::ModelRenderer^ modelRenderer = (Engine::EngineObjects::ModelRenderer^)reference;
				deserializedData = Serialize(modelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::Datamodel:
			{
				Engine::Internal::Components::Object^ genericType = (Engine::Internal::Components::Object^)reference;
				deserializedData = Serialize(genericType);
				break;
			}

			case Engine::Internal::Components::ObjectType::CubeRenderer:
			{
				Engine::EngineObjects::CubeRenderer^ modelRenderer = (Engine::EngineObjects::CubeRenderer^)reference;
				deserializedData = Serialize(modelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::GridRenderer:
			{
				Engine::EngineObjects::GridRenderer^ modelRenderer = (Engine::EngineObjects::GridRenderer^)reference;
				deserializedData = Serialize(modelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::PBR_ModelRenderer:
			{
				Engine::EngineObjects::PBRModelRenderer^ pbrModelRenderer = (Engine::EngineObjects::PBRModelRenderer^)reference;
				deserializedData = Serialize(pbrModelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::LightManager:
			{
				Engine::EngineObjects::LightManager^ pbrModelRenderer = (Engine::EngineObjects::LightManager^)reference;
				deserializedData = Serialize(pbrModelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::LightSource:
			{
				Engine::EngineObjects::LightSource^ pbrModelRenderer = (Engine::EngineObjects::LightSource^)reference;
				deserializedData = Serialize(pbrModelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::BoundingBoxRenderer:
			{
				Engine::EngineObjects::BoundingBoxRenderer^ pbrModelRenderer = (Engine::EngineObjects::BoundingBoxRenderer^)reference;
				deserializedData = Serialize(pbrModelRenderer);
				break;
			}

			case Engine::Internal::Components::ObjectType::Daemon:
			{
				Engine::EngineObjects::Daemon^ pbrModelRenderer = (Engine::EngineObjects::Daemon^)reference;
				deserializedData = Serialize(pbrModelRenderer);
				break;
			}

			default:
			{
				Engine::EngineObjects::Script^ genericType = (Engine::EngineObjects::Script^)reference;
				deserializedData = Serialize(genericType);
				break;
			}
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
