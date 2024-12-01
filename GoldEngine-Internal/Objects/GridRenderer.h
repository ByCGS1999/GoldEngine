#pragma once

namespace Engine::EngineObjects
{
	public ref class GridRenderer : public Engine::Internal::Components::GameObject
	{
	public:
		int lines;
		float spacing;

	public:
		GridRenderer(System::String^ name, Engine::Internal::Components::Transform^ trans, int lines, float spacing) : Engine::Internal::Components::GameObject(name, trans, Engine::Internal::Components::ObjectType::GridRenderer, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			this->lines = lines;
			this->spacing = spacing;
		}

		void Init(int lines, float spacing) override
		{
			this->lines = lines;
			this->spacing = spacing;
		}

		void DrawGuizmos() override
		{
			DrawGrid(lines, spacing);
		}
	};
}