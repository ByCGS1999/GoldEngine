#pragma once

#include "PreloadScript.h"

ref class EngineAssembly
{
private:
	System::Reflection::Assembly^ loadedAssembly;

public:
	EngineAssembly(System::String^ fN)
	{
		LoadAssemblyFromFile(fN);
	}

	EngineAssembly(System::Reflection::Assembly^ assm)
	{
		LoadAssemblyFromRawAssembly(assm);
	}

private:
	void LoadAssemblyFromFile(System::String^ fileName)
	{
		loadedAssembly = loadedAssembly->LoadFile(fileName);
	}
	
	void LoadAssemblyFromRawAssembly(System::Reflection::Assembly^ assembly)
	{
		loadedAssembly = assembly;
	}

public:
	System::Reflection::Assembly^ getLoadedAssembly()
	{
		return loadedAssembly;
	}

public:
	void ListAssemblyTypes()
	{
		if (loadedAssembly != nullptr)
		{
			printf(" -- Assembly Types -- \n");
			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (!type->Namespace->IsNullOrEmpty(type->Namespace))
				{
					if (type->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid) || type->IsSubclassOf(Engine::Internal::Components::Object::typeid))
					{
						Console::WriteLine("Type Found: " + type->FullName);
					}
					/*
#if !PRODUCTION_BUILD
					if (type->Namespace->Contains("EditorScripts"))
					{
						Console::WriteLine("Type Found: " + type->FullName);
					}
#endif
*/
				}
			}
		}
	}

public:
	System::Collections::Generic::List<System::Type^>^ GetTypesOf(System::Type^ parentType)
	{
		System::Collections::Generic::List<System::Type^>^ types = gcnew System::Collections::Generic::List<System::Type^>();

		if (loadedAssembly != nullptr)
		{
			for each (Type ^ t in loadedAssembly->GetTypes())
			{
				if (!t->Namespace->IsNullOrEmpty(t->Namespace))
				{
					if (t->IsSubclassOf(parentType))
					{
						types->Add(t);
					}
				}
			}
		}

		return types;
	}

public:
	System::Collections::Generic::List<System::Type^>^ GetAssemblyTypes()
	{
		System::Collections::Generic::List<System::Type^>^ types = gcnew System::Collections::Generic::List<System::Type^>();

		if (loadedAssembly != nullptr)
		{
			for each (Type ^ t in loadedAssembly->GetTypes())
			{
				if (!t->Namespace->IsNullOrEmpty(t->Namespace))
				{
					if (t->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid) || t->IsSubclassOf(Engine::Internal::Components::Object::typeid))
					{
						types->Add(t);
					}
				}
			}
		}

		return types;
	}

	bool hasType(String^ asmType)
	{
		if (loadedAssembly != nullptr)
		{
			Type^ storedType = nullptr;

			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (type->FullName->Equals(asmType))
				{
					storedType = type;
				}
			}

			if (storedType != nullptr)
				return true;
			else
				return false;
		}

		return false;
	}

	Type^ GetTypeFromString(String^ asmType)
	{
		Type^ storedType = nullptr;

		for each (Type ^ type in loadedAssembly->GetTypes())
		{
			if (type->FullName->Equals(asmType))
			{
				storedType = type;
			}
		}

		return storedType;
	}

	auto CastToType(Engine::Management::MiddleLevel::SceneObject^ sceneObject, String^ asmType)
	{
		if (loadedAssembly != nullptr)
		{
			Type^ storedType = nullptr;

			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (type->FullName->Equals(asmType))
				{
					storedType = type;
				}
			}

			if (storedType != nullptr)
			{
				System::Reflection::MethodInfo^ baseInfo = sceneObject->GetType()->GetMethod("serializeAs");
				System::Reflection::MethodInfo^ mInfo = baseInfo->MakeGenericMethod(storedType);

				return (System::Object^)mInfo->Invoke(sceneObject, nullptr);
			}

		}

		return (System::Object^)sceneObject;
	}

	auto CastToType(Engine::Internal::Components::Object^ object, String^ asmType)
	{
		if (loadedAssembly != nullptr)
		{
			Type^ storedType = nullptr;

			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (type->FullName->Equals(asmType))
				{
					storedType = type;
				}
			}

			if (storedType != nullptr)
			{
				System::Reflection::MethodInfo^ baseInfo = object->GetType()->GetMethod("ToObjectType");
				System::Reflection::MethodInfo^ mInfo = baseInfo->MakeGenericMethod(storedType);

				return (System::Object^)mInfo->Invoke(object, nullptr);
			}

			//return (System::Object^)Convert::ChangeType((System::Object^)object, storedType);
		}

		return (System::Object^)object;
	}

	System::Collections::Generic::List<System::Type^>^ getPreloadScripts()
	{
		System::Collections::Generic::List<System::Type^>^ types = gcnew System::Collections::Generic::List<System::Type^>();

		if (loadedAssembly != nullptr)
		{
			for each (Type ^ t in loadedAssembly->GetTypes())
			{
				if (t->IsClass && Engine::Preload::IPreloadScript::typeid->IsAssignableFrom(t))
				{
					types->Add(t);
				}
			}
		}

		return types;
	}

	auto CreateSimple(Type^ targetType)
	{
		if (loadedAssembly != nullptr)
		{
			auto params = gcnew System::Collections::Generic::List<System::Object^>();

			if (targetType != nullptr)
				return System::Convert::ChangeType(System::Activator::CreateInstance(targetType, params->ToArray()), targetType);
		}
	}

	auto Create(Type^ targetType, String^ str)
	{
		if (loadedAssembly != nullptr)
		{
			Type^ storedType = nullptr;

			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (type->FullName->Equals(str))
				{
					storedType = type;
				}
			}

			auto transform = gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Components::Vector3(0, 0, 0),
				gcnew Engine::Components::Vector3(0, 0, 0),
				0.0f,
				gcnew Engine::Components::Vector3(1, 1, 1),
				nullptr
			);

			auto params = gcnew System::Collections::Generic::List<System::Object^>();

			params->Add(storedType->Name + " - Reflected");
			params->Add(transform);

			if (storedType != nullptr)
				return System::Convert::ChangeType(System::Activator::CreateInstance(storedType, params->ToArray()), targetType);
		}
	}

	template <class T>
	T Create(String^ str)
	{
		if (loadedAssembly != nullptr)
		{
			Type^ storedType = nullptr;

			for each (Type ^ type in loadedAssembly->GetTypes())
			{
				if (type->FullName->Equals(str))
				{
					storedType = type;
				}
			}

			auto transform = gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Components::Vector3(0, 0, 0),
				gcnew Engine::Components::Vector3(0, 0, 0),
				0.0f,
				gcnew Engine::Components::Vector3(1, 1, 1),
				nullptr
			);

			array<System::Object^>^ params = { storedType->Name + " - Reflected", transform };

			if (storedType != nullptr)
				return (T)System::Activator::CreateInstance(storedType, params);
		}
	}
};