#pragma once

namespace Engine::Scripting
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Attribute
	{
	public:
		String^ name;
		System::Object^ userData;
		System::String^ type;
		System::Type^ userDataType;

	public:
		Attribute(String^ str, System::Object^ data)
		{
			if (data == nullptr)
			{
				printError("Attribute: " + str + " Value is a null pointer.");
				delete this; // clean memory, fuck off
				return;
			}

			this->name = str;
			userData = data;
			type = data->GetType()->Name;
			userDataType = data->GetType();
		}
		Attribute(String^ str, System::Object^ data, String^ dataType)
		{
			if (data == nullptr)
			{
				printError("Attribute: " + str + " Value is a null pointer.");
				delete this; // clean memory, fuck off
				return;
			}

			this->name = str;
			userData = data;
			type = dataType;
			userDataType = data->GetType();
		}
		[Newtonsoft::Json::JsonConstructorAttribute]
		Attribute(String^ str, System::Object^ data, String^ dataType, System::Type^ dT)
		{
			if (data == nullptr)
			{
				printError("Attribute: " + str + " Value is a null pointer.");
				delete this; // clean memory, fuck off
				return;
			}

			this->name = str;
			userData = data;
			type = dataType;
			userDataType = dT;
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
				print("[ENGINE EXCEPTION]", ex->Message);

				return userData;
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

			return gcnew Attribute(name, data, data->GetType()->Name, data->GetType());
		}

		static Attribute^ New(String^ name, System::Object^ data, Type^ type)
		{
			if (data == nullptr)
				return nullptr;

			return gcnew Attribute(name, data, type->Name, type);
		}
	};
}