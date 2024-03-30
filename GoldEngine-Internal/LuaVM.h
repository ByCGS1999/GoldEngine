#pragma once

using namespace MoonSharp::Interpreter;

namespace Engine::Lua::VM
{
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
			scriptState = gcnew MoonSharp::Interpreter::Script();

			RegisterGlobalFunctions();
		}

	public:
		void DumpBytecode(String^ fileName)
		{
			if (fileName == nullptr || value == nullptr)
				return;

			File::WriteAllText(fileName, value->ToString());
		}

		void DumpBytecode(DynValue^ value, String^ fileName)
		{
			Stream^ stream = File::Open(fileName, System::IO::FileMode::OpenOrCreate);

			scriptState->Dump(value, stream);

			stream->Close();
		}

	public:
		void WriteLuaCodeToFile(String^ src)
		{
			FileStream^ f = File::Open(src, FileMode::OpenOrCreate);
			BinaryWriter^ bwriter = gcnew BinaryWriter(f);

			bwriter->Write("GoldVM");
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
			UserData::RegisterAssembly(System::Reflection::Assembly::GetCallingAssembly(), true);

			//scriptState->Globals["Logging"] = Engine::Scripting::Logging::typeid;
			RegisterGlobal("Logging", Engine::Scripting::Logging::typeid);
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

	public:
		void InvokeFunction(String^ functionName)
		{
			if(hasFunction(functionName))
				scriptState->Call(scriptState->Globals[functionName]);
		}

		void InvokeFunction(String^ functionName, array<System::Object^>^ args)
		{
			if (hasFunction(functionName))
				scriptState->Call(scriptState->Globals[functionName], args);
		}

		void InvokeFunction(String^ functionName, List<System::Object^>^ args)
		{
			if (hasFunction(functionName))
				scriptState->Call(scriptState->Globals[functionName], args);
		}

	public:
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
