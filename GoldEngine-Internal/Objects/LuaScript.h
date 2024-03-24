#pragma once

namespace Engine::EngineObjects
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
		public ref class LuaScript : public Engine::EngineObjects::Script
	{
	private:
		Engine::Lua::VM::LuaVM^ virtualMachine;
	public:
		String^ luaSource;

	public:
		LuaScript(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::Script(name, transform)
		{
			if (luaSource == nullptr)
				luaSource = "function Start()\nprint(\"Hello world!\");\nend";
			
			virtualMachine = gcnew Engine::Lua::VM::LuaVM();
			virtualMachine->RegisterGlobal("script", this->GetType());
			virtualMachine->RegisterScript(luaSource);

			Start();
		}
		
		void Start() override
		{
			virtualMachine->InvokeFunction("Call");
		}

		void Draw() override
		{
			virtualMachine->InvokeFunction("Draw");
		}

		void Update() override
		{
			virtualMachine->InvokeFunction("Update");
		}

		void PhysicsUpdate() override 
		{
			virtualMachine->InvokeFunction("PhysicsUpdate");
		}

		void DrawGizmo() override 
		{
			virtualMachine->InvokeFunction("DrawGizmo");
		}

		void DrawImGUI() override
		{
			virtualMachine->InvokeFunction("DrawImGUI");
		}
	};
}