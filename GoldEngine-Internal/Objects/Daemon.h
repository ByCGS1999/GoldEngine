#pragma once

namespace Engine::EngineObjects
{
	public ref class Daemon : public Engine::Internal::Components::GameObject
	{
	public:
		System::String^ assemblyReference;
		Engine::Scripting::AttributeManager^ attributes;


	public:
		Daemon(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::GameObject(name, transform, Engine::Internal::Components::ObjectType::Daemon, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			assemblyReference = GetType()->FullName->ToString();
		}
	};
}