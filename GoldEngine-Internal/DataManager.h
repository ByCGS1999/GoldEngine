#pragma once
#include "Includes.h"

#include "imgui/imgui.h"
#include "CastToNative.h"

#include <vector>
#include "Transform.h"
#include "DataPacks.h"

namespace Engine::Internal
{
	/// <summary>
	/// Binding to the raylib abstraction layer
	/// </summary>
	public ref class DataManager
	{
	public:
		// -- MASTER
		inline static void HL_BeginRendering()
		{
			BeginDrawing();
		}

		inline static void HL_EndRendering()
		{
			EndDrawing();
		}

		inline static void HL_ClearBackground(unsigned int color)
		{
			ClearBackground(GetColor(color));
		}
		// -- CAMERA
		inline static void HL_SetCameraFov(unsigned int cameraId, float fov)
		{
			Camera3D c = DataPacks::singleton().GetCamera3D(cameraId);
			c.fovy = fov;
		}

		inline static void HL_SetCameraProjection(unsigned int cameraId, int projection)
		{
			Camera3D c = DataPacks::singleton().GetCamera3D(cameraId);
			c.projection = projection;
		}

		// -- LOADING
		inline static void HL_LoadShader(unsigned int shaderId, System::String^ vertexShaderFile, System::String^ fragmentShaderFile)
		{
			const char* vsFile = CastToNative(vertexShaderFile);
			const char* fsFile = CastToNative(fragmentShaderFile);

			Shader s = LoadShader(vsFile, fsFile);

			DataPacks::singleton().AddShader(shaderId, s);
		}

		inline static void HL_LoadModel(unsigned int modelId, System::String^ fileName)
		{
			Model model = LoadModel(CastToNative(fileName));
			Engine::Assets::Storage::DataPacks::singleton().AddModel(modelId, model);
		}

		inline static void HL_LoadTexture2D(unsigned int textureId, System::String^ fileName)
		{
			Texture2D texture = LoadTexture(CastToNative(fileName));
		}

		// -- UNLOADING / FREEING
		inline static void HL_FreeCameras()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeCameras();
		}

		inline static void HL_FreeModels()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeModels();
		}

		inline static void HL_FreeShaders()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeShaders();
		}

		inline static void HL_FreeAll()
		{
			Engine::Assets::Storage::DataPacks::singleton().FreeAll();
		}

		// -- CONFIGURATION
		inline static void HL_SetMaterialShader(unsigned int materialId, unsigned int shaderId)
		{
			Material* material = &Engine::Assets::Storage::DataPacks::singleton().GetMaterial(materialId);
			Shader* shader = &Engine::Assets::Storage::DataPacks::singleton().GetShader(shaderId);

			material->shader = *shader;
		}
		inline static void HL_SetMaterialTexture(unsigned int materialId, unsigned int textureId)
		{
			Material m = Engine::Assets::Storage::DataPacks::singleton().GetMaterial(materialId);
			Texture2D t = Engine::Assets::Storage::DataPacks::singleton().GetTexture2D(textureId);
			MaterialMap mMap;

			mMap.texture = t;
			m.maps = &mMap;
		}
		// -- CREATION
		inline static void HL_CreateMaterial(unsigned int materialId)
		{
			Material newMaterial;

			DataPacks::singleton().AddMaterial(materialId, newMaterial);
		}

		inline static void HL_CreateLight()
		{

		}

		inline static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ initialforward, Engine::Internal::Components::CameraType cameraType)
		{
			Vector3 rlib_position;
			Vector3 rlib_forward;
			Camera3D camera;

			rlib_position.x = initialPosition->x;
			rlib_position.y = initialPosition->y;
			rlib_position.z = initialPosition->z;
			rlib_forward.x = initialforward->x;
			rlib_forward.y = initialforward->y;
			rlib_forward.z = initialforward->z;

			camera.position = rlib_position;
			camera.target = rlib_forward;

			DataPacks::singleton().AddCamera(cameraId, camera, cameraType);
		}

		inline static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ forward, Engine::Internal::Components::Vector3^ up, Engine::Internal::Components::CameraType cameraType)
		{
			Vector3 rlib_position;
			Vector3 rlib_forward;
			Vector3 rlib_up;
			Camera3D camera;

			rlib_position.x = initialPosition->x;
			rlib_position.y = initialPosition->y;
			rlib_position.z = initialPosition->z;
			rlib_forward.x = forward->x;
			rlib_forward.y = forward->y;
			rlib_forward.z = forward->z;
			rlib_up.x = up->x;
			rlib_up.y = up->y;
			rlib_up.z = up->z;

			camera.position = rlib_position;
			camera.target = rlib_forward;
			camera.up = rlib_up;
			camera.projection = CAMERA_PERSPECTIVE;

			DataPacks::singleton().AddCamera(cameraId, camera, cameraType);
		}

		inline static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector2^ position, Engine::Internal::Components::CameraType cameraType)
		{
			Vector2 rlib_position;
			Camera2D camera;
			rlib_position.x = position->x;
			rlib_position.y = position->y;
			camera.target = rlib_position;

			DataPacks::singleton().AddCamera(cameraId, camera, cameraType);
		}
		inline static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::CameraType cameraType)
		{
			Vector3 rlib_position;
			Camera3D camera;

			rlib_position.x = initialPosition->x;
			rlib_position.y = initialPosition->y;
			rlib_position.z = initialPosition->z;

			camera.position = rlib_position;

			DataPacks::singleton().AddCamera(cameraId, camera, cameraType);
		}
		// -- RENDERING / DRAWING
		inline static void HL_DrawRectangle(int posX, int posY, int width, int height, unsigned int hexCode)
		{
			DrawRectangle(posX, posY, width, height, GetColor(hexCode));
		}

		inline static void HL_DrawGrid(int slices, float spaces)
		{
			DrawGrid(slices, spaces);
		}

		inline static void HL_DrawCube(Engine::Internal::Components::Vector3^ position, Engine::Internal::Components::Vector3^ scale, unsigned int color) {
			::Vector3 rlib_v3;
			rlib_v3.x = position->x;
			rlib_v3.y = position->y;
			rlib_v3.z = position->z;

			DrawCube(rlib_v3, scale->x, scale->y, scale->z, GetColor(color));
		}

		inline static void HL_DrawModel(unsigned int modelId, Engine::Internal::Components::Vector3^ position, float scale, unsigned int tint)
		{
			Model m = DataPacks::singleton().GetModel(modelId);
			::Vector3 convertedVector;


			convertedVector.x = position->x;
			convertedVector.y = position->y;
			convertedVector.z = position->z;

			DrawModel(m, convertedVector, scale, GetColor(tint));
		}

		inline static void HL_DrawEllipse(int centerX, int centerY, float radiusH, float radiusY, unsigned int hexCode)
		{
			DrawEllipse(centerX, centerY, radiusH, radiusY, GetColor(hexCode));
		}

		inline static void HL_DrawTexture(unsigned int textureId, int posX, int posY, unsigned int tint)
		{
			Texture2D t = DataPacks::singleton().GetTexture2D(textureId);
			Texture2D nullTex;

			DrawTexture(t, posX, posY, GetColor(tint));
		}

		// -- MODES
		inline static void HL_BeginShaderMode(unsigned int shaderId)
		{
			Shader s = Engine::Assets::Storage::DataPacks::singleton().GetShader(shaderId);

			BeginShaderMode(s);
		}

		inline static void HL_EndShaderMode()
		{
			EndShaderMode();
		}

		inline static void HL_Begin3DMode(unsigned int cameraId)
		{
			Camera3D c = (Camera3D)Engine::Assets::Storage::DataPacks::singleton().GetCamera3D(cameraId);

			BeginMode3D(c);
		}

		inline static void HL_Begin2DMode(unsigned int cameraId)
		{
			Camera2D c = (Camera2D)Engine::Assets::Storage::DataPacks::singleton().GetCamera2D(cameraId);
			BeginMode2D(c);
		}

		inline static void HL_EndMode(int mode)
		{
			switch (mode)
			{
			case 0:
				EndMode3D();
				break;
			case 1:
				EndMode2D();
				break;
			}
		}

		// -- YIELDING
		inline static void HL_Wait(double seconds)
		{
			WaitTime(seconds);
		}

		// -- UPDATE
		inline static void HL_UpdateCamera(unsigned int cameraId, Engine::Internal::Components::CameraType camType, int mode)
		{
			switch (camType)
			{
			case Engine::Internal::Components::CameraType::C2D:
				Camera2D cam2d = Engine::Assets::Storage::DataPacks::singleton().GetCamera2D(cameraId);
				UpdateCamera((Camera*)&cam2d, (CameraMode)mode);
				break;
			case Engine::Internal::Components::CameraType::C3D:
				Camera3D cam3d = Engine::Assets::Storage::DataPacks::singleton().GetCamera3D(cameraId);
				UpdateCamera(&cam3d, (CameraMode)mode);
				break;
			}
		}

		inline static void HL_SetModelTexture(unsigned int modelId, unsigned int materialId)
		{
			Material* m = &Engine::Assets::Storage::DataPacks::singleton().GetMaterial(materialId);
			Model* mod = &Engine::Assets::Storage::DataPacks::singleton().GetModel(modelId);

			mod->materials = m;
		}
	};
}