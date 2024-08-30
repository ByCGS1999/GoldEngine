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
#include "../LoggingAPI.h"

using namespace System;

namespace Engine::Internal::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Transform
	{
	private:
		[Newtonsoft::Json::JsonPropertyAttribute]
		System::String^ uid;
	public:
		Transform^ parent;
		// worldspace
		Engine::Components::Vector3^ position;
		Engine::Components::Vector3^ rotation;
		float rotationValue;
		Engine::Components::Vector3^ scale;
		// localspace
		Engine::Components::Vector3^ localPosition;
		Engine::Components::Vector3^ localRotation;
		// CONSTANT VECTORS
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ forward = gcnew Engine::Components::Vector3(0, 0, 1);
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ top = gcnew Engine::Components::Vector3(0, 1, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ right = gcnew Engine::Components::Vector3(1, 0, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ backward = gcnew Engine::Components::Vector3(0, 0, -1);
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ bottom = gcnew Engine::Components::Vector3(0, -1, 0);
		[Newtonsoft::Json::JsonIgnoreAttribute]
		const Engine::Components::Vector3^ left = gcnew Engine::Components::Vector3(-1, 0, 0);

	private:
		Object^ gameObject;

	public:
		Transform(Engine::Components::Vector3^ position, Engine::Components::Vector3^ rotation, float rotationValue, Engine::Components::Vector3^ scale, Transform^ parent)
		{
			this->uid = System::Guid::NewGuid().ToString();
			this->localPosition = position;
			this->localRotation = rotation;
			this->scale = scale;
			this->rotationValue = rotationValue;
			this->gameObject = gameObject;

			if (parent != nullptr)
			{
				this->parent = parent;
				this->position = Engine::Components::Vector3::add(parent->position, this->localPosition);
				this->rotation = Engine::Components::Vector3::add(parent->rotation, this->localRotation);

				this->localPosition = Engine::Components::Vector3::sub(parent->position, this->position);
			}
			else
			{
				this->position = this->localPosition;
				this->rotation = this->localRotation;
			}
		}

	public:
		void UpdateLocalPosition(Engine::Components::Vector3^ newLocalPosition)
		{
			position = localPosition - newLocalPosition;
		}

		Object^ getGameObject()
		{
			return this->gameObject;
		}

		void SetReference(Object^ gameObject)
		{
			this->gameObject = gameObject;
		}

		String^ GetUID() { return uid; }

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
		bool active;

		String^ name;
		initonly ObjectType type;
		Transform^ transform;
		ViewSpace viewSpace;
		Layer^ layerMask;
		String^ tag;

	private:
		bool memberIsProtected;

	public:

		[[JsonConstructorAttribute]]
		Object(System::String^ n, Transform^ transform, ObjectType t, String^ tag, Layer^ layer)
		{
			this->active = true;
			this->memberIsProtected = false;
			this->name = n;
			this->transform = transform;
			this->type = t;
			this->viewSpace = ViewSpace::V3D;
			layerMask = layer;

			if (tag == nullptr)
				tag = "";

			if(this->transform != nullptr)
				this->transform->SetReference(this);

			this->tag = tag;
		}

	private:
		~Object()
		{
			delete name;
			delete transform;
			delete layerMask;
			delete tag;
		}

	public:
		// vmethods
		// init functions (used by reflector & scene loader).
		virtual void Init(Object^ object) {}
		virtual void Init() {}
		virtual void Init(array<System::Object^>^ params) {}
		virtual void Init(System::Object^ object) {}
		virtual void Setup() {}

		// object methods
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() { }
		virtual void Draw() {}
		virtual void DrawGizmo() {}
		virtual void DrawImGUI() {}

		// engine methods

	private:
		void UpdateLocalPosition()
		{
			if (transform->parent != nullptr)
			{
				Engine::Components::Vector3^ diff = (transform->parent->position - transform->position);
				transform->localPosition = transform->position + diff;
			}
		}

		void UpdatePosition()
		{
			if (transform->parent != nullptr)
			{
				Engine::Components::Vector3^ diff = (transform->parent->position - transform->position);
				transform->position = transform->localPosition - diff;
			}
		}

	public:
		void GameUpdate()
		{
			UpdateLocalPosition();
			UpdatePosition();

			if (!active)
				return;

			Update();
		}

	public:
		void GameDraw()
		{
			if (!active)
				return;

			Draw();
		}

	public:
		void GameDrawGizmos()
		{
			if (!active)
				return;

			DrawGizmo();
		}

	public:
		void GameDrawImGUI()
		{
			if (!active)
				return;

			DrawImGUI();
		}

		// defined
	public:
		bool isProtected()
		{
			return memberIsProtected;
		}

		void protectMember()
		{
			memberIsProtected = true;
		}

		void unprotectMember()
		{
			memberIsProtected = false;
		}

		String^ GetTag() { return tag; }

		void SetTag(String^ tag) { this->tag = tag; }

		Transform^ GetTransform() { return transform; }

		void SetParent(Object^ object)
		{
			if (object == nullptr)
				return;

			transform->SetParent(object->transform);
		}

		generic <class T>
		T ToObjectType()
		{
			try
			{
				return Cast::Dynamic<T>(this);
			}
			catch(Exception^ ex)
			{
				printError(ex->Message);
			}
		}

		virtual void Destroy()
		{
			delete this;
		}

		auto ToDerivate()
		{
			return Convert::ChangeType(this, this->GetType());
		}

		System::Type^ GetRuntimeType()
		{
			return this->GetType();
		}

		void SetActive(bool active)
		{
			this->active = active;
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