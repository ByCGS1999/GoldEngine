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
	};

	public ref class Transform
	{
	public:
		Vector3^ position;
	public:
		Quaternion^ rotation;
	public:
		Vector3^ scale;

	public:
		Transform(Vector3 position, Quaternion rotation, Vector3 scale)
		{
			this->position = gcnew Vector3(position.x, position.y, position.z);
			this->rotation = gcnew Quaternion(rotation.x, rotation.y, rotation.z, rotation.w);
			this->scale = gcnew Vector3(scale.x, scale.y, scale.z);
		}
	};
}