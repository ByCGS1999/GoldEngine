#pragma once

namespace Engine::Scripting
{
	public enum class AccessLevel
	{
		Public,
		ReadOnly,
		WriteOnly
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public:
		AccessLevel accessLevel;
		String^ name;
		System::Type^ userDataType;
		System::Object^ userData;

	private:
		System::Reflection::PropertyInfo^ descriptor;
		System::Object^ rootObject;
		
	public:
		Attribute(String^ str, System::Object^ data);
		[Newtonsoft::Json::JsonConstructorAttribute]
		Attribute(AccessLevel level, String^ str, System::Object^ data, System::Type^ dT);

	public:
		void setValue(System::Object^ object);
		void setValue(System::Object^ object, bool overrideType);
		void setPropertyDescriptor(System::Reflection::PropertyInfo^ descriptor, System::Object^ rootDescriptor);

	public:
		System::Object^ getValue()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return nullptr;

			return userData;
		}

		generic <class T>
		T getValue()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return { };

			return (T)userData;
		}

		generic <class T>
		T getValueAs()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return { };

			return (T)userData;
		}

		generic <class T>
		T getValueFromJObject()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return { };

			return (T)(getValueAs<Newtonsoft::Json::Linq::JObject^>()->ToObject<T>());
		}

		bool isJObject()
		{
			return userData->GetType() == Newtonsoft::Json::Linq::JObject::typeid;
		}

		auto getValueAuto()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return gcnew System::Object();

			return Convert::ChangeType(userData, userDataType);
		}

		System::Type^ getValueType()
		{
			return this->userDataType;
		}

		System::Type^ getCurrentType()
		{
			return this->userData->GetType();
		}

		void lockRead();
		void lockWrite();
		void unlock();

	public:
		System::Object^ DeserializeAttribute()
		{
			if (userData->GetType() != Newtonsoft::Json::Linq::JObject::typeid)
				return userData;

			try
			{
				Newtonsoft::Json::Linq::JObject^ sonObject = (Newtonsoft::Json::Linq::JObject^)userData;

				return sonObject->ToObject(userDataType);
			}
			catch (Exception^ ex)
			{
			#ifdef LOGAPI_IMPL
				printError(ex->Message);
				printError(ex->StackTrace);
			#endif

				return userData;
			}
		}

		void setType(System::Type^ type)
		{
			if (userData == nullptr)
				return;

			if (userData->GetType() == type)
				return;

			try 
			{
			#ifdef LOGAPI_IMPL
				printConsole("Converting from " + userData->GetType()->Name + " To -> " + type->Name);
			#endif
				userDataType = type;

				userData = System::Convert::ChangeType(userData, type);
			}
			catch (Exception^ ex)
			{
			#ifdef LOGAPI_IMPL

			#endif
			}
		}

		void resetType()
		{
			try
			{
#ifdef LOGAPI_IMPL
				printConsole("Converting from " + userData->GetType()->Name + " To -> " + userDataType->Name);
#endif
				userData = System::Convert::ChangeType(userData, userDataType);
			}
			catch (Exception^ ex)
			{
#ifdef LOGAPI_IMPL

#endif
			}
		}


	public:
		static Attribute^ create(String^ name, System::Object^ data, Type^ type)
		{
			return New(AccessLevel::Public, name, data, type);
		}

		static Attribute^ create(AccessLevel accessLevel, String^ name, System::Object^ data, Type^ type)
		{
			return New(accessLevel, name, data, type);
		}

		static Attribute^ create(String^ name, System::Object^ data)
		{
			return New(AccessLevel::Public, name, data);
		}

		static Attribute^ create(AccessLevel accessLevel, String^ name, System::Object^ data)
		{
			return New(accessLevel, name, data);
		}

		static Attribute^ New(AccessLevel level, String^ name, System::Object^ data)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(level, name, data, data->GetType());
		}

		static Attribute^ New(AccessLevel level, String^ name, System::Object^ data, Type^ type)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(level, name, data, type);
		}
	};

	public ref class PropertyAttribute : System::Attribute
	{
	public:
		Engine::Scripting::AccessLevel accessLevel;
		String^ attributeName;

	public:
		PropertyAttribute(Engine::Scripting::AccessLevel level, String^ name)
		{
			this->accessLevel = level;
			this->attributeName = name;
		}

		PropertyAttribute(Engine::Scripting::AccessLevel level)
		{
			this->accessLevel = level;
			this->attributeName = "";
		}
	};
}