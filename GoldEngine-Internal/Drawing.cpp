#include "Drawing.h"
#include "Raylib/include/rPBR.h"

rPBR::Environment pbrEnv;

void Engine::Drawing::Drawing::HL_BeginRendering()
{
	BeginDrawing();
}

void Engine::Drawing::Drawing::HL_EndRendering()
{
	EndDrawing();
}

void Engine::Drawing::Drawing::HL_ClearBackground(unsigned int color)
{
	ClearBackground(GetColor(color));
}

void Engine::Drawing::Drawing::HL_CreateLight()
{

}

void Engine::Drawing::Drawing::HL_CreateMaterial(unsigned int materialId)
{
	Material newMaterial;

	DataPacks::AddMaterial(materialId, newMaterial);
}

void Engine::Drawing::Drawing::HL_DrawRectangle(int posX, int posY, int width, int height, unsigned int hexCode)
{
	DrawRectangle(posX, posY, width, height, GetColor(hexCode));
}

void Engine::Drawing::Drawing::HL_LoadShader(unsigned int shaderId, System::String^ vertexShaderFile, System::String^ fragmentShaderFile)
{
	const char* vsFile = CastToNative(vertexShaderFile);
	const char* fsFile = CastToNative(fragmentShaderFile);

	Shader s = LoadShader(vsFile, fsFile);

	DataPacks::AddShader(shaderId, s);
}

void Engine::Drawing::Drawing::HL_DrawGrid(int slices, float spaces)
{
	DrawGrid(slices, spaces);
}

void Engine::Drawing::Drawing::HL_CreateEnvironment(String^ fileName, int cubemapSize, int irradianceSize, int prefilterSize, int brdfSize)
{
	const char* fN = CastToNative(fileName);

	pbrEnv = rPBR::LoadEnvironment(fN, cubemapSize, irradianceSize, prefilterSize, brdfSize);
}

void Engine::Drawing::Drawing::HL_UnloadEnvironment()
{
	rPBR::UnloadEnvironment(pbrEnv);
	pbrEnv = { 0 };
}

void Engine::Drawing::Drawing::HL_DrawSkybox(unsigned int cameraId, Engine::Internal::Components::CameraType cameraType)
{
	Camera3D camera3d = DataPacks::GetCamera3D(cameraId);
	rPBR::DrawSkybox(pbrEnv, camera3d);
}

void Engine::Drawing::Drawing::HL_SetMaxLights(int ammount)
{
#undef MAX_LIGHTS
#define MAX_LIGHTS ammount
}

void Engine::Drawing::Drawing::HL_DrawCube(Engine::Internal::Components::Vector3^ position, Engine::Internal::Components::Vector3^ scale, unsigned int color)
{
	Vector3 rlib_v3;
	rlib_v3.x = position->x;
	rlib_v3.y = position->y;
	rlib_v3.z = position->z;

	DrawCube(rlib_v3, scale->x, scale->y, scale->z, GetColor(color));
}

void Engine::Drawing::Drawing::HL_LoadModel(unsigned int modelId, System::String^ fileName)
{
	Model model = LoadModel(CastToNative(fileName));
	DataPacks::AddModel(modelId, model);
}

void Engine::Drawing::Drawing::HL_DrawModel(unsigned int modelId, Engine::Internal::Components::Vector3^ position, float scale, unsigned int tint)
{
	Model m = DataPacks::GetModel(modelId);
	Vector3 convertedVector;


	convertedVector.x = position->x;
	convertedVector.y = position->y;
	convertedVector.z = position->z;

	DrawModel(m, convertedVector, scale, GetColor(tint));
}

void Engine::Drawing::Drawing::HL_SetMaterialShader(unsigned int materialId, unsigned int shaderId)
{
	Material material = DataPacks::GetMaterial(materialId);
	Shader shader = DataPacks::GetShader(shaderId);

	material.shader = shader;
}

void Engine::Drawing::Drawing::HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ initialEuler, Engine::Internal::Components::CameraType cameraType)
{
	Vector3 rlib_position;
	Vector3 rlib_euler;
	Camera3D camera;

	rlib_position.x = initialPosition->x;
	rlib_position.y = initialPosition->y;
	rlib_position.z = initialPosition->z;
	rlib_euler.x = initialEuler->x;
	rlib_euler.y = initialEuler->y;
	rlib_euler.z = initialEuler->z;

	camera.position = rlib_position;
	camera.target = rlib_euler;

	DataPacks::AddCamera(cameraId, camera, cameraType);
}

void Engine::Drawing::Drawing::HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::Vector3^ initialEuler, Engine::Internal::Components::Vector3^ up, Engine::Internal::Components::CameraType cameraType)
{
	Vector3 rlib_position;
	Vector3 rlib_euler;
	Vector3 rlib_up;
	Camera3D camera;

	rlib_position.x = initialPosition->x;
	rlib_position.y = initialPosition->y;
	rlib_position.z = initialPosition->z;
	rlib_euler.x = initialEuler->x;
	rlib_euler.y = initialEuler->y;
	rlib_euler.z = initialEuler->z;
	rlib_up.x = up->x;
	rlib_up.y = up->y;
	rlib_up.z = up->z;

	camera.position = rlib_position;
	camera.target = rlib_euler;
	camera.up = rlib_up;
	camera.projection = CAMERA_PERSPECTIVE;

	DataPacks::AddCamera(cameraId, camera, cameraType);
}

void Engine::Drawing::Drawing::HL_EndMode(int mode)
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

void Engine::Drawing::Drawing::HL_UpdateCamera(unsigned int cameraId, Engine::Internal::Components::CameraType camType, int mode)
{
	switch (camType)
	{
	case Engine::Internal::Components::CameraType::C2D:
		Camera2D cam2d = DataPacks::GetCamera2D(cameraId);
		UpdateCamera((Camera*)&cam2d, (CameraMode)mode);
		break;
	case Engine::Internal::Components::CameraType::C3D:
		Camera3D cam3d = DataPacks::GetCamera3D(cameraId);
		UpdateCamera(&cam3d, (CameraMode)mode);
		break;
	}
}

void Engine::Drawing::Drawing::HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector3^ initialPosition, Engine::Internal::Components::CameraType cameraType)
{
	Vector3 rlib_position;
	Camera3D camera;

	rlib_position.x = initialPosition->x;
	rlib_position.y = initialPosition->y;
	rlib_position.z = initialPosition->z;

	camera.position = rlib_position;

	DataPacks::AddCamera(cameraId, camera, cameraType);
}

void Engine::Drawing::Drawing::HL_BeginShaderMode(unsigned int shaderId)
{
	Shader s = DataPacks::GetShader(shaderId);

	BeginShaderMode(s);
}

void Engine::Drawing::Drawing::HL_EndShaderMode()
{
	EndShaderMode();
}

void Engine::Drawing::Drawing::HL_FreeCameras()
{
	DataPacks::FreeCameras();
}

void Engine::Drawing::Drawing::HL_FreeModels()
{
	DataPacks::FreeModels();
}

void Engine::Drawing::Drawing::HL_FreeShaders()
{
	DataPacks::FreeShaders();
}

void Engine::Drawing::Drawing::HL_FreeAll()
{
	DataPacks::FreeAll();
}

void Engine::Drawing::Drawing::HL_LoadTexture2D(unsigned int textureId, System::String^ fileName)
{
	Texture2D texture = LoadTexture(CastToNative(fileName));
}

void Engine::Drawing::Drawing::HL_DrawEllipse(int centerX, int centerY, float radiusH, float radiusY, unsigned int hexCode)
{
	DrawEllipse(centerX, centerY, radiusH, radiusY, GetColor(hexCode));
}

void Engine::Drawing::Drawing::HL_CreateCamera(unsigned int cameraId, Engine::Internal::Components::Vector2^ position, Engine::Internal::Components::CameraType cameraType)
{
	Vector2 rlib_position;
	Camera2D camera;
	rlib_position.x = position->x;
	rlib_position.y = position->y;
	camera.target = rlib_position;

	DataPacks::AddCamera(cameraId, camera, cameraType);
}

void Engine::Drawing::Drawing::HL_Begin3DMode(unsigned int cameraId)
{
	Camera3D c = (Camera3D)DataPacks::GetCamera3D(cameraId);

	BeginMode3D(c);
}

void Engine::Drawing::Drawing::HL_Begin2DMode(unsigned int cameraId)
{
	Camera2D c = (Camera2D)DataPacks::GetCamera2D(cameraId);
	BeginMode2D(c);
}

void Engine::Drawing::Drawing::HL_Wait(double seconds)
{
	WaitTime(seconds);
}

void Engine::Drawing::Drawing::HL_SetMaterialTexture(unsigned int materialId, unsigned int textureId)
{
	Material m = DataPacks::GetMaterial(materialId);
	Texture2D t = DataPacks::GetTexture2D(textureId);
	MaterialMap mMap;

	mMap.texture = t;
	m.maps = &mMap;
}

void Engine::Drawing::Drawing::HL_SetModelTexture(unsigned int modelId, unsigned int materialId)
{
	Material m = DataPacks::GetMaterial(materialId);
	Model mod = DataPacks::GetModel(modelId);

	mod.materials = &m;
}

void Engine::Drawing::Drawing::HL_SetCameraFov(unsigned int cameraId, float fov)
{
	Camera3D c = DataPacks::GetCamera3D(cameraId);
	c.fovy = fov;
}

void Engine::Drawing::Drawing::HL_SetCameraProjection(unsigned int cameraId, int projection)
{
	Camera3D c = DataPacks::GetCamera3D(cameraId);
	c.projection = projection;
}

void Engine::Drawing::Drawing::HL_DrawTexture(unsigned int textureId, int posX, int posY, unsigned int tint)
{
	Texture2D t = DataPacks::GetTexture2D(textureId);
	Texture2D nullTex;

	DrawTexture(t, posX, posY, GetColor(tint));
}