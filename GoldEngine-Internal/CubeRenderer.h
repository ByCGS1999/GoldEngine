#pragma once

namespace Engine::EngineObjects
{
	public ref class CubeRenderer : public Engine::Internal::Components::Object
	{
	public:
		unsigned long color;

	public:
		CubeRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned long tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::CubeRenderer)
		{
			this->color = tint;
		}

		void Init(unsigned long tint) override
		{
			this->color = tint;
		}

		void Draw() override
		{
			auto v3 = GetTransform()->scale->toNative();
			DrawCube(GetTransform()->position->toNative(), v3.x, v3.y, v3.z , GetColor(color));
		}
	};
}