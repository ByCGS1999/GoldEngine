#pragma once

namespace Engine::Scripting::Events
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Event
	{
	private:
		delegate void LuaEvent(cli::array<MoonSharp::Interpreter::DynValue^>^);
		System::Object^ pInvokable;
		bool isLuaFunction = false;
		bool isDelegate = false;
		bool isAction = false;
		MoonSharp::Interpreter::Script^ scriptOwner;

	public:
		Event();
		static Event^ Create() { return gcnew Event(); }
		static Event^ New() { return gcnew Event(); }

	public:
		void connect(System::Delegate^);
		void connect(System::Action^);
		void connect(MoonSharp::Interpreter::DynValue^);

		void disconnect();


	public:
		System::Object^ invoke(cli::array<System::Object^>^);
		System::Object^ invoke();
		System::Object^ raiseExecution(cli::array<System::Object^>^);
	};
}