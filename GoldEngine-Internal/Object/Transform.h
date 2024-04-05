#pragma once
#include "../Includes.h"
#include "../GlIncludes.h"
#include "../CastToNative.h"
#include "ObjectType.h"
#include "ViewSpace.h"
#include "../Cast.h"
#include "Layer.h"
#include "Transform.h"
#include "LayerManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

using namespace System;

namespace Engine::Internal::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
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
		// CONSTANT VECTORS
		const Engine::Internal::Components::Vector3^ forward = gcnew Vector3(0, 0, 1);
		const Engine::Internal::Components::Vector3^ top = gcnew Vector3(0, 1, 0);
		const Engine::Internal::Components::Vector3^ right = gcnew Vector3(1, 0, 0);
		const Engine::Internal::Components::Vector3^ backward = gcnew Vector3(0, 0, -1);
		const Engine::Internal::Components::Vector3^ down = gcnew Vector3(0, -1, 0);
		const Engine::Internal::Components::Vector3^ left = gcnew Vector3(-1, 0, 0);

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

		Object^ getGameObject()
		{
			return this->gameObject;
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

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Object : IDisposable
	{
	public:
		String^ name;
		ObjectType type;
		Transform^ transform;
		ViewSpace viewSpace;
		Layer^ layerMask;
		String^ tag;

		[[JsonConstructorAttribute]]
		Object(System::String^ n, Transform^ transform, ObjectType t, String^ tag)
		{
			this->name = n;
			this->transform = transform;
			this->type = t;
			this->viewSpace = ViewSpace::V3D;
			layerMask = LayerManager::GetLayerFromId(0);

			if (tag == nullptr)
				tag = "";
			
			this->tag = tag;
		}

	private:
		~Object()
		{
			delete name;
			delete transform;
		}

	public:
		// vmethods
		// init functions (used by reflector & scene loader).
		virtual void Init(Object^ object) {}
		virtual void Init() {}
		virtual void Init(array<System::Object^>^ params) {}
		virtual void Init(System::Object^ object) {}

		// object methods
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() {}
		virtual void Draw() {}
		virtual void DrawGizmo() {}
		virtual void DrawImGUI() {}
		// defined

		String^ GetTag() { return tag; }

		void SetTag(String^ tag) { this->tag = tag; }

		Transform^ GetTransform() { return transform; }

		void SetParent(Object^ object)
		{
			transform->SetParent(object->transform);
		}

		generic <class T>
		T ToObjectType()
		{
			return Cast::Dynamic<T>(this);
		}

		virtual void Destroy()
		{
			delete this;
		}

		/*

#pragma region IConvertible_Implementation

		virtual System::TypeCode GetTypeCode()
		{
			return System::TypeCode();
		}

		virtual bool ToBoolean(System::IFormatProvider^ provider)
		{
			return false;
		}

		virtual wchar_t ToChar(System::IFormatProvider^ provider)
		{
			return L'\0';
		}

		virtual signed char ToSByte(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual unsigned char ToByte(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual short ToInt16(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual unsigned short ToUInt16(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual int ToInt32(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual unsigned int ToUInt32(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual long long ToInt64(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual unsigned long long ToUInt64(System::IFormatProvider^ provider)
		{
			return 0;
		}

		virtual float ToSingle(System::IFormatProvider^ provider)
		{
			return 0.0f;
		}

		virtual double ToDouble(System::IFormatProvider^ provider)
		{
			return 0.0;
		}

		virtual System::Decimal ToDecimal(System::IFormatProvider^ provider)
		{
			return System::Decimal();
		}

		virtual System::DateTime ToDateTime(System::IFormatProvider^ provider)
		{
			return System::DateTime();
		}

		virtual System::String^ ToString(System::IFormatProvider^ provider)
		{
			return "Script";
		}

		virtual System::Object^ ToType(System::Type^ conversionType, System::IFormatProvider^ provider)
		{
			return System::Convert::ChangeType(this, conversionType);
		}

#pragma endregion

*/

	};
}