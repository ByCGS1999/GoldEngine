#pragma once

namespace Engine::EngineObjects
{
	public ref class Camera2D : public Engine::EngineObjects::Camera
	{
	private:
		Native::NativeCamera3D* nativeCamera;

	public:
		Camera2D(String^ name, Engine::Internal::Components::Transform^ trans) : Engine::EngineObjects::Camera(name, trans, CameraProjection::CAMERA_ORTHOGRAPHIC)
		{
			nativeCamera = new Native::NativeCamera3D(cameraProjection);

			if (!attributes->getAttribute("camera direction"))
			{
				attributes->addAttribute("camera direction", gcnew Engine::Components::Vector3(0, 0, 1));
			}
		}

		void Update() override
		{
			if (!attributes->getAttribute("camera direction"))
			{
				attributes->addAttribute("camera direction", gcnew Engine::Components::Vector3(0, 0, 1));
			}

			if (attributes->getAttribute("camera direction"))
				nativeCamera->get().target = ((Engine::Components::Vector3^)attributes->getAttribute("camera direction")->getValue())->toNative();

#if !defined(PRODUCTION_BUILD)
			nativeCamera->getCameraPtr()->position = transform->position->toNative();
			UpdateCamera(nativeCamera->getCameraPtr(), CAMERA_CUSTOM);
#else
			nativeCamera->getCameraPtr()->position = transform->position->toNative();
			UpdateCamera(nativeCamera->getCameraPtr(), cameraProjection);
#endif
		}

		void DrawGizmo() override
		{
			Engine::Components::Vector3^ fwd = gcnew Engine::Components::Vector3(0, 0, 0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), nativeCamera->get().target, GetColor(0xFF0000FF));
		}

		void* get() override
		{
			return (void*)nativeCamera;
		}

		bool is3DCamera() override
		{
			return false;
		}
	};
}