#pragma once

#if !defined(PRODUCTION_BUILD)

namespace Engine::EngineObjects::Editor
{
	public ref class EditorCamera : public Engine::EngineObjects::Camera3D
	{
	public:
		EditorCamera(String^ name, Engine::Internal::Components::Transform^ parent) : Engine::EngineObjects::Camera3D(name, parent)
		{

		}

		[Engine::Scripting::ExecuteInEditModeAttribute]
		void Update() override
		{
			this->transform->position = Engine::Components::Vector3::create(((Native::NativeCamera3D*)this->get())->get().position);
			
			if (attributes->hasAttribute("camera direction"))
			{
				attributes->getAttribute("camera direction")->setValue(Engine::Components::Vector3::create(((Native::NativeCamera3D*)this->get())->get().target));
			}

		}
	};
}

#endif