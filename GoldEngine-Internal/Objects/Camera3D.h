
namespace Engine::EngineObjects
{
	public ref class Camera3D : public Engine::EngineObjects::Script
	{
	public:
		::Camera3D* camera;
		CameraProjection cameraProjection;

	public:
		Camera3D(String^ name, Engine::Internal::Components::Transform^ trans) : Engine::EngineObjects::Script(name, trans)
		{
			cameraProjection = CameraProjection::CAMERA_PERSPECTIVE;
			camera = new Camera();

			
		}

		void Update() override
		{
			camera->position = transform->position->toNative();
			UpdateCamera(camera, cameraProjection);
		}

		void DrawGizmo() override
		{
			Components::Vector3^ fwd = gcnew Components::Vector3(0,0,0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), fwd->toNative(), RED);
		}
	};
}