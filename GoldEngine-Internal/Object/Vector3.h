#pragma once

namespace Engine::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Vector3
	{
	public:
		float x, y, z;

	public:
		[[JsonConstructorAttribute]]
		Vector3(float x, float y, float z);

		Engine::Components::Vector2^ toVector2()
		{
			return gcnew Engine::Components::Vector2(x, y);
		}

		void Set(float x, float y, float z);

		void Set(float* v);

		RAYLIB::Vector3 toNative();

		RAYLIB::Color toColor();

		void add(float x, float y, float z);

		void add(Vector3^ origin);

		void multiply(Vector3^ origin);

		void multiply(float x, float y, float z);

		void divide(Vector3^ origin);

		void divide(float x, float y, float z);

		void sub(float x, float y, float z);

		void sub(Vector3^ origin);

		void copy(const Vector3^ inVec);

		static Vector3^ lerp(Vector3^ origin, Vector3^ target, float interpolate)
		{
			auto newX = ::Lerp(origin->x, target->x, interpolate);
			auto newY = ::Lerp(origin->y, target->y, interpolate);
			auto newZ = ::Lerp(origin->z, target->z, interpolate);

			return gcnew Vector3(newX, newY, newZ);
		}

		static Vector3^ zero()
		{
			return gcnew Vector3(0, 0, 0);
		}

		static Vector3^ add(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x + right->x, left->y + right->y, left->z + right->z);
		}

		static Vector3^ sub(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x - right->x, left->y - right->y, left->z - right->z);
		}

		static Vector3^ multiply(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x * right->x, left->y * right->y, left->z * right->z);
		}

		static Vector3^ divide(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x / right->x, left->y / right->y, left->z / right->z);
		}

		static Vector3^ create(RAYLIB::Vector3 vec)
		{
			return gcnew Vector3(vec.x, vec.y, vec.z);
		}
	};
}