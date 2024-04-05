
namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public:
		String^ name;
		System::Object^ userData;
		System::String^ type;

	public:
		Attribute(String^ str, System::Object^ data, String^ dataType)
		{
			this->name = str;
			userData = data;
			type = dataType;
		}

	public:
		void setValue(System::Object^ object)
		{
			userData = object;
			type = object->GetType()->Name;
		}

		void setValue(System::Object^ object, bool overrideType)
		{
			userData = object;

			if(overrideType)
				type = object->GetType()->Name;
		}

	public:
		generic <class T>
		T getValue()
		{
			return (T)userData;
		}

		System::Object^ getValue()
		{
			return userData;
		}

		auto getValueAuto()
		{
			Type^ targetType = System::Type::GetType(type);

			return Convert::ChangeType(userData, targetType);
		}


	public:
		static Attribute^ create(String^ name, System::Object^ data, Type^ type)
		{
			return New(name, data, type);
		}

		static Attribute^ create(String^ name, System::Object^ data)
		{
			return New(name, data);
		}

		static Attribute^ New(String^ name, System::Object^ data)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(name, data, data->GetType()->Name);
		}

		static Attribute^ New(String^ name, System::Object^ data, Type^ type)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(name, data, type->Name);
		}
	};
}