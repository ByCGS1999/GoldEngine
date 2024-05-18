#pragma once
#include "EngineIncludes.h"

using namespace System;

namespace Engine::EngineObjects::Daemons
{
	public ref class LightDaemon : Engine::EngineObjects::Daemon
	{
	private:
		LightManager^ lightM;
		[JSON_SERIALIZE]
		unsigned int shaderId;

	public:
		LightDaemon(System::String^ name, Engine::Internal::Components::Transform^ transform, LightManager^ lightManager) : EngineObjects::Daemon(name, transform)
		{
			lightM = lightManager;
		}

		void Draw() override
		{
			if (lightM == nullptr)
			{
				printf("Lightmanager not set to an instance. Daemon won't work!");
				return;
			}

			Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetFirstObjectOfType<Engine::EngineObjects::Camera^>();

			if (camera == nullptr)
				return;

			float cameraPos[3] = { camera->GetTransform()->position->x, camera->GetTransform()->position->y, camera->GetTransform()->position->z };
			lightM->UpdateCameraPosition(cameraPos);
		}

		void Update() override
		{
			if (lightM == nullptr)
			{
				printf("Lightmanager not set to an instance. Daemon won't work!");
				return;
			}

			auto lights = Engine::Scripting::ObjectManager::singleton()->GetObjectsOfType(Engine::Internal::Components::ObjectType::LightSource);

			for each (auto light in lights)
			{
				if (light != nullptr)
				{
					Engine::EngineObjects::LightSource^ src = nullptr;
					try
					{
						src = (Engine::EngineObjects::LightSource^)light;
					}
					catch (Exception^ ex)
					{
						printError("Failed to cast <unknown-type> to GC_LightSource");
						throw ex;
					}

					if (src != nullptr)
					{
						if (!lightM->hasLight(src))
						{
							lightM->AddLight(src, src->shaderId);
						}
					}
				}
			}

			lightM->ShaderUpdate(shaderId);
			lightM->Update();
		}
	};
}