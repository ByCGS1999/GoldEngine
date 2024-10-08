using namespace System;

#include "Includes.h"
#include "Reflection/ReflectedType.h"
#include "GlIncludes.h"
#include "CastToNative.h"
#include "LoggingAPI.h"
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

	if (data != nullptr)
	{
		if (userDataType == nullptr)
			userDataType = gcnew Engine::Reflectable::ReflectableType(data->GetType());
		else
			userDataType->DeserializeType();
	}
	else
	{
		userDataType = gcnew Engine::Reflectable::ReflectableType();
	}

	if (data == Newtonsoft::Json::Linq::JObject::typeid)
	{
		userData = ((Newtonsoft::Json::Linq::JObject^)data)->ToObject<System::Object^>();
	}
	else
	{
		userData = data;
	}

	setType(userDataType->getTypeReference()); // For consistency
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
	{
		if (userDataType == nullptr)
			userDataType = gcnew Engine::Reflectable::ReflectableType(data->GetType());
		else
			userDataType->DeserializeType();
	}
	else
	{
		userDataType = gcnew Engine::Reflectable::ReflectableType(dT);
	}

	if (data == Newtonsoft::Json::Linq::JObject::typeid)
	{
		userData = ((Newtonsoft::Json::Linq::JObject^)data)->ToObject<Object^>();
	}
	else
	{
		userData = data;
	}

	setType(userDataType->getTypeReference()); // For consistency
}

inline void Engine::Scripting::Attribute::synchronizeDescriptor()
{
	if(descriptor != nullptr && rootObject != nullptr)
		if(descriptor->GetType() == System::Reflection::FieldInfo::typeid)
			((System::Reflection::FieldInfo^)descriptor)->SetValue(rootObject, userData);
		else
			((System::Reflection::PropertyInfo^)descriptor)->SetValue(rootObject, userData);
}

inline void Engine::Scripting::Attribute::setPropertyDescriptor(System::Reflection::MemberInfo^ descriptor, System::Object^ rootDescriptor)
{
	this->descriptor = descriptor;
	this->rootObject = rootDescriptor;
}

inline void Engine::Scripting::Attribute::setValue(System::Object^ object)
{
	if (accessLevel == AccessLevel::ReadOnly)
		return;

	userData = object;
	userDataType->SetType(object->GetType());

	if (descriptor != nullptr)
		if (descriptor->GetType() == System::Reflection::FieldInfo::typeid)
			((System::Reflection::FieldInfo^)descriptor)->SetValue(rootObject, userData);
		else
			((System::Reflection::PropertyInfo^)descriptor)->SetValue(rootObject, userData);
}

inline void Engine::Scripting::Attribute::setValue(System::Object^ object, bool overrideType)
{
	if (accessLevel == AccessLevel::ReadOnly)
		return;

	userData = object;
	
	if (descriptor != nullptr)
		if (descriptor->GetType() == System::Reflection::FieldInfo::typeid)
			((System::Reflection::FieldInfo^)descriptor)->SetValue(rootObject, userData);
		else
			((System::Reflection::PropertyInfo^)descriptor)->SetValue(rootObject, userData);

	if (overrideType)
		userDataType->SetType(object->GetType());
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