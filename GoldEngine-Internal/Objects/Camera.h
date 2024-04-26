#include "../Object/LayerManager.h"

using namespace Engine::Internal::Components;

namespace Engine::EngineObjects
{
	namespace Native
	{
		public class NativeCamera
		{
		private:
			::Camera camera;

		public:
			NativeCamera(CameraProjection projection)
			{
				printf("Creating camera");
				this->camera = {};
				//this->camera.projection = projection;
			}

		public:
			void setCameraPosition(::Vector3 nativeVector)
			{
				camera.position = nativeVector;
			}

		public:
			::Camera* getCameraPtr()
			{
				return &this->camera;
			}

			::Camera getCamera()
			{
				return this->camera;
			}
		};
	}

	public ref class Camera : public Engine::EngineObjects::Script
	{
	public:
		Native::NativeCamera* nativeCamera;
		CameraProjection cameraProjection;

	public:
		Camera(String^ name, Engine::Internal::Components::Transform^ trans, CameraProjection projection) : Engine::EngineObjects::Script(name, trans)
		{
			this->layerMask = LayerManager::GetLayerFromId(0);
			cameraProjection = projection;
			//nativeCamera = new Native::NativeCamera(cameraProjection);
			
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

	public:
		::Camera* getCamera()
		{
			return nativeCamera->getCameraPtr();
		}
	};
}