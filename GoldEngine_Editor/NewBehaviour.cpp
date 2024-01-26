#include <cstdio>
#include "Keys.h"

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;

namespace UserScripts
{
	ref class NewBehaviour : public Engine::EngineObjects::Script
	{
	public:
		int increment = 1.0f;
		float smoothness = 0.15f;

	public:
		NewBehaviour(System::String^ name, Engine::Internal::Components::Transform^ transform) : Script(name, transform)
		{

		}

	public:
		void Update() override
		{
			Engine::Internal::Components::Object^ object = ObjectManager::singleton()->GetFirstObjectOfName("Cube"); // Obtain the first object with the name 'Cube'

			if (object != nullptr)
			{
				auto trans = object->GetTransform();

				// Translate its position depending on the keypress (W - UP) - (S - DOWN);

				if (InputManager::IsKeyDown(KEY_W))
				{
					trans->position->Add(Vector3::Lerp(transform->position, gcnew Vector3(0, increment, 0), smoothness));
				}
				else if(InputManager::IsKeyDown(KEY_S))
				{
					trans->position->Sub(Vector3::Lerp(transform->position, gcnew Vector3(0, increment, 0), smoothness));
				}
			}
		}

		void Draw() override
		{

		}
	};

	ref class CustomRenderer : public Engine::Internal::Components::Object
	{
	public:
		CustomRenderer(System::String^ name, Engine::Internal::Components::Transform^ transform) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::Script)
		{

		}

	};
}