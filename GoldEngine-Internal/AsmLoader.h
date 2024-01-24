#pragma once

ref class EngineAssembly
{
private:
	System::Reflection::Assembly^ loadedAssembly;

public:
	void LoadAssemblyFromFile(System::String^ fileName)
	{
		loadedAssembly = loadedAssembly->LoadFrom(fileName);
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
					if (type->Namespace->Contains("UserScripts"))
					{
						Console::WriteLine("Type Found: " + type->FullName);
					}
				}
			}
		}
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
					if (t->Namespace->Contains("UserScripts"))
					{
						types->Add(t);

					}
					else if (t->Namespace->Contains("Preload"))
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

			return System::Convert::ChangeType(object, storedType);
		}

		return (System::Object^)object;
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
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				0.0f,
				gcnew Engine::Internal::Components::Vector3(1, 1, 1),
				nullptr
			);

			auto params = gcnew System::Collections::Generic::List<System::Object^>();

			params->Add(storedType->Name + " - Reflected");
			params->Add(transform);

			if (storedType != nullptr)
				return (T)System::Activator::CreateInstance(storedType, params->ToArray());
		}
	}
};