#pragma once

namespace Engine::Internal::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
		public ref class Vector3
	{
	public:
		float x, y, z;

	public:
		Vector3(float x, float y, float z);

		Engine::Internal::Components::Vector2^ toVector2()
		{
			return gcnew Engine::Internal::Components::Vector2(x, y);
		}

		void Set(float x, float y, float z);

		void Set(float* v);

		::Vector3 toNative()
		{
			return { x,y,z };
		}

		::Color toColor()
		{
			return GetColor(x + y + z);
		}

		void Add(float x, float y, float z)
		{
			this->x += x;
			this->y += y;
			this->z += z;
		}

		void Add(Vector3^ origin)
		{
			this->x += origin->x;
			this->y += origin->y;
			this->z += origin->z;
		}

		void Multiply(Vector3^ origin)
		{
			this->x *= origin->x;
			this->y *= origin->y;
			this->z *= origin->z;
		}

		void Multiply(float x, float y, float z)
		{
			this->x *= x;
			this->y *= y;
			this->z *= z;
		}

		void Divide(Vector3^ origin)
		{
			this->x /= origin->x;
			this->y /= origin->y;
			this->z /= origin->z;
		}

		void Divide(float x, float y, float z)
		{
			this->x /= x;
			this->y /= y;
			this->z /= z;
		}

		void Sub(float x, float y, float z)
		{
			this->x -= x;
			this->y -= y;
			this->z -= z;
		}

		void Sub(Vector3^ origin)
		{
			this->x -= origin->x;
			this->y -= origin->y;
			this->z -= origin->z;
		}

		static Vector3^ Lerp(Vector3^ origin, Vector3^ target, float interpolate)
		{
			auto newX = ::Lerp(origin->x, target->x, interpolate);
			auto newY = ::Lerp(origin->y, target->y, interpolate);
			auto newZ = ::Lerp(origin->z, target->z, interpolate);

			return gcnew Vector3(newX, newY, newZ);
		}

		static Vector3^ Add(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x + right->x, left->y + right->y, left->z + right->z);
		}

		static Vector3^ Sub(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x - right->x, left->y - right->y, left->z - right->z);
		}

		static Vector3^ Multiply(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x * right->x, left->y * right->y, left->z * right->z);
		}

		static Vector3^ Divide(Vector3^ left, Vector3^ right)
		{
			return gcnew Vector3(left->x / right->x, left->y / right->y, left->z / right->z);
		}
	};
}