using namespace System;

#include "Attribute.h"

inline Engine::Scripting::Attribute::Attribute(String^ str, System::Object^ data)
{
	if (data == nullptr)
	{
		delete this; // clean memory, fuck off
		return;
	}

	this->name = str;
	userDataType = data->GetType();

	if (data == Newtonsoft::Json::Linq::JObject::typeid)
	{
		userData = ((Newtonsoft::Json::Linq::JObject^)data)->ToObject<System::Object^>();
	}
	else
	{
		userData = data;
	}
}

[Newtonsoft::Json::JsonConstructorAttribute]
inline Engine::Scripting::Attribute::Attribute(String^ str, System::Object^ data, System::Type^ dT)
{
	if (data == nullptr)
	{
		delete this; // clean memory, fuck off
		return;
	}

	this->name = str;
	userDataType = dT;

	if (data == Newtonsoft::Json::Linq::JObject::typeid)
	{
		userData = ((Newtonsoft::Json::Linq::JObject^)data)->ToObject<Object^>();
	}
	else
	{
		userData = data;
	}

	setType(userDataType); // For consistency
}

inline void Engine::Scripting::Attribute::setValue(System::Object^ object)
{
	userData = object;
	userDataType = object->GetType();
}

inline void Engine::Scripting::Attribute::setValue(System::Object^ object, bool overrideType)
{
	userData = object;

	if (overrideType)
		userDataType = object->GetType();
}
