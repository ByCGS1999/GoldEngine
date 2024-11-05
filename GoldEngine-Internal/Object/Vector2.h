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
		Vector2();

		RAYLIB::Vector2 toNative();

	public:
		static Vector2^ lerp(Vector2^ origin, Vector2^ target, float interpolate)
		{
			auto newX = RAYMATH::Lerp(origin->x, target->x, interpolate);
			auto newY = RAYMATH::Lerp(origin->y, target->y, interpolate);

			return gcnew Vector2(newX, newY);
		}

		static Vector2^ zero()
		{
			return gcnew Vector2(0, 0);
		}

		System::Numerics::Vector2^ toNumericsVector2()
		{
			return gcnew System::Numerics::Vector2(this->x, this->y);
		}

		static Vector2^ create(RAYLIB::Vector2 vector)
		{
			return gcnew Engine::Components::Vector2(vector.x, vector.y);
		}

		static Vector2^ create(float* vector)
		{
			return gcnew Engine::Components::Vector2(vector[0], vector[1]);
		}

		static Vector2^ New(float x, float y)
		{
			return gcnew Vector2(x, y);
		}

		bool Equals(Vector2^ value) override
		{
			return ((this->x == value->x) && (this->y == value->y));
		}

		static Vector2^ add(Vector2^ left, Vector2^ right)
		{
			return gcnew Vector2(left->x + right->x, left->y + right->y);
		}

		static Vector2^ sub(Vector2^ left, Vector2^ right)
		{
			return gcnew Vector2(left->x - right->x, left->y - right->y);
		}

		static Vector2^ multiply(Vector2^ left, Vector2^ right)
		{
			return gcnew Vector2(left->x * right->x, left->y * right->y);
		}

		static Vector2^ divide(Vector2^ left, Vector2^ right)
		{
			return gcnew Vector2(left->x / right->x, left->y / right->y);
		}

		Engine::Components::Vector2^ operator+(Vector2^ other)
		{
			return Vector2::add(this, other);
		}

		Engine::Components::Vector2^ operator-(Vector2^ other)
		{
			return Vector2::sub(this, other);
		}

		Engine::Components::Vector2^ operator*(Vector2^ other)
		{
			return Vector2::multiply(this, other);
		}

		Engine::Components::Vector2^ operator/(Vector2^ other)
		{
			return Vector2::divide(this, other);
		}
	};
}