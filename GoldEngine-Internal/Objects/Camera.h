#include "../Object/LayerManager.h"
#include <memory>

using namespace Engine::Internal::Components;

namespace Engine::EngineObjects
{
	namespace Native
	{
		private class NativeCamera
		{
		private:
			RAYLIB::Camera* camera;

		public:
			RAYLIB::Camera* getCameraPtr()
			{
				return this->camera;
			}

			RAYLIB::Camera get()
			{
				return *this->camera;
			}

		public:
			NativeCamera(CameraProjection projection)
			{
				printf("Creating camera\n");
				this->camera = new RAYLIB::Camera();
				this->camera->projection = projection;
				this->camera->fovy = 90;
				this->camera->position = { 0,0,0 };
				this->camera->target = { 0, 0, 1 };
				this->camera->up = { 0, 1, 0 };
			}

		public:
			void setCameraPosition(RAYLIB::Vector3 nativeVector)
			{
				camera->position = nativeVector;
			}
			
		public:
			void setCameraTarget(RAYLIB::Vector3 nativeVector)
			{
				camera->target = nativeVector;
			}

		};
	}

	public ref class Camera : public Engine::EngineObjects::Script
	{
	public:
		[Newtonsoft::Json::JsonIgnoreAttribute]
		Native::NativeCamera* nativeCamera;
		CameraProjection cameraProjection;

	private:
		bool isMainCamera;

	public:
		Camera(String^ name, Engine::Internal::Components::Transform^ trans, CameraProjection projection) : Engine::EngineObjects::Script(name, trans)
		{
			cameraProjection = projection;

			nativeCamera = new Native::NativeCamera(cameraProjection);

			if (!attributes->getAttribute("IsMainCamera"))
			{
				attributes->addAttribute(Engine::Scripting::Attribute::create("IsMainCamera", false, bool::typeid));
			}
			else
			{
				isMainCamera = attributes->getAttribute("IsMainCamera")->getValue<bool>();
			}
		}

		void SetCameraTarget(Engine::Components::Vector3^ target)
		{
			nativeCamera->setCameraTarget(target->toNative());
		}

		void Update() override
		{

		}

		void DrawGizmo() override
		{
			Engine::Components::Vector3^ fwd = gcnew Engine::Components::Vector3(0,0,0);
			fwd->copy(transform->forward);
			DrawLine3D(transform->position->toNative(), fwd->toNative(), GetColor(0xFF0000FF));
		}

		RAYLIB::Camera* get()
		{
			return nativeCamera->getCameraPtr();
		}
	};
}