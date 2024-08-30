using namespace System;

#include "Attribute.h"

inline Engine::Scripting::Attribute::Attribute(String^ str, System::Object^ data)
{
	/*
	if (data == nullptr)
	{
		delete this; // clean memory, fuck off
		return;
	}
	*/
	this->accessLevel = AccessLevel::Public;
	this->name = str;
	if(data != nullptr)
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
inline Engine::Scripting::Attribute::Attribute(AccessLevel level, String^ str, System::Object^ data, System::Type^ dT)
{
	/*
	if (data == nullptr)
	{
		delete this; // clean memory, fuck off
		return;
	}
	*/
	this->accessLevel = level;
	this->name = str;
	if (data != nullptr)
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
	if (accessLevel == AccessLevel::ReadOnly)
		return;

	userData = object;
	userDataType = object->GetType();
}

inline void Engine::Scripting::Attribute::setValue(System::Object^ object, bool overrideType)
{
	if (accessLevel == AccessLevel::ReadOnly)
		return;

	userData = object;

	if (overrideType)
		userDataType = object->GetType();
}

inline void Engine::Scripting::Attribute::lockRead()
{
	accessLevel = AccessLevel::WriteOnly;
}

inline void Engine::Scripting::Attribute::lockWrite()
{
	accessLevel = AccessLevel::ReadOnly;
}

inline void Engine::Scripting::Attribute::unlock()
{
	accessLevel = AccessLevel::Public;
}