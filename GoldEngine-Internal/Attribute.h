#pragma once

namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public:
		String^ name;
		System::Object^ userData;
		System::Type^ userDataType;

	public:
		Attribute(String^ str, System::Object^ data);
		[Newtonsoft::Json::JsonConstructorAttribute]
		Attribute(String^ str, System::Object^ data, System::Type^ dT);

	public:
		void setValue(System::Object^ object);

		void setValue(System::Object^ object, bool overrideType);

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
			return Convert::ChangeType(userData, userDataType);
		}

		System::Type^ getValueType()
		{
			return this->userDataType;
		}

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

			return gcnew Attribute(name, data, data->GetType());
		}

		static Attribute^ New(String^ name, System::Object^ data, Type^ type)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(name, data, type);
		}
	};
}