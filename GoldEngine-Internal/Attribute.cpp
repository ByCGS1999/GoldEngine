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
	userData = data;
	userDataType = data->GetType();
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
	userData = data;
	userDataType = dT;

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
