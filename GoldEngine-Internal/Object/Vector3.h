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
		Vector3(float, float, float);
		Vector3();

		Engine::Components::Vector2^ toVector2()
		{
			return gcnew Engine::Components::Vector2(x, y);
		}

		System::Numerics::Vector3^ toNumericsVector3()
		{
			return gcnew System::Numerics::Vector3(this->x, this->y, this->z);
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

		static Vector3^ New(float x, float y, float z)
		{
			return gcnew Vector3(x, y, z);
		}

		static Vector3^ create(float* vec)
		{
			return gcnew Vector3(vec[0], vec[1], vec[2]);
		}

		static float Dot(Vector3^ left, Vector3^ right)
		{
			return (Vector3DotProduct(left->toNative(), right->toNative()));
		}

		static float Distance(Vector3^ left, Vector3^ right)
		{
			return Vector3Distance(left->toNative(), right->toNative());
		}

		bool Equals(Vector3^ value) override
		{
			return ((this->x == value->x) && (this->y == value->y) && (this->z == value->z));
		}

		Engine::Components::Vector3^ operator+(Vector3^ other)
		{
			return Vector3::add(this, other);
		}

		Engine::Components::Vector3^ operator-(Vector3^ other)
		{
			return Vector3::sub(this, other);
		}

		Engine::Components::Vector3^ operator*(Vector3^ other)
		{
			return Vector3::multiply(this, other);
		}

		Engine::Components::Vector3^ operator/(Vector3^ other)
		{
			return Vector3::divide(this, other);
		}
	};

}