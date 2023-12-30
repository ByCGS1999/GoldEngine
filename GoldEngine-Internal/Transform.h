#pragma once
#include "Includes.h"

using namespace System;

namespace Engine::Internal::Components
{
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

		Engine::Internal::Components::Vector2^ toVector2()
		{
			return gcnew Engine::Internal::Components::Vector2(x, y);
		}
		
		::Vector3 toNative()
		{
			return { x,y,z };
		}

		::Color toColor()
		{
			return GetColor(x + y + z);
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
		Transform^ parent;
		// worldspace
		Vector3^ position;
		Vector3^ rotation;
		float scale;
		// localspace
		Vector3^ localPosition;
		Vector3^ localRotation;

		Transform(Vector3^ position, Vector3^ rotation, float scale, Transform^ parent)
		{
			this->localPosition = position;
			this->localRotation = rotation;
			this->scale = scale;

			if (parent != nullptr)
			{
				this->parent = parent;
				this->position = Vector3::Sub(parent->position, this->localPosition);
				this->rotation = Vector3::Sub(parent->rotation, this->localRotation);
			}
			else
			{
				this->position = this->localPosition;
				this->rotation = this->localRotation;
			}
		}


	};
}