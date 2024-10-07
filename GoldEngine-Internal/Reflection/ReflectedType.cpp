using namespace System;

#include "../Includes.h"
#include "../GlIncludes.h"
#include "../CastToNative.h"
#include "../LoggingAPI.h"
#include "ReflectedType.h"

using namespace Engine::Reflectable;

Reflection::Assembly^ ReflectableManager::GetAssemblyByName(String^ assemblyName)
{
	for each (auto Assembly in assemblies)
	{
		if (Assembly->GetName()->Name->Equals(assemblyName))
			return Assembly;
	}

	return nullptr;
}


Type^ ReflectableManager::GetTypeFromName(Reflection::Assembly^ assembly, String^ typeName, String^ nameSpace)
{
	for each (auto type in assembly->GetTypes())
	{
		if (type->Namespace != nameSpace)
			continue;

		if (type->FullName->Equals(typeName))
			return type;
	}

	return assembly->GetType(typeName);
}

Type^ ReflectableManager::GetTypeFromName(String^ typeName, String^ nameSpace)
{
	return System::Type::GetType(typeName);
}

ReflectableType::ReflectableType(Type^ type)
{
	if (type == nullptr)
		return;

	this->typeReference = type;
	this->type = gcnew ReflectedType(
		type->Assembly->GetName()->Name,
		type->FullName,
		type->Namespace,
		type->Assembly->ImageRuntimeVersion
	);

	this->reflectedMess = Serialize(this->type);
}

ReflectableType::ReflectableType(String^ messyCode)
{
	this->type = nullptr;
	this->typeReference = nullptr;

	this->reflectedMess = messyCode;
}


ReflectableType::ReflectableType()
{
	this->type = nullptr;
	this->typeReference = nullptr;

	DeserializeType();
}

void ReflectableType::SolveTypeRef()
{
	ReflectedType^ type = this->type;

	Reflection::Assembly^ assembly = ReflectableManager::GetAssemblyByName(type->assemblyName);

	if (assembly != nullptr)
	{
		this->typeReference = ReflectableManager::GetTypeFromName(assembly, type->typeName, type->nameSpace);
	}
	else
	{
		this->typeReference = ReflectableManager::GetTypeFromName(type->typeName, type->nameSpace);
	}

	/*
	if (assembly->ImageRuntimeVersion->Equals(type->assemblyVersion))
	{
		// valid version
	}
	else
	{
		Engine::Scripting::Logging::LogError("Invalid image version of " + type->assemblyName + " : expected " + type->assemblyVersion + " got " + assembly->ImageRuntimeVersion + ".");
	}
	*/
}

void ReflectableType::SetType(Type^ type)
{
	this->typeReference = type;
	this->type = gcnew ReflectedType(
		type->Assembly->GetName()->Name,
		type->FullName,
		type->Namespace,
		type->Assembly->ImageRuntimeVersion
	);

	this->reflectedMess = Serialize(this->type);
}

Type^ ReflectableType::getTypeReference()
{
	return this->typeReference;
}

void ReflectableType::DeserializeType(String^ serializedData)
{
	if (this->typeReference != nullptr && this->type != nullptr)
		return;

	try
	{
		this->type = Deserialize<ReflectedType>(serializedData);
		SolveTypeRef();
	}
	catch (Exception^ ex)
	{
		Engine::Scripting::Logging::LogError(ex->Message);
		Engine::Scripting::Logging::LogError(ex->StackTrace);
	}
}

void ReflectableType::DeserializeType()
{
	if (this->typeReference != nullptr || this->type != nullptr || (reflectedMess == nullptr || reflectedMess->IsNullOrEmpty(reflectedMess)))
		return;

	try
	{
		this->type = Deserialize<ReflectedType>(reflectedMess);
		SolveTypeRef();
	}
	catch (Exception^ ex)
	{
		Engine::Scripting::Logging::LogError(ex->Message);
		Engine::Scripting::Logging::LogError(ex->StackTrace);
	}
}