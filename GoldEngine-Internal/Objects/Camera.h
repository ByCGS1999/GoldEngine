#include "../Object/LayerManager.h"
#include <memory>

using namespace Engine::Internal::Components;

namespace Engine::EngineObjects
{
	namespace Native
	{
		public class NativeCamera
		{
		private:
			::Camera* camera;

		public:
			::Camera* getCameraPtr()
			{
				return this->camera;
			}

			::Camera get()
			{
				return *this->camera;
			}

		public:
			NativeCamera(CameraProjection projection)
			{
				printf("Creating camera\n");
				this->camera = new ::Camera();
				this->camera->projection = projection;
				this->camera->fovy = 90;
				this->camera->position = { 0,0,0 };
				this->camera->target = { 0, 0, 1 };
				this->camera->up = { 0, 1, 0 };
			}

		public:
			void setCameraPosition(::Vector3 nativeVector)
			{
				camera->position = nativeVector;
			}

		};
	}

	public ref class Camera : public Engine::EngineObjects::Script
	{
	public:
		[Newtonsoft::Json::JsonIgnoreAttribute]
		Native::NativeCamera* nativeCamera;
		CameraProjection cameraProjection;

	public:
		Camera(String^ name, Engine::Internal::Components::Transform^ trans, CameraProjection projection) : Engine::EngineObjects::Script(name, trans)
		{
			cameraProjection = projection;

			nativeCamera = new Native::NativeCamera(cameraProjection);
		}

		void Update() override
		{
			UpdateCamera(nativeCamera->getCameraPtr(), cameraProjection);
		}

		void DrawGizmo() override
		{
			Engine::Components::Vector3^ fwd = gcnew Engine::Components::Vector3(0,0,0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), fwd->toNative(), GetColor(0xFF0000FF));
		}

		::Camera* get()
		{
			return nativeCamera->getCameraPtr();
		}
	};
}