#pragma once
#include "Includes.h"
#include "GlIncludes.h"
#include "Transform.h"

using namespace System;

namespace Engine::Internal::Components
{
	public enum class ObjectType
	{
		Generic = 0,
		Datamodel = 1,
		Skybox = 2,
		ModelRenderer = 3,
		GridRenderer = 4,
		CubeRenderer = 5,
		LightManager = 6,
		LightSource = 7,
		PBR_ModelRenderer = 8,
		Script = 9
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
		
		void Set(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		void Set(float* v)
		{
			this->x = v[0];
			this->y = v[1];
			this->z = v[2];
		}

		::Vector3 toNative()
		{
			return { x,y,z };
		}

		::Color toColor()
		{
			return GetColor(x + y + z);
		}

		void Add(int x, int y, int z)
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

		void Sub(int x, int y, int z)
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
		System::String^ uid;
		String^ name;
		Transform^ parent;
		// worldspace
		Vector3^ position;
		Vector3^ rotation;
		float rotationValue;
		Vector3^ scale;
		// localspace
		Vector3^ localPosition;
		Vector3^ localRotation;

	private:
		Object^ gameObject;

	public:
		Transform(Vector3^ position, Vector3^ rotation, float rotationValue, Vector3^ scale, Transform^ parent)
		{
			this->uid = System::Guid::NewGuid().ToString();
			this->name = "";
			this->localPosition = position;
			this->localRotation = rotation;
			this->scale = scale;
			this->rotationValue = rotationValue;
			this->gameObject = gameObject;

			if (parent != nullptr)
			{
				this->parent = parent;
				this->position = Vector3::Add(parent->position, this->localPosition);
				this->rotation = Vector3::Add(parent->rotation, this->localRotation);
			}
			else
			{
				this->position = this->localPosition;
				this->rotation = this->localRotation;
			}
		}

		void SetReference(Object^ gameObject)
		{
			this->gameObject = gameObject;
		}

		void SetName(String^ name)
		{
			this->name = name;
		}

		void SetParent(Transform^ newTransform)
		{
			this->parent = newTransform;
		}

		Transform^ GetParent()
		{
			return parent;
		}

		generic <class T>
		T GetObject()
		{
			return (T)gameObject;
		}
	};

	public ref class Object
	{
	public:
		String^ name;
		ObjectType type;
		Transform^ transform;

		[[JsonConstructorAttribute]]
		Object(System::String^ n, Transform^ transform, ObjectType t)
		{
			this->name = n;
			this->transform = transform;
			this->type = t;
		}
		virtual void Init() {}
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() {}
		virtual void Draw() {}
		virtual void DrawGizmo() {}
		Transform^ GetTransform() { return transform; }
		void SetParent(Object^ object)
		{
			transform->SetParent(object->transform);
		}
	};
}