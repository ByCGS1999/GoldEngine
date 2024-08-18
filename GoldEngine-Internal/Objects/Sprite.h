#pragma once

namespace Engine::EngineObjects
{
	public ref class Sprite : Engine::EngineObjects::ScriptBehaviour
	{
	public:
		Sprite(String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::ScriptBehaviour(name, transform)
		{
			if (!attributes->getAttribute("Texture ID"))
			{
				attributes->addAttribute(Engine::Scripting::Attribute::create(Engine::Scripting::Attribute::AccessLevel::Public, "Texture ID", 0, System::UInt32::typeid));
			}

			if (!attributes->getAttribute("Sprite Tint"))
			{
				attributes->addAttribute(Engine::Scripting::Attribute::create(Engine::Scripting::Attribute::AccessLevel::Public, "Sprite Tint", gcnew Engine::Components::Color(0xFFFFFFFF)));
			}
		}

	public:
		void Start() override
		{
			if (!attributes->getAttribute("Texture ID"))
			{
				attributes->addAttribute(Engine::Scripting::Attribute::create(Engine::Scripting::Attribute::AccessLevel::Public, "Texture ID", 0, System::UInt32::typeid));
			}

			if (!attributes->getAttribute("Sprite Tint"))
			{
				attributes->addAttribute(Engine::Scripting::Attribute::create(Engine::Scripting::Attribute::AccessLevel::Public, "Sprite Tint", gcnew Engine::Components::Color(0xFFFFFFFF)));
			}
		}

		void Update() override
		{

		}

		void Draw() override
		{
			Engine::Components::Vector2^ transformedVector = this->transform->position->toVector2();

			DrawTexture(
				DataPacks::singleton().GetTexture2D(
					(unsigned int)attributes->getAttribute("Texture ID")->getValueAuto()
				), 
				(int)transformedVector->x, 
				(int)transformedVector->y, 
				attributes->getAttribute("Sprite Tint")->getValue<Engine::Components::Color^>()->toNative()
			);
		}

		void Destroy() override
		{
			delete this;
		}
	};

}