#pragma once

namespace Engine::EngineObjects
{
	public ref class CubeRenderer : public Engine::Internal::Components::Object
	{
	public:
		unsigned int color;

	public:
		CubeRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::CubeRenderer, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			this->color = tint;
		}

		void Init(unsigned int tint) override
		{
			this->color = tint;
		}

		void Draw() override
		{
			auto v3 = GetTransform()->scale->toNative();
			RAYLIB::Color c =
			{
				color >> 0,
				color >> 8,
				color >> 16,
				color >> 24
			};
			
			DrawCube(GetTransform()->position->toNative(), v3.x, v3.y, v3.z , c);
		}

	public:
		void SetColor(unsigned int color)
		{
			this->color = color;
		}
	};
}