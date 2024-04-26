
namespace Engine::EngineObjects
{
	public ref class Camera3D : public Engine::EngineObjects::Camera
	{

	public:
		Camera3D(String^ name, Engine::Internal::Components::Transform^ trans) : Engine::EngineObjects::Camera(name, trans, CameraProjection::CAMERA_PERSPECTIVE)
		{

		}

		void Update() override
		{
			nativeCamera->getCameraPtr()->position = transform->position->toNative();
			UpdateCamera(nativeCamera->getCameraPtr(), cameraProjection);
		}

		void DrawGizmo() override
		{
			Engine::Internal::Components::Vector3^ fwd = gcnew Engine::Internal::Components::Vector3(0,0,0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), fwd->toNative(), RED);
		}

	};
}