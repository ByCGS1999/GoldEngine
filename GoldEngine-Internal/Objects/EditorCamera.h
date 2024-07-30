#pragma once

namespace Engine::EngineObjects
{
	public ref class EditorCamera : public Engine::EngineObjects::Camera3D
	{
	public:
		EditorCamera(String^ name, Engine::Internal::Components::Transform^ parent) : Engine::EngineObjects::Camera3D(name, parent)
		{

		}

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