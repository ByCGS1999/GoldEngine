#pragma once

namespace Engine::EngineObjects
{
	public ref class ScriptBehaviour : public Engine::Internal::Components::Object
	{
	public:
		System::String^ assemblyReference;
		Engine::Scripting::AttributeManager^ attributes;

	public:
		ScriptBehaviour(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Script, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			attributes = gcnew Engine::Scripting::AttributeManager();
			assemblyReference = GetType()->FullName->ToString();

			attributes->DeserializeAttributes();
		}

		virtual void Init() override
		{
			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						if (attrib->attributeName == "")
						{
							this->attributes->setAttribute_suppressed(
								Engine::Scripting::Attribute::create(
									attrib->accessLevel,
									prop->Name,
									prop->GetValue(this),
									prop->PropertyType
								)
							);
						}
						else
						{
							this->attributes->setAttribute_suppressed(
								Engine::Scripting::Attribute::create(
									attrib->accessLevel,
									attrib->attributeName,
									prop->GetValue(this),
									prop->PropertyType
								)
							);
						}
					}
				}

			}
			catch (Exception^ ex)
			{

			}

			attributes->DeserializeAttributes();
		}

		virtual void Setup() override
		{
			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						if (attrib->attributeName == "")
						{
							this->attributes->setAttribute_suppressed(
								Engine::Scripting::Attribute::create(
									attrib->accessLevel,
									prop->Name,
									prop->GetValue(this),
									prop->PropertyType
								)
							);
						}
						else
						{
							this->attributes->setAttribute_suppressed(
								Engine::Scripting::Attribute::create(
									attrib->accessLevel,
									attrib->attributeName,
									prop->GetValue(this),
									prop->PropertyType
								)
							);
						}
					}
				}

			}
			catch (Exception^ ex)
			{

			}

			attributes->DeserializeAttributes();
		}
	};
}