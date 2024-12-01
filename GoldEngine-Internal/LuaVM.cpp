#include "SDK.h"
#include "SceneManager.h"
#include "Attribute.h"
#include "AttributeManager.h"
#include "LuaVM.h"
#include "DataManager.h"
#include "Time.h"
#include "GraphicsWrapper.h"
#include "ManagedSignal.h"
#include "Objects/LuaScript.h"

using namespace Engine::Scripting;
using namespace Engine::Lua::VM;

// VMWRAPPER \\

cli::array<EngineAssembly^>^ VMWrapper::GetAssemblies()
{
	return Engine::Managers::SceneManager::GetAssemblyManager()->ToArray();
}

DynValue^ VMWrapper::Derivate(System::Object^ object)
{
	return UserData::Create(System::Convert::ChangeType(object, object->GetType()));
}

bool VMWrapper::HasProperty(Engine::Internal::Components::GameObject^ object, String^ propertyName)
{
	if (object->GetType()->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid))
	{
		Engine::Scripting::AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;

		if (attribs->hasAttribute(propertyName))
		{
			return true;
		}
	}

	return false;
}

Engine::Scripting::AttributeManager^ VMWrapper::GetAttributeManager(Engine::Internal::Components::GameObject^ object)
{
	if (object->GetType()->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid))
	{
		Engine::Scripting::AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;
		return attribs;
	}

	return nullptr;
}

void VMWrapper::SetProperty(Engine::Internal::Components::GameObject^ object, String^ propertyName, System::Object^ newValue)
{
	if (HasProperty(object, propertyName))
	{
		Engine::Scripting::AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;
		attribs->getAttribute(propertyName)->setValue(newValue);
	}
}

// STATIC VM FUNCTIONS \\

LuaVM^ LuaVM::RequireOverride(System::Object^ luaSource)
{
	if (luaSource != Engine::EngineObjects::LuaScript::typeid)
		printError("Cannot require an object that is not a LuaScript");

	return ((Engine::EngineObjects::LuaScript^)luaSource)->virtualMachine;
}

// LUAVM \\

void LuaVM::RegisterGlobalFunctions()
{
	for each (auto asms in VMWrapper::GetAssemblies())
	{
		try
		{
			UserData::RegisterAssembly(asms->getLoadedAssembly(), true);
		}
		catch (Exception^ ex)
		{
			printError(ex->Message);
			printError(ex->StackTrace);
		}
	}

	RegisterGlobal("Logging", Engine::Scripting::Logging::typeid);
	RegisterGlobal("Attribute", Engine::Scripting::Attribute::typeid);
	RegisterGlobal("DataManager", Engine::Internal::DataManager::typeid);
	RegisterGlobal("ObjectManager", Engine::Scripting::ObjectManager::singleton());
	RegisterGlobal("Input", Engine::Scripting::InputManager::typeid);
	RegisterGlobal("KeyCode", Engine::Scripting::KeyCodes::typeid);
	RegisterGlobal("SharedInstance", SharedInstance::typeid);
	RegisterGlobal("VMWrap", VMWrapper::typeid);
	RegisterGlobal("Time", Engine::Scripting::Time::typeid);
	RegisterGlobal("Graphics", Engine::Internal::GraphicsWrapper::typeid);
	RegisterGlobal("Vector2", Engine::Components::Vector2::typeid);
	RegisterGlobal("Vector3", Engine::Components::Vector3::typeid);
	RegisterGlobal("Color", Engine::Components::Color::typeid);
	RegisterGlobal("Event", Engine::Scripting::Events::Event::typeid);


	RegisterGlobal("print", gcnew System::Action<String^>(&Logging::Log));
	RegisterGlobal("warn", gcnew System::Action<String^>(&Logging::LogWarning));
	RegisterGlobal("error", gcnew System::Action<String^>(&Logging::LogError));
	RegisterGlobal("info", gcnew System::Action<String^>(&Logging::LogDebug));
	RegisterGlobal("require", gcnew System::Func<System::Object^, LuaVM^>(this, &LuaVM::RequireOverride));
	RegisterGlobal("log", gcnew System::Action<String^, String^>(&Logging::LogCustom));
	RegisterGlobal("HasProperty", gcnew System::Func<Engine::Internal::Components::GameObject^, String^, bool>(&VMWrapper::HasProperty));
	RegisterGlobal("GetAttributes", gcnew System::Func<Engine::Internal::Components::GameObject^, Engine::Scripting::AttributeManager^>(&VMWrapper::GetAttributeManager));
	RegisterGlobal("SetProperty", gcnew System::Action<Engine::Internal::Components::GameObject^, String^, Object^>(&VMWrapper::SetProperty));
	RegisterGlobal("CastToClass", gcnew System::Func<System::Object^, System::String^, System::Object^>(&VMWrapper::ToDerivate));
	RegisterGlobal("ToDerivate", gcnew System::Func<System::Object^, System::Object^>(&VMWrapper::ToDerivate));
	//RegisterGlobal("VM", this->scriptState);
}

System::Collections::Generic::List<Type^>^ LuaVM::GetMoonSharpTypes(System::Reflection::Assembly^ a)
{
	List<Type^>^ result = gcnew System::Collections::Generic::List<Type^>();

	for each (Type ^ t in a->GetTypes())
	{
		if (t->GetCustomAttributes(MoonSharp::Interpreter::MoonSharpUserDataAttribute::typeid, false)->Length > 0)
		{
			result->Add(t);
		}
	}

	return result;
}

void LuaVM::GenerateLuaBindings()
{
	String^ fileName = "./Bindings/Lua/GoldEngineBindings.lua";
	int index = fileName->LastIndexOf("/");

	Directory::CreateDirectory(fileName->Substring(0, index));

	String^ luaSrcFile = "--[[ GoldVM Binding Generator : Work In Progress ]]--\n\n";

	try
	{
		for each (auto asms in VMWrapper::GetAssemblies())
		{
			try
			{
				List<Type^>^ lua_proxy_types = GetMoonSharpTypes(asms->getLoadedAssembly());


				for each (Type ^ type in lua_proxy_types)
				{
					String^ fullName = type->Name;

					if (fullName->Contains("Proxy"))
						continue;

					luaSrcFile += "\n--[[ " + fullName + " CLASS DEFINITION ]]--\n";

					luaSrcFile += fullName + " = {}\n";

					auto members = type->GetMembers();
					for each (auto member in members)
					{
						if (member->MemberType == System::Reflection::MemberTypes::Field || member->MemberType == System::Reflection::MemberTypes::Property)
						{
							luaSrcFile += fullName + "." + member->Name + " = nil;\n";
						}
					}

					auto methods = type->GetMethods();
					for each (auto method in methods)
					{
						if (method->IsPublic)
						{
							luaSrcFile += "function " + fullName + "." + method->Name + "(";

							auto params = method->GetParameters();
							int length = params->Length;
							for (int x = 0; x < length; x++)
							{
								auto param = params[x];
								if (x < length - 1)
								{
									luaSrcFile += param->Name + "__" + param->ParameterType->Name + ",";
								}
								else
								{
									luaSrcFile += param->Name + "__" + param->ParameterType->Name + "";
								}
							}

							luaSrcFile += ")end\n";
						}
					}
				}
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				printError(ex->StackTrace);
			}
		}

		File::WriteAllText("./Bindings/Lua/GoldEngineBindings.lua", luaSrcFile);
	}
	catch (Exception^ ex)
	{
		printError(ex->Message);
		printError(ex->StackTrace);
	}
}