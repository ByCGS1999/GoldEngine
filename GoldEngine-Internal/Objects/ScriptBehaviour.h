#pragma once

namespace Engine::EngineObjects
{
	public ref class ScriptBehaviour : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;
		Engine::Scripting::AttributeManager^ attributes;

	public:
		ScriptBehaviour(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Script, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			attributes = gcnew Engine::Scripting::AttributeManager();
			assemblyReference = GetType()->FullName->ToString();

			attributes->DeserializeAttributes();
		}

		virtual void Init() override
		{
			attributes->DeserializeAttributes();
		}

		virtual void Setup() override
		{
			attributes->DeserializeAttributes();
		}
	};
}