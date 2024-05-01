#pragma once

namespace Engine::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Quaternion
	{
	public:
		float x, y, z, w;

	public:
		[[JsonConstructorAttribute]]
		Quaternion(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

	public:
		Engine::Components::Vector3^ toEuler();
	};
}