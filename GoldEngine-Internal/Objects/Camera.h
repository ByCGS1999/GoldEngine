#include "../Object/LayerManager.h"
#include <memory>

using namespace Engine::Internal::Components;

namespace Engine::EngineObjects
{
	namespace Native
	{
		public class NativeCamera3D
		{
		private:
			RAYLIB::Camera camera;

		public:
			RAYLIB::Camera* getCameraPtr()
			{
				return &this->camera;
			}

			RAYLIB::Camera& get()
			{
				return this->camera;
			}

		public:
			NativeCamera3D(CameraProjection projection)
			{
				printf("Creating camera\n");
				this->camera = RAYLIB::Camera();
				this->camera.projection = projection;
				this->camera.fovy = 90;
				this->camera.position = { 0,0,0 };
				this->camera.target = { 0, 0, 1 };
				this->camera.up = { 0, 1, 0 };
			}

		public:
			void setCameraPosition(RAYLIB::Vector3 nativeVector)
			{
				camera.position = nativeVector;
			}
			
		public:
			void setCameraTarget(RAYLIB::Vector3 nativeVector)
			{
				camera.target = nativeVector;
			}

		};
	}

	public ref class Camera abstract : public Engine::EngineObjects::Script
	{
	public:
		[Newtonsoft::Json::JsonIgnoreAttribute]
		CameraProjection cameraProjection;

	private:
		bool isMainCamera;

	public:
		Camera(String^ name, Engine::Internal::Components::Transform^ trans, CameraProjection projection) : Engine::EngineObjects::Script(name, trans)
		{
			cameraProjection = projection;

			if (!attributes->getAttribute("IsMainCamera"))
			{
				if (!SharedInstance::ExistsInstance("CameraExists"))
				{
					attributes->addAttribute(Engine::Scripting::Attribute::create("IsMainCamera", true, bool::typeid));
					SharedInstance::Create("CameraExists", true);
				}
				else
				{
					attributes->addAttribute(Engine::Scripting::Attribute::create("IsMainCamera", false, bool::typeid));
				}
			}
			else
			{
				isMainCamera = attributes->getAttribute("IsMainCamera")->getValue<bool>();
			}
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

	public:
		virtual bool is3DCamera() abstract;
		virtual void* get() abstract;
	};
}