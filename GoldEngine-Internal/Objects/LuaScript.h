#pragma once

namespace Engine::EngineObjects
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class LuaScript : public Engine::EngineObjects::Script
	{
	private:
		Engine::Lua::VM::LuaVM^ virtualMachine;
	public:
		String^ luaSource = "";

	public:
		LuaScript(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::Script(name, transform)
		{
			initVM();
		}

	private:
		void initVM()
		{
			virtualMachine = gcnew Engine::Lua::VM::LuaVM();
			virtualMachine->RegisterGlobal("script", this);
			virtualMachine->RegisterScript(luaSource);
		}

	public:
		void LoadSource(String^ source)
		{
			luaSource = source;
		}

		void LoadFromFile(String^ file)
		{
			luaSource = File::ReadAllText(file);
		}

	public:
		void Init(Object^ object) override
		{
			try
			{
				LuaScript^ luaSrc = Cast::Dynamic<LuaScript^>(object);

				if (luaSrc != nullptr)
				{
					if (luaSrc->luaSource != nullptr)
						this->luaSource = luaSrc->luaSource;
					else
						print("lua source is nullptr, wtf is going on\n");
				}
				else
				{
					print("luasrc is not a valid instance\n");
				}

				if (virtualMachine != nullptr)
				{
					delete virtualMachine;
				}

				initVM();
			}
			catch (Exception^ ex)
			{
				print("Failed converting object to gc_luascript\n");
			}
		}

		void Start() override
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("Start");
		}

		void Draw() override
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("Draw");
		}

		void Update() override
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("Update");
		}

		void PhysicsUpdate() override 
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("PhysicsUpdate");
		}

		void DrawGizmo() override 
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("DrawGizmo");
		}

		void DrawImGUI() override
		{
			if (virtualMachine == nullptr)
				initVM();

			virtualMachine->InvokeFunction("DrawImGUI");
		}
	};
}