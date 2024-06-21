#pragma once

namespace Engine::Signals
{
	generic <typename T>
	ref class ManagedSignal;
}

namespace Engine::Managers
{
	ref class SignalManager;
}

namespace Engine::Signals
{
	public delegate void SimpleSignal();
	
	public delegate bool LuaSignal(System::String^);

	public delegate bool LuaSignal_V2();

	generic<typename T>
	public delegate T SignalWithArgs(cli::array<System::Object^>^);

	generic<typename T>
	public delegate T SignalWithArg(System::Object^);

	generic<typename T>
	public delegate T Signal();

	generic <typename T>
	public ref class ManagedSignal
	{
	private:
		System::Delegate^ bindedSignal;

		void Callback()
		{
			if (bindedSignal != nullptr)
			{
				bindedSignal->DynamicInvoke();
			}
		}

		void Callback(System::Object^ arg)
		{
			if (bindedSignal != nullptr)
			{
				bindedSignal->DynamicInvoke(arg);
			}
		}

		void Callback(cli::array<System::Object^>^ args)
		{
			if (bindedSignal != nullptr)
			{
				bindedSignal->DynamicInvoke(args);
			}
		}

		Object^ invoker;

	public:
		ManagedSignal(Engine::Managers::SignalManager^ signalManager)
		{
			if (signalManager == nullptr)
				throw gcnew System::Exception("Cannot instantiate a signal without a signal manager");
		}

		~ManagedSignal()
		{

		}

	public:
		void Bind(Signal<T>^ signal)
		{
			bindedSignal = signal;
		}

		void Bind(SignalWithArg<T>^ signal)
		{
			bindedSignal = signal;
		}

		void Bind(SignalWithArgs<T>^ signal)
		{
			bindedSignal = signal;
		}

		void Bind(SimpleSignal^ signal)
		{
			bindedSignal = signal;
		}

		void Bind(Engine::Lua::VM::LuaVM^ vmInstance, System::String^ funcName)
		{
			DynValue^ temp = vmInstance->GetScriptState()->Globals->Get(funcName);
			invoker = funcName;

			if (temp->Type == DataType::Function)
			{
				LuaSignal^ signal = gcnew LuaSignal(vmInstance, &Engine::Lua::VM::LuaVM::InvokeFunction);
			}
		}
		
		void Bind(Engine::Lua::VM::LuaVM^ vmInstance, DynValue^ temp)
		{
			invoker = temp;

			if (temp->Type == DataType::Function)
			{
				LuaSignal^ signal = gcnew LuaSignal(vmInstance, &Engine::Lua::VM::LuaVM::InvokeFunction);
			}
		}

		void Call() 
		{
			Callback();
		}

		void Call(System::Object^ arg)
		{
			Callback(arg);
		}

		void Call(cli::array<System::Object^>^ args)
		{
			Callback(args);
		}

		void CallUsingInvoker()
		{
			Callback(invoker);
		}

		void DisposeSignal()
		{
			bindedSignal = nullptr;
			delete this;
		}
	};

}


namespace Engine::Managers
{
	ref class SignalManager
	{
	private:
		System::Collections::Generic::List<System::Object^>^ signals;

	public:
		SignalManager() 
		{
			signals = gcnew System::Collections::Generic::List<System::Object^>();
			Instance = this;
		}

	public:
		generic <class T>
		Engine::Signals::ManagedSignal<T>^ GetSignalByIndex(int index)
		{
			return (Engine::Signals::ManagedSignal<T>^)signals[index];
		}

		generic <class T>
		int GetIndexBySignal(Engine::Signals::ManagedSignal<T>^ signal)
		{
			return signals->IndexOf(signal);
		}

		bool RemoveSignal(System::Object^ signal)
		{
			return signals->Remove(signal);
		}

		generic <class T>
		Engine::Signals::ManagedSignal<T>^ CreateSignal()
		{
			auto signal = gcnew Engine::Signals::ManagedSignal<T>(this);
			signals->Add(signal);
			return signal;
		}

		generic <class T>
		System::Tuple<Engine::Signals::ManagedSignal<T>^, int>^ CreateIndexedSignal()
		{
			int newIndex = signals->Count;
			auto signal = gcnew Engine::Signals::ManagedSignal<T>(this);
			signals->Insert(newIndex-1, signal);

			return System::Tuple::Create<Engine::Signals::ManagedSignal<T>^, int>(signal, newIndex);
		}

		static SignalManager^ Instance;
	};
}
