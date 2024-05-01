#pragma once

namespace Engine::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Layer
	{
	public:
		int layerMask; // this int represents the priority on rendering (higher number = higher priority);
		System::String^ layerName;

	public:
		Layer(int mask, System::String^ name);
	};
}
