#pragma once

namespace Engine::Scripting::Events
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Event
	{
	private:
		System::Object^ pInvokable;
		bool isLuaFunction = false;
		bool isDelegate = false;
		bool isAction = false;

	public:
		Event();

	public:
		void connect(System::Delegate^);
		void connect(System::Action^);
		void connect(MoonSharp::Interpreter::DynValue^);

		void disconnect();


	public:
		void invoke(cli::array<System::Object^>^);
		void invoke();
		void raiseExecution(cli::array<System::Object^>^);
	};
}