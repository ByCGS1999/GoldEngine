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

	public:
		void HookUpdate() override
		{
			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						String^ attribName = attrib->attributeName;

						if (attribName == "")
							attribName = prop->Name;

						if (prop->GetValue(this) != this->attributes->getAttribute(attribName)->getValue())
							this->attributes->getAttribute(attribName)->setValue(prop->GetValue(this));
					}
				}

			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
			}
		}

	public:
		virtual void Init() override
		{
			attributes->DeserializeAttributes();

			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						if (attrib->attributeName == "")
						{
							Engine::Scripting::Attribute^ attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								prop->Name,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(prop->Name)->setPropertyDescriptor(prop, this);
						}
						else
						{
							Engine::Scripting::Attribute^ attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attrib->attributeName,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(prop->Name)->setPropertyDescriptor(prop, this);
						}
					}
				}

			}
			catch (Exception^ ex)
			{

			}

		}

		virtual void Setup() override
		{
			attributes->DeserializeAttributes();

			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						if (attrib->attributeName == "")
						{
							Engine::Scripting::Attribute^ attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								prop->Name,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(prop->Name)->setPropertyDescriptor(prop, this);
						}
						else
						{
							Engine::Scripting::Attribute^ attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attrib->attributeName,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(prop->Name)->setPropertyDescriptor(prop, this);
						}
					}
				}

			}
			catch (Exception^ ex)
			{

			}
		}
	};
}