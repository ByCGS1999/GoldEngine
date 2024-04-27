
#include "Typedefs.h"
#include <Windows.h>

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;


namespace EditorScripts
{
	public ref class SampleBehaviour : public Engine::EngineObjects::Script
	{
	public:
		SampleBehaviour(System::String^ objectName, Transform^ transformComponent) : Engine::EngineObjects::Script(objectName, transformComponent) { };

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
	};
}