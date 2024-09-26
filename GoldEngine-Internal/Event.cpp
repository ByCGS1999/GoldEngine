#include "Event.h"

using namespace Engine::Scripting::Events;

Event::Event()
{
	this->pInvokable = nullptr;
	this->isLuaFunction = false;
	this->isAction = false;
	this->isDelegate = false;
}

void Event::connect(System::Delegate^ delegate)
{
	this->pInvokable = delegate;
	this->isLuaFunction = false;
	this->isAction = false;
	this->isDelegate = true;
}

void Event::connect(System::Action^ delegate)
{
	this->pInvokable = delegate;
	this->isLuaFunction = false;
	this->isDelegate = false;
	this->isAction = true;
}

void Event::connect(MoonSharp::Interpreter::DynValue^ function)
{
	if (function->Type == MoonSharp::Interpreter::DataType::Function)
	{
		this->pInvokable = function->Function->GetDelegate();
		this->isLuaFunction = true;
		this->isAction = false;
		this->isDelegate = false;
	}
}

void Event::disconnect()
{
	this->pInvokable = nullptr;
	this->isLuaFunction = false;
	this->isAction = false;
	this->isDelegate = false;
}

System::Object^ Event::invoke()
{
	if (pInvokable == nullptr)
		return nullptr;

	if (isLuaFunction)
	{
		MoonSharp::Interpreter::ScriptFunctionDelegate^ delegate = (MoonSharp::Interpreter::ScriptFunctionDelegate^)pInvokable;

		delegate->Invoke();
	}
	else if (isDelegate)
	{
		System::Delegate^ delegate = (System::Delegate^)pInvokable;

		delegate->DynamicInvoke();
	}
	else if (isAction)
	{
		System::Action^ delegate = (System::Action^)pInvokable;

		delegate->DynamicInvoke();
	}
}

System::Object^ Event::invoke(cli::array<System::Object^>^ objects)
{
	if (pInvokable == nullptr)
		return nullptr;

	if (isLuaFunction)
	{
		MoonSharp::Interpreter::ScriptFunctionDelegate^ delegate = (MoonSharp::Interpreter::ScriptFunctionDelegate^)pInvokable;

		return delegate->Invoke(objects);
	}
	else if(isDelegate)
	{
		System::Delegate^ delegate = (System::Delegate^)pInvokable;

		return delegate->DynamicInvoke(objects);
	}
	else if (isAction)
	{
		System::Action^ delegate = (System::Action^)pInvokable;

		return delegate->DynamicInvoke(objects);
	}
}

System::Object^ Event::raiseExecution(cli::array<System::Object^>^ objects)
{
	return invoke(objects);
}
