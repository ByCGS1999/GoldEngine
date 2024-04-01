
#include "Typedefs.h"

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;


namespace EditorScripts
{
	public ref class NewBehaviour : Engine::EngineObjects::Script
	{
	public:
		NewBehaviour(System::String^ name, Transform^ transform) : Script(name, transform)
		{

		}

		void Start() override
		{
			Logging::Log("Hello world!");
		}

	};
}