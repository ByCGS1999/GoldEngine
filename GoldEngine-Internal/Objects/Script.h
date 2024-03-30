#pragma once

namespace Engine::EngineObjects
{
	public ref class Script : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;
	public:
		Engine::Scripting::AttributeManager^ attributes;

	public:
		Script(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Script, this->tag)
		{
			attributes = gcnew Engine::Scripting::AttributeManager();
			assemblyReference = GetType()->FullName->ToString();
		}

	};
}