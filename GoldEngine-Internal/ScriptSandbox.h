#include "Includes.h"
#include "ScriptBehaviour.h"
#include <vector>

using namespace System;
using namespace System::Collections;

namespace Engine::Scripting
{
	public ref class ScriptSandbox
	{
	public:
		ArrayList^ scripts;

	public:
		void AddScript(Scripting::ScriptBehaviour^ behaviour)
		{
			scripts->Add(behaviour);
			behaviour->Start();
		}

	public:
		void ExecuteUpdate()
		{
			for each (Object ^ src in scripts)
			{
				ScriptBehaviour^ behaviour = reinterpret_cast<ScriptBehaviour^>(src);
				behaviour->Update();
			}
		}

	public:
		void ExecuteFixedUpdate()
		{
			for each (Object ^ src in scripts)
			{
				ScriptBehaviour^ behaviour = reinterpret_cast<ScriptBehaviour^>(src);
				behaviour->FixedUpdate();
			}
		}

	public:
		Scripting::ScriptSandbox()
		{
			scripts = gcnew ArrayList(0);
		}
	};
}

