#pragma once
#include "EngineIncludes.h"

using namespace System;

namespace Engine::EngineObjects::Daemons
{
	public ref class LightDaemon : Engine::EngineObjects::Daemon
	{
	private:
		LightManager^ lightM;

	public:
		LightDaemon(System::String^ name, Engine::Internal::Components::Transform^ transform, LightManager^ lightManager) : EngineObjects::Daemon(name, transform)
		{
			lightM = lightManager;
			attributes = gcnew Engine::Scripting::AttributeManager();

			attributes->setAttribute(gcnew Engine::Scripting::Attribute("shaderId", gcnew System::UInt32(0)));
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
						TraceLog(LOG_FATAL, "Failed to cast <unknown-type> to LightSource^");
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

			lightM->ShaderUpdate((unsigned int)attributes->getAttribute("shaderId")->getValue());
			lightM->Update();
		}
	};
}