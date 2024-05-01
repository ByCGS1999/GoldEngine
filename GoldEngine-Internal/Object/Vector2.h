#pragma once

namespace Engine::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
		public ref class Vector2
	{
	public:
		float x, y;

	public:
		[[JsonConstructorAttribute]]
		Vector2(float x, float y);

		::Vector2 toNative();
	};
}