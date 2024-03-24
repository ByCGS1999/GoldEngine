#pragma once

namespace Engine::EngineObjects
{
	public ref class Script : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;

	public:
		Script(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Script, this->tag)
		{
			assemblyReference = GetType()->FullName->ToString();
		}

	};
}