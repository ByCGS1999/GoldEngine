#pragma once

namespace Engine::EngineObjects
{
	public ref class GridRenderer : public Engine::Internal::Components::Object
	{
	public:
		int lines;
		float spacing;

	public:
		GridRenderer(String^ name, Engine::Internal::Components::Transform^ trans, int lines, float spacing) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::GridRenderer)
		{
			this->lines = lines;
			this->spacing = spacing;
		}

		void Init(int lines, float spacing) override
		{
			this->lines = lines;
			this->spacing = spacing;
		}

		void Draw() override
		{
			DrawGrid(lines, spacing);
		}
	};
}