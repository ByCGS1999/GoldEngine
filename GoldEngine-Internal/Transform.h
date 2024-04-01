#pragma once
#include "Includes.h"
#include "GlIncludes.h"
#include "Cast.h"
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
		Script = 9,
		BoundingBoxRenderer = 10,
		Daemon = 11
	};

	public enum class ViewSpace
	{
		V2D,
		V3D,
		VNone
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
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

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Layer
	{
	public:
		int layerMask; // this int represents the priority on rendering (higher number = higher priority);
		String^ layerName;

	public:
		Layer(int mask, String^ name)
		{
			this->layerMask = mask;
			this->layerName = name;
		}
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class LayerManager
	{
	private:
		static System::Collections::Generic::List<Engine::Internal::Components::Layer^>^ layers;

	public:
		static void RegisterDefaultLayers()
		{
			layers = gcnew System::Collections::Generic::List<Layer^>();

			AddLayer(gcnew Engine::Internal::Components::Layer(0, "Geometry"));
			AddLayer(gcnew Engine::Internal::Components::Layer(1, "Transparent"));
			AddLayer(gcnew Engine::Internal::Components::Layer(2, "PostFX"));
		}

	public:
		static std::vector<std::string> getLayerNames()
		{
			std::vector<std::string> slayers;

			for (int x = 0; x < layers->Count; x++)
			{
				slayers.push_back(CastStringToNative(layers[x]->layerMask + " - " + layers[x]->layerName));
			}

			return slayers;
		}
		

	public:
		static Engine::Internal::Components::Layer^ GetLayerFromId(int id)
		{
			for each(Engine::Internal::Components::Layer^ l in layers)
			{
				if (l->layerMask == id)
				{
					return l;
				}
			}

			return nullptr;
		}

	public:
		static Engine::Internal::Components::Layer^ GetLayerFromName(String^ id)
		{
			for each (Engine::Internal::Components::Layer ^ l in layers)
			{
				if (l->layerName->Equals(id) || l->layerName->CompareTo(id) <= 0 || l->layerName == id)
				{
					return l;
				}
			}

			return nullptr;
		}


	public:
		static void AddLayer(Engine::Internal::Components::Layer^ layer)
		{
			layers->Add(layer);
		}
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
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

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
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