#pragma once

namespace Engine::EngineObjects
{
	public ref class Script : public Engine::EngineObjects::ScriptBehaviour
	{
	public:
		Script(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::ScriptBehaviour(name, transform)
		{
			for each (Engine::Scripting::Attribute ^ attrib in attributes->attributes)
			{
				Singleton<ObjectManager^>::Instance->PatchAttributeObject(attrib);
			}
		}

		virtual void Init() override
		{
			Engine::EngineObjects::ScriptBehaviour::Init();

			for each (Engine::Scripting::Attribute ^ attrib in attributes->attributes)
			{
				Singleton<ObjectManager^>::Instance->PatchAttributeObject(attrib);
			}

			GC::Collect();
		}

		virtual void Setup() override
		{
			Engine::EngineObjects::ScriptBehaviour::Setup();

			for each (Engine::Scripting::Attribute ^ attrib in attributes->attributes)
			{
				Singleton<ObjectManager^>::Instance->PatchAttributeObject(attrib);
			}

			GC::Collect();
		}
	};
}