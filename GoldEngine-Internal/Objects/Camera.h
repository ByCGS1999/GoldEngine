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

	public enum class CamProjection
	{
		CAMERA_PERSPECTIVE = 0,         // Perspective projection
		CAMERA_ORTHOGRAPHIC             // Orthographic projection
	};

	public enum class CamMode
	{
		CAMERA_CUSTOM = 0,              // Camera custom, controlled by user (UpdateCamera() does nothing)
		CAMERA_FREE,                    // Camera free mode
		CAMERA_ORBITAL,                 // Camera orbital, around target, zoom supported
		CAMERA_FIRST_PERSON,            // Camera first person
		CAMERA_THIRD_PERSON             // Camera third person
	};


	public ref class Camera abstract : public Engine::EngineObjects::ScriptBehaviour
	{
	protected:
		CamProjection cameraProjection;

	public:
		[Engine::Scripting::PropertyAttribute(Engine::Scripting::AccessLevel::Public)]
		CamMode cameraMode;

		[Engine::Scripting::PropertyAttribute(Engine::Scripting::AccessLevel::Public)]
		float nearPlane = 1.0f;

		[Engine::Scripting::PropertyAttribute(Engine::Scripting::AccessLevel::Public)]
		float farPlane = 10.0f;

		bool isMainCamera;

	public:
		Camera(String^ name, Engine::Internal::Components::Transform^ trans, int projection) : Engine::EngineObjects::ScriptBehaviour(name, trans)
		{
			cameraProjection = (CamProjection)projection;

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

		[Engine::Attributes::ExecuteInEditModeAttribute]
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
		CamProjection getProjection() { return cameraProjection; }

	public:
		virtual void setTarget(Engine::Components::Vector3^ target) abstract;
		virtual bool is3DCamera() abstract;
		virtual void* get() abstract;
	};
}