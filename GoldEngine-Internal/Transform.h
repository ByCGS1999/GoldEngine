#pragma once
#include "Includes.h"

using namespace System;

namespace Engine::Internal::Components
{
	public ref class Vector3
	{
	public:
		float x, y, z;

	public:
		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		::Vector3 toNative()
		{
			::Vector3 rLib_v3;
			rLib_v3.x = this->x;
			rLib_v3.y = this->y;
			rLib_v3.z = this->z;

			return rLib_v3;
		}

		::Color toColor()
		{
			return GetColor(x + y + z);
		}
	};

	public ref class Vector2
	{
	public:
		float x, y;

	public:
		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}

		::Vector2 toNative()
		{
			return { x, y };
		}
	};

	public ref class Quaternion
	{
	public:
		float x, y, z, w;

	public:
		Quaternion(float x, float y, float z, float w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

	public:
		Engine::Internal::Components::Vector3^ toEuler()
		{
			return gcnew Engine::Internal::Components::Vector3(x, y, z);
		}
	};

	public ref class Transform
	{
	public:
		Vector3^ position;
		Quaternion^ rotation;
		float scale;
		Transform(Vector3^ position, Quaternion^ rotation, float scale);
	};
}