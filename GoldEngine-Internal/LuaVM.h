#pragma once

using namespace MoonSharp::Interpreter;

namespace Engine::Lua::VM
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class TypeWrapper // Lua Wrapper for System::Type^
	{
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

		static System::Object^ ToDerivate(System::Object^ object)
		{
			Logging::Log("Changing object type");
			return System::Convert::ChangeType(object, object->GetType());
		}

		static System::Object^ ToDerivate(System::Object^ object, System::Type^ targetType)
		{
			return System::Convert::ChangeType(object, targetType);
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

			bwriter->Write(System::Convert::ToBase64String(Encoding::UTF32->GetBytes(CypherLib::EncryptFileContents(source, ::passwd))));

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
					String^ base64 = breader->ReadString();

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
						String^ base64 = breader->ReadString();

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
					String^ base64 = breader->ReadString();

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
			auto appDomain = AppDomain::CurrentDomain;

			//UserData::RegisterAssembly(assembly, true);

			for each (auto asms in appDomain->GetAssemblies())
			{
				UserData::RegisterAssembly(asms, true);
			}
			
			RegisterGlobal("Logging", Engine::Scripting::Logging::typeid);
			RegisterGlobal("Attribute", Engine::Scripting::Attribute::typeid);
			RegisterGlobal("DataManager", Engine::Internal::DataManager::typeid);
			RegisterGlobal("ObjectManager", Engine::Scripting::ObjectManager::singleton());
			RegisterGlobal("Input", Engine::Scripting::InputManager::typeid);
			RegisterGlobal("KeyCode", Engine::Scripting::KeyCodes::typeid);
			RegisterGlobal("SharedInstance", SharedInstance::typeid);
			RegisterGlobal("TypeWrapper", TypeWrapper::typeid);
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
