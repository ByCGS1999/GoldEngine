
#include "Typedefs.h"
#include <Windows.h>

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;


namespace EditorScripts
{
	public ref class NewBehaviour : Engine::EngineObjects::Script
	{
	private:
		bool isOpen = true;

	public:
		NewBehaviour(System::String^ name, Transform^ transform) : Script(name, transform)
		{

		}

		void Start() override
		{
			Logging::Log("Hello world!");
		}

		void DrawImGUI() override
		{
			if (isOpen)
			{
				if (ImguiHook::ImGui_begin("Test Window", isOpen))
				{
					ImguiHook::ImGui_text("Hello world!");

					ImguiHook::ImGui_end();
				}
			}
		}

	};

	public ref class TestPreload : Engine::Preload::IPreloadScript
	{
	public:
		static void Preload()
		{
			Engine::Internal::DataManager::HL_LoadModel(1, "./Data/Game/Models/Spaceship.obj"); // loading assets at runtime
		}
	};
}