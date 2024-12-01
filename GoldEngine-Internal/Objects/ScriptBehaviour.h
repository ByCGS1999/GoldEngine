#pragma once

namespace Engine::EngineObjects
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class ScriptBehaviour : public Engine::Internal::Components::GameObject
	{
	public:
		System::String^ assemblyReference;
		Engine::Scripting::AttributeManager^ attributes;

	public:
		ScriptBehaviour(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::GameObject(name, transform, Engine::Internal::Components::ObjectType::Script, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			attributes = gcnew Engine::Scripting::AttributeManager();
			assemblyReference = GetType()->FullName->ToString();
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

				for each (auto prop in GetType()->GetFields(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
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
			try
			{
				for each (auto prop in GetType()->GetProperties(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						String^ attributeName = attrib->attributeName;

						if (attributeName == "")
							attributeName = prop->Name;

						if (!this->attributes->hasAttribute(attributeName))
						{
							auto attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attributeName,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->PropertyType);
						}
						else
						{
							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->PropertyType);
						}
					}
				}

				for each (auto prop in GetType()->GetFields(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						String^ attributeName = attrib->attributeName;

						if (attributeName == "")
							attributeName = prop->Name;

						if (!this->attributes->hasAttribute(attributeName))
						{
							auto attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attributeName,
								prop->GetValue(this),
								prop->FieldType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->FieldType);
						}
						else
						{
							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->FieldType);
						}
					}
				}

				attributes->DeserializeAttributes();
				HookUpdate();
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
						String^ attributeName = attrib->attributeName;

						if (attributeName == "")
							attributeName = prop->Name;

						if (!this->attributes->hasAttribute(attributeName))
						{
							auto attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attributeName,
								prop->GetValue(this),
								prop->PropertyType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->PropertyType);
						}
						else
						{
							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->PropertyType);
						}
					}
				}

				for each (auto prop in GetType()->GetFields(System::Reflection::BindingFlags::Public | System::Reflection::BindingFlags::NonPublic | System::Reflection::BindingFlags::Instance | System::Reflection::BindingFlags::CreateInstance))
				{
					auto attributes = prop->GetCustomAttributes(Engine::Scripting::PropertyAttribute::typeid, true);
					for each (Engine::Scripting::PropertyAttribute ^ attrib in attributes)
					{
						String^ attributeName = attrib->attributeName;

						if (attributeName == "")
							attributeName = prop->Name;

						if (!this->attributes->hasAttribute(attributeName))
						{
							auto attribv = Engine::Scripting::Attribute::create(
								attrib->accessLevel,
								attributeName,
								prop->GetValue(this),
								prop->FieldType
							);

							this->attributes->setAttribute_suppressed(
								attribv
							);

							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->FieldType);
						}
						else
						{
							this->attributes->getAttribute(attributeName)->setPropertyDescriptor(prop, this);
							this->attributes->getAttribute(attributeName)->userDataType = gcnew Engine::Reflectable::ReflectableType(prop->FieldType);
						}
					}
				}

				attributes->DeserializeAttributes();
				HookUpdate();
			}
			catch (Exception^ ex)
			{

			}
		}
	};
}