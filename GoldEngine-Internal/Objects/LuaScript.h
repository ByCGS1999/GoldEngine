#pragma once

namespace Engine::EngineObjects
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class LuaScript : public Engine::EngineObjects::Script
	{
	private:
		Engine::Lua::VM::LuaVM^ virtualMachine;
		String^ luaSource = "";
	public:
		String^ luaFilePath = "./";

	public:
		LuaScript(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::Script(name, transform)
		{
			initVM();
		}

	public:
		void Reset()
		{
			initVM();
		}

	private:
		void initVM()
		{
			if (File::Exists(luaFilePath))
			{
				virtualMachine = gcnew Engine::Lua::VM::LuaVM();

				luaSource = virtualMachine->ReadFromFile(luaFilePath);

				virtualMachine->RegisterGlobal("script", this);
				virtualMachine->RegisterGlobal("attributes", attributes);
				virtualMachine->RegisterGlobal("Attribute", Engine::Scripting::Attribute::typeid);
				virtualMachine->RegisterGlobal("DataManager", Engine::Internal::DataManager::typeid);
				virtualMachine->RegisterGlobal("ObjectManager", Engine::Scripting::ObjectManager::singleton());
				virtualMachine->RegisterGlobal("Input", Engine::Scripting::InputManager::typeid);

				virtualMachine->RegisterScript(luaSource);
			}
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
						printError("lua source is nullptr, wtf is going on\n");
				}
				else
				{
					printError("luasrc is not a valid instance\n");
				}

				if (virtualMachine != nullptr)
				{
					delete virtualMachine;
				}

				initVM();
			}
			catch (Exception^ ex)
			{
				printError("Failed converting object to gc_luascript\n");
			}
		}

		void Start() override
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("Start");
		}

		void Draw() override
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("Draw");
		}

		void Update() override
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("Update");
		}

		void PhysicsUpdate() override 
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("PhysicsUpdate");
		}

		void DrawGizmo() override 
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("DrawGizmo");
		}

		void DrawImGUI() override
		{
			if (virtualMachine == nullptr)
				initVM();

			if (virtualMachine != nullptr)
				virtualMachine->InvokeFunction("DrawImGUI");
		}
	};
}