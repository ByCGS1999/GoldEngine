
namespace Engine::EngineObjects
{
	public ref class Camera3D : public Engine::EngineObjects::Camera
	{
	private: 
		Native::NativeCamera3D* nativeCamera;

	public:
		Camera3D(String^ name, Engine::Internal::Components::Transform^ trans) : Engine::EngineObjects::Camera(name, trans, CameraProjection::CAMERA_PERSPECTIVE)
		{
			nativeCamera = new Native::NativeCamera3D((RAYLIB::CameraProjection)((int)cameraProjection));

			if (!attributes->getAttribute("camera direction"))
			{
				attributes->addAttribute("camera direction", gcnew Engine::Components::Vector3(0, 0, 1));
			}
		}

		[Engine::Attributes::ExecuteInEditModeAttribute]
		void Update() override
		{
 			if (!attributes->getAttribute("camera direction"))
			{
				attributes->addAttribute("camera direction", gcnew Engine::Components::Vector3(0, 0, 1));
			}

			if (cameraMode == CamMode::CAMERA_CUSTOM)
			{
				if (attributes->getAttribute("camera direction"))
					nativeCamera->get().target = ((Engine::Components::Vector3^)attributes->getAttribute("camera direction")->getValue())->toNative();

				nativeCamera->getCameraPtr()->position = transform->position->toNative();
			}
			
			UpdateCamera(nativeCamera->getCameraPtr(), (int)cameraMode);
		}

		void DrawGizmo() override
		{
			Engine::Components::Vector3^ fwd = gcnew Engine::Components::Vector3(0,0,0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), nativeCamera->get().target, GetColor(0xFF0000FF));
		}

		void setTarget(Engine::Components::Vector3^ target) override
		{
			this->nativeCamera->setCameraTarget(target->toNative());
		}

		void* get() override
		{
			return (void*)nativeCamera;
		}

		bool is3DCamera() override
		{
			return true;
		}
	};
}