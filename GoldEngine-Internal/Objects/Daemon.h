#pragma once

namespace Engine::EngineObjects
{
	public ref class Daemon : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;
		Engine::Scripting::AttributeManager^ attributes;


	public:
		Daemon(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Daemon, this->tag)
		{
			assemblyReference = GetType()->FullName->ToString();
		}
	};
}