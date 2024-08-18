#pragma once

namespace Engine::Scripting
{

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public: 
		enum class AccessLevel
		{
			Public,
			ReadOnly,
			WriteOnly
		};

	public:
		AccessLevel accessLevel;
		String^ name;
		System::Object^ userData;
		System::Type^ userDataType;

	public:
		Attribute(String^ str, System::Object^ data);
		[Newtonsoft::Json::JsonConstructorAttribute]
		Attribute(AccessLevel level, String^ str, System::Object^ data, System::Type^ dT);

	public:
		void setValue(System::Object^ object);

		void setValue(System::Object^ object, bool overrideType);

	public:
		generic <class T>
		T getValue()
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

			return (T)(getValue<Newtonsoft::Json::Linq::JObject^>()->ToObject<T>());
		}

		bool isJObject()
		{
			return userData->GetType() == Newtonsoft::Json::Linq::JObject::typeid;
		}

		System::Object^ getValue()
		{
			if (accessLevel == AccessLevel::WriteOnly)
				return { };

			return userData;
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
				printError(ex->Message);
				printError(ex->StackTrace);
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
				printError(ex->Message);
				printError(ex->StackTrace);
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
}