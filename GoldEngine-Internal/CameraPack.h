#pragma once

namespace Engine::Internal::Components
{
	public enum CameraType
	{
		C2D,
		C3D
	};
}

namespace Engine::Assets::Storage::Types
{
	public class CameraPack
	{
	public:
		unsigned int cameraId;
		::Camera2D camera2D;
		::Camera3D camera3D;
		Engine::Internal::Components::CameraType cameraType;

	public:
		CameraPack(unsigned int cameraId, Engine::Internal::Components::CameraType cameraType, ::Camera3D* cam3DRef = nullptr)
		{
			this->cameraId = cameraId;
			this->camera3D = *cam3DRef;
			this->cameraType = cameraType;
		}

	public:
		CameraPack(unsigned int cameraId, Engine::Internal::Components::CameraType cameraType, Camera2D* cam2DRef = nullptr)
		{
			this->cameraId = cameraId;
			this->camera2D = *cam2DRef;
			this->cameraType = cameraType;
		}
	};
}