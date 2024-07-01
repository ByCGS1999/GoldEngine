
#include "Typedefs.h"

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;


namespace EditorScripts
{
	public ref class RuntimeConsole : public Engine::EngineObjects::Script
	{
	public:
		RuntimeConsole(System::String^ objectName, Engine::Internal::Components::Transform^ transformComponent) : Engine::EngineObjects::Script(objectName, transformComponent) { };

	public:
		void Start() override
		{

		}

		void Draw() override
		{

		}

		void Update() override
		{

		}

		void DrawImGUI() override
		{
			if (ImguiHook::ImGui_begin_window("Runtime Console", true))
			{
				auto windowSize = ImguiHook::ImGui_get_windowsize();
				windowSize->x -= 40;
				windowSize->y -= 50;

				if (ImguiHook::ImGui_begin_listbox("###LOGS", windowSize))
				{
					for each (Engine::Scripting::Log^ x in Engine::Scripting::Logging::getLogs())
					{
						ImguiHook::ImGui_text(x->message);
					}

					ImguiHook::ImGui_end_listbox();
				}

				ImguiHook::ImGui_end_window();
			}
		}
	};
}