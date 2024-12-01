#pragma once

namespace Engine::Internal::Components
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
		public ref class GameObject
	{
	private:
		void* collisionShape;

	public:
		bool active;

		System::String^ name;
		initonly Engine::Internal::Components::ObjectType type;
		Engine::Internal::Components::Transform^ transform;
		Engine::Internal::Components::ViewSpace viewSpace;
		Engine::Components::Layer^ layerMask;
		System::String^ tag;

		[Newtonsoft::Json::JsonIgnoreAttribute]
			Engine::Scripting::Events::Event^ onPropertyChanged;
		[Newtonsoft::Json::JsonIgnoreAttribute]
			Engine::Scripting::Events::Event^ onChildAdded;
		[Newtonsoft::Json::JsonIgnoreAttribute]
			Engine::Scripting::Events::Event^ onChildRemoved;
		[Newtonsoft::Json::JsonIgnoreAttribute]
			Engine::Scripting::Events::Event^ onDescendantAdded;

	private:
		bool memberIsProtected;
		Engine::Internal::Components::Transform^ lastTransform;

	public:
		[Newtonsoft::Json::JsonConstructorAttribute]
			GameObject(System::String^ n, Engine::Internal::Components::Transform^ transform, Engine::Internal::Components::ObjectType t, String^ tag, Engine::Components::Layer^ layer);

	private:
		~GameObject()
		{
			delete collisionShape;
			delete name;
			delete transform;
			delete layerMask;
			delete tag;
		}

	public:
		void* getCollisionShape() { return collisionShape; }

	public:
		// vmethods
		// init functions (used by reflector & scene loader).
		virtual void Init(GameObject^ object) {}
		virtual void Init() {}
		virtual void Init(array<System::Object^>^ params) {}
		virtual void Init(System::Object^ object) {}
		virtual void Setup() {}

		// object methods
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() { }
		virtual void Draw() {}
		virtual void DrawGUI() {}
		virtual void DrawGizmo() {}
		virtual void DrawImGUI() {}

		virtual void OnActive() {}
		virtual void OnUnactive() {}

		virtual void OnCollisionEnter(GameObject^ other) {}

		// internal methods
	protected:
		virtual void HookUpdate() {}

		// engine methods

	private:
		void descendantAdded(GameObject^ descendant);
		void OnPropChanged();

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
		void GameUpdate();
		void GameDraw();
		void GameDrawGizmos();
		void GameDrawImGUI();

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

		String^ getTag();
		void setTag(String^);
		Transform^ getTransform();
		void setParent(GameObject^ object);

		String^ GetTag() { return getTag(); }

		void SetTag(String^ arg) { this->setTag(arg); }
		Transform^ GetTransform() { return getTransform(); }
		void SetParent(GameObject^ arg) { this->setParent(arg); }

		generic <class T>
		T ToObjectType();

		virtual void Destroy()
		{
			delete this;
		}

		auto ToDerivate()
		{
			return System::Convert::ChangeType(this, this->GetType());
		}

		System::Type^ GetRuntimeType()
		{
			return this->GetType();
		}

		void SetActive(bool active)
		{
			this->active = active;
		}

		void SetLayerMask(Engine::Components::Layer^ layerMask)
		{
			this->layerMask = layerMask;
		}

		System::Collections::Generic::List<GameObject^>^ GetChildren();
		GameObject^ GetChild(int index);
		GameObject^ GetChild(String^ name);

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