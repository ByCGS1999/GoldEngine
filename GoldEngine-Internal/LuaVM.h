#pragma once

using namespace MoonSharp::Interpreter;

namespace Engine::Lua::VM
{
	public ref class LuaVM
	{
	private:
		MoonSharp::Interpreter::Script^ scriptState;

	private:
		DynValue^ value;

	public:
		LuaVM()
		{
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
			value = scriptState->DoString(source, scriptState->Globals, "GoldEngineLuaThread");
		}

		void ExecuteSource(String^ source, String^ friendlyName)
		{
			value = scriptState->DoString(source, scriptState->Globals, friendlyName);
		}

		DynValue^ RunScript(String^ source)
		{
			return scriptState->DoString(source, scriptState->Globals, "GoldEngineLuaThread");
		}

		DynValue^ RunScript(String^ source, String^ friendlyName)
		{
			return scriptState->DoString(source, scriptState->Globals, friendlyName);
		}
	};
}
