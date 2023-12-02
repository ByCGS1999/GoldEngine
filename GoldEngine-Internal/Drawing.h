#pragma once
#include "Includes.h"

#include "imgui/imgui.h"
#include "CastToNative.h"

#include <vector>
#include "Object.h"
#include "Transform.h"
#include "DataPacks.h"

namespace Engine::Drawing
{
	/// <summary>
	/// Binding to the raylib abstraction layer
	/// </summary>
	public ref class Drawing
	{
	public:
		// -- MASTER
		static void HL_BeginRendering();
		static void HL_EndRendering();
		static void HL_ClearBackground(unsigned int color);
		// -- CAMERA
		static void HL_SetCameraFov(unsigned int cameraId, float fov);
		static void HL_SetCameraProjection(unsigned int cameraId, int projection);
		// -- LOADING
		static void HL_LoadShader(unsigned int shaderId, System::String^ vertexShaderFile, System::String^ fragmentShaderFile);
		static void HL_LoadModel(unsigned int modelId, System::String^ fileName);
		static void HL_LoadTexture2D(unsigned int textureId, System::String^ fileName);
		// -- UNLOADING / FREEING
		static void HL_UnloadEnvironment();
		static void HL_FreeCameras();
		static void HL_FreeModels();
		static void HL_FreeShaders();
		static void HL_FreeAll();
		// -- CONFIGURATION
		static void HL_SetMaxLights(int ammount);
		static void HL_SetMaterialShader(unsigned int materialId, unsigned int shaderId);
		static void HL_SetMaterialTexture(unsigned int materialId, unsigned int textureId);
		// -- CREATION
		static void HL_CreateEnvironment(String^ fileName, int cubemapSize, int irradianceSize, int prefilterSize, int brdfSize);
		static void HL_CreateMaterial(unsigned int materialId);
		static void HL_CreateLight();
		static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ initialEuler, Engine::Internal::Components::CameraType cameraType);
		static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ initialEuler, Engine::Internal::Components::Vector3^ up, Engine::Internal::Components::CameraType cameraType);
		static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector2^ position, Engine::Internal::Components::CameraType cameraType);
		static void HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::CameraType cameraType);
		// -- RENDERING / DRAWING
		static void HL_DrawRectangle(int posX, int posY, int width, int height, unsigned int hexCode);
		static void HL_DrawGrid(int slices, float spaces);
		static void HL_DrawSkybox(unsigned int cameraId, Engine::Internal::Components::CameraType cameraType);
		static void HL_DrawCube(Engine::Internal::Components::Vector3^ position, Engine::Internal::Components::Vector3^ scale, unsigned int color);
		static void HL_DrawModel(unsigned int modelId, Engine::Internal::Components::Vector3^ position, float scale, unsigned int tint);
		static void HL_DrawEllipse(int centerX, int centerY, float radiusH, float radiusY, unsigned int hexCode);
		static void HL_DrawTexture(unsigned int textureId, int posX, int posY, unsigned int tint);
		// -- MODES
		static void HL_BeginShaderMode(unsigned int shaderId);
		static void HL_EndShaderMode();
		static void HL_Begin3DMode(unsigned int cameraId);
		static void HL_Begin2DMode(unsigned int cameraId);
		static void HL_EndMode(int mode);
		// -- YIELDING
		static void HL_Wait(double seconds);
		// -- UPDATE
		static void HL_UpdateCamera(unsigned int cameraId, Engine::Internal::Components::CameraType camType, int mode);
		static void HL_SetModelTexture(unsigned int modelId, unsigned int materialId);
	};
}