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
		this->pInvokable = function->Function;
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

void Event::invoke()
{
	if (pInvokable == nullptr)
		return;

	if (isLuaFunction)
	{
		MoonSharp::Interpreter::Closure^ closure = (MoonSharp::Interpreter::Closure^)pInvokable;
		closure->Call();
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

void Event::invoke(cli::array<System::Object^>^ objects)
{
	if (pInvokable == nullptr)
		return;

	if (isLuaFunction)
	{
		MoonSharp::Interpreter::Closure^ closure = (MoonSharp::Interpreter::Closure^)pInvokable;
		closure->Call(objects);
	}
	else if(isDelegate)
	{
		System::Delegate^ delegate = (System::Delegate^)pInvokable;

		delegate->DynamicInvoke(objects);
	}
	else if (isAction)
	{
		System::Action^ delegate = (System::Action^)pInvokable;

		delegate->DynamicInvoke(objects);
	}
}

void Event::raiseExecution(cli::array<System::Object^>^ objects)
{
	if (pInvokable == nullptr)
		return;

	if (isLuaFunction)
	{
		MoonSharp::Interpreter::Closure^ closure = (MoonSharp::Interpreter::Closure^)pInvokable;
		closure->Call(objects);
	}
	else if(isDelegate)
	{
		System::Delegate^ delegate = (System::Delegate^)pInvokable;

		delegate->DynamicInvoke(objects);
	}
	else if (isAction)
	{
		System::Action^ delegate = (System::Action^)pInvokable;

		delegate->DynamicInvoke(objects);
	}
}