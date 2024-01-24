#pragma once

namespace Engine::EngineObjects
{
	public ref class Daemon : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;

	public:
		Daemon(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Daemon)
		{
			assemblyReference = GetType()->FullName->ToString();
		}
	};
}