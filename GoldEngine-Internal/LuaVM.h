#pragma once

#using <mscorlib.dll>
#using <System.Core.dll>

using namespace MoonSharp::Interpreter;

namespace Engine::Lua::VM
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class VMWrapper // Lua Wrapper for System::Type^
	{
	public:
		static cli::array<EngineAssembly^>^ GetAssemblies()
		{
			return Engine::Managers::SceneManager::GetAssemblyManager()->ToArray();
		}

	public:
		static System::Type^ GetRuntimeType(System::Object^ object)
		{
			return object->GetType();
		}

		static String^ GetFullName(System::Type^ type)
		{
			return type->FullName;
		}

		static void PrintType(System::Object^ object)
		{
			Logging::Log(object->GetType()->FullName);
		}

		static System::Object^ ToDerivate(System::Object^ object, System::String^ targetType)
		{
			System::Type^ toType = GetTypeByName(targetType);

			if (toType == nullptr)
				return DynValue::Nil;

			auto convertedType = System::Convert::ChangeType(object, toType);

			return UserData::Create(convertedType);
		}

		static System::Object^ ToDerivate(System::Object^ object)
		{
			if (object == nullptr)
				return DynValue::Nil;

			System::Type^ toType = object->GetType();

			if (toType == nullptr)
				return DynValue::Nil;

			auto convertedType = System::Convert::ChangeType(object, toType);

			return UserData::Create(convertedType);
		}

		static void AlterVMGlobals(MoonSharp::Interpreter::Table^ globals, String^ globalName, Object^ newObject)
		{
			globals[globalName] = newObject;
		}

		static void DerivateVMGlobal(MoonSharp::Interpreter::Script^& vm, String^ globalName)
		{
			System::Object^ object = vm->Globals[globalName];

			vm->Globals[globalName] = System::Convert::ChangeType(object, object->GetType());
		}

		static void UpdateVMGlobal(MoonSharp::Interpreter::Script^& vm, String^ globalName, Object^ newObject)
		{
			vm->Globals[globalName] = newObject;
		}

		static System::Type^ GetTypeByName(System::String^ typeName)
		{
			auto asms = GetAssemblies();
			System::Type^ currentType = nullptr;
			
			for each (auto assembly in asms)
			{
				currentType = assembly->GetTypeByName(typeName);

				if (currentType != nullptr)
					break;
			}

			return currentType;
		}

		static DynValue^ Derivate(System::Object^ object)
		{
			return UserData::Create(System::Convert::ChangeType(object, object->GetType()));
		}

		static bool HasProperty(Engine::Internal::Components::Object^ object, String^ propertyName)
		{
			if (object->GetType()->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid))
			{
				AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;

				if (attribs->hasAttribute(propertyName))
				{
					return true;
				}
			}

			return false;
		}

		static AttributeManager^ GetAttributeManager(Engine::Internal::Components::Object^ object)
		{
			if (object->GetType()->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid))
			{
				AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;
				return attribs;
			}

			return nullptr;
		}

		static void SetProperty(Engine::Internal::Components::Object^ object, String^ propertyName, System::Object^ newValue)
		{
			if (HasProperty(object, propertyName))
			{
				AttributeManager^ attribs = ((Engine::EngineObjects::ScriptBehaviour^)object)->attributes;
				attribs->getAttribute(propertyName)->setValue(newValue);
			}
		}
	};

	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class LuaVM
	{
	public:
		String^ BINARY_HEADER = "GoldVM";
		int BYTECODE_VERSION = 0x72;
		String^ tempBuffer;

	private:
		MoonSharp::Interpreter::Script^ scriptState;
		String^ source;
		DynValue^ value;

	public:
		LuaVM()
		{
			tempBuffer = "";
			scriptState = gcnew MoonSharp::Interpreter::Script(CoreModules::Preset_Complete);

			RegisterGlobalFunctions();
		}

	public:
		auto GetGlobals()
		{
			return scriptState->Globals;
		}

	public:
		MoonSharp::Interpreter::Script^ GetScriptState()
		{
			return scriptState;
		}

	public:
		void LoadSource(String^ source)
		{
			this->source = source;
		}

	public:
		void WriteLuaCodeToFile(String^ src)
		{
			FileStream^ f = File::Open(src, FileMode::OpenOrCreate);
			BinaryWriter^ bwriter = gcnew BinaryWriter(f);

			bwriter->Write(BINARY_HEADER);
			bwriter->Write(BYTECODE_VERSION);
			auto bytes = Encoding::UTF32->GetBytes(CypherLib::EncryptFileContents(source, ::passwd));
			String^ base64_str = System::Convert::ToBase64String(bytes);
			auto byteArray = Encoding::UTF7->GetBytes(base64_str);

			bwriter->Write(byteArray->Length);

			bwriter->Write(byteArray);

			bwriter->Close();
		}

		void ReadLuaCodeFromFile(String^ src)
		{
			FileStream^ f = File::Open(src, FileMode::OpenOrCreate);
			BinaryReader^ breader = gcnew BinaryReader(f);

			String^ header = breader->ReadString();
			int version = breader->ReadInt32();

			if (header->Equals(BINARY_HEADER))
			{
				if (version == BYTECODE_VERSION)
				{
					int len = breader->ReadInt32();
					String^ base64 = Encoding::UTF7->GetString(breader->ReadBytes(len));

					tempBuffer = CypherLib::DecryptFileContents(Encoding::UTF32->GetString(System::Convert::FromBase64String(base64)), ::passwd);
				}
				else
				{
					printError("Lua version mismatch\n");
				}
			}
			else
			{
				printError("Lua header mismatch\n");
			}
		}

		String^ ReadFromFile(String^ src)
		{
			try
			{
				FileStream^ f = File::Open(src, FileMode::OpenOrCreate);
				BinaryReader^ breader = gcnew BinaryReader(f);

				String^ header = breader->ReadString();
				int version = breader->ReadInt32();

				if (header->Equals(BINARY_HEADER))
				{
					if (version == BYTECODE_VERSION)
					{
						int len = breader->ReadInt32();
						String^ base64 = Encoding::UTF7->GetString(breader->ReadBytes(len));

						return CypherLib::DecryptFileContents(Encoding::UTF32->GetString(System::Convert::FromBase64String(base64)), ::passwd);
					}
					else
					{
						printError("Lua version mismatch\n");
					}
				}
				else
				{
					printError("Lua header mismatch\n");
				}
			}
			catch(Exception^ ex)
			{

			}

			return "";
		}

		String^ LoadLuaCodeFromFile(String^ src)
		{
			String^ data = "";
			FileStream^ f = File::Open(src, FileMode::OpenOrCreate);
			BinaryReader^ breader = gcnew BinaryReader(f);

			String^ header = breader->ReadString();
			int version = breader->ReadInt32();

			if (header->Equals(BINARY_HEADER))
			{
				if (version == BYTECODE_VERSION)
				{
					int len = breader->ReadInt32();
					String^ base64 = Encoding::UTF7->GetString(breader->ReadBytes(len));

					data = CypherLib::DecryptFileContents(Encoding::UTF32->GetString(System::Convert::FromBase64String(base64)), ::passwd);
				}
				else
				{
					printError("Lua version mismatch\n");
				}
			}
			else
			{
				printError("Lua header mismatch\n");
			}

			return data;
		}

	private:
		void RegisterGlobalFunctions()
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


			RegisterGlobal("print", gcnew System::Action<String^>(&Logging::Log));
			RegisterGlobal("warn", gcnew System::Action<String^>(&Logging::LogWarning));
			RegisterGlobal("error", gcnew System::Action<String^>(&Logging::LogError));
			RegisterGlobal("info", gcnew System::Action<String^>(&Logging::LogDebug));
			RegisterGlobal("log", gcnew System::Action<String^, String^>(&Logging::LogCustom));
			RegisterGlobal("HasProperty", gcnew System::Func<Engine::Internal::Components::Object^, String^, bool>(&VMWrapper::HasProperty));
			RegisterGlobal("GetAttributes", gcnew System::Func<Engine::Internal::Components::Object^, AttributeManager^>(&VMWrapper::GetAttributeManager));
			RegisterGlobal("SetProperty", gcnew System::Action<Engine::Internal::Components::Object^, String^, Object^>(&VMWrapper::SetProperty));
			RegisterGlobal("CastToClass", gcnew System::Func<System::Object^, System::String^, System::Object^>(&VMWrapper::ToDerivate));
			RegisterGlobal("ToDerivate", gcnew System::Func<System::Object^, System::Object^>(&VMWrapper::ToDerivate));
			//RegisterGlobal("VM", this->scriptState);
		}

	public:
		void RegisterGlobal(String^ functionName, System::Type^ userData)
		{
			scriptState->Globals[functionName] = userData;
		}


		void RegisterGlobal(String^ functionName, System::Object^ userData)
		{
			scriptState->Globals[functionName] = userData;
		}

		generic <class T>
		void RegisterGlobalFunction(String^ functionName, System::Func<T>^ func)
		{
			scriptState->Globals[functionName] = func;
		}

		generic <class T>
		void RegisterGlobalFunction(String^ functionName, Action<T>^ method)
		{
			scriptState->Globals[functionName] = method;
		}

	public:
		void RegisterScript(String^ source)
		{
			ExecuteSource(source);
		}

	private:
		bool hasFunction(String^ data)
		{
			return scriptState->Globals[data] != nullptr;
		}

		bool hasFunction(MoonSharp::Interpreter::DynValue^ data)
		{
			return scriptState->Globals[data] != nullptr;
		}

	public:
		bool InvokeFunction(String^ functionName)
		{
			try
			{
				if (hasFunction(functionName))
				{
					scriptState->Call(scriptState->Globals[functionName]);
					return true;
				}
			}
			catch (MoonSharp::Interpreter::ScriptRuntimeException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (MoonSharp::Interpreter::InterpreterException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				printError(ex->StackTrace);
			}

			return false;
		}

		bool InvokeFunction(MoonSharp::Interpreter::DynValue^ functionName)
		{
			try
			{
				scriptState->Call(functionName);
				return true;
			}
			catch (MoonSharp::Interpreter::ScriptRuntimeException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (MoonSharp::Interpreter::InterpreterException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				printError(ex->StackTrace);
			}

			return false;
		}

		bool InvokeFunction(String^ functionName, array<System::Object^>^ args)
		{
			try
			{
				if (hasFunction(functionName))
				{
					scriptState->Call(scriptState->Globals[functionName], args);
					return true;
				}
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
			}

			return false;
		}

		bool InvokeFunction(String^ functionName, List<System::Object^>^ args)
		{
			try
			{
				if (hasFunction(functionName))
				{
					scriptState->Call(scriptState->Globals[functionName], args);
					return true;
				}
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
			}

			return false;
		}

		System::Object^ InvokeFunctionO(MoonSharp::Interpreter::DynValue^ functionName)
		{
			try
			{
				return scriptState->Call(functionName);
			}
			catch (MoonSharp::Interpreter::ScriptRuntimeException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (MoonSharp::Interpreter::InterpreterException^ ex)
			{
				printError(ex->Message);
				printError(ex->DecoratedMessage);
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				printError(ex->StackTrace);
			}

			return nullptr;
		}

	private:
		void ExecuteSource(String^ source)
		{
			this->source = source;

			try
			{
				value = scriptState->DoString(source, scriptState->Globals, "GoldEngineLuaThread");
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
			}
		}

		void ExecuteSource(String^ source, String^ friendlyName)
		{
			this->source = source;
			try
			{
				value = scriptState->DoString(source, scriptState->Globals, friendlyName);
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
			}
		}

		DynValue^ RunScript(String^ source)
		{
			this->source = source;

			try
			{
				return scriptState->DoString(source, scriptState->Globals, "GoldEngineLuaThread");
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				return nullptr;
			}
		}

		DynValue^ RunScript(String^ source, String^ friendlyName)
		{
			this->source = source;

			try
			{
				return scriptState->DoString(source, scriptState->Globals, friendlyName);
			}
			catch (Exception^ ex)
			{
				printError(ex->Message);
				return nullptr;
			}
		}
	};
}
