#include "Window.h"

#include "Includes.h"
#include "Drawing.h"
#include "Transform.h"
#include "Raylib/include/rlImGui.h"

using namespace Engine::Drawing;

DataPacks dataPack;
Texture2D tx2;
Camera3D c3d;
Camera3D c3d2;
ref class EntryPoint : Engine::Window
{
public:
	EntryPoint()
	{
		dataPack = DataPacks();
		Start();

	}


	void Start()
	{
		SetWindowFlags(4096 | 4);
		OpenWindow(640, 480, (const char*)"GoldEngine Window");
		SetFPS(60);
		Preload();


		Loop();
	}

	void Exit() override
	{
		dataPack.FreeAll();
	}

	void Draw() override
	{
		BeginDrawing();

			ClearBackground(RAYWHITE);
		
			BeginMode3D(c3d2);

				DrawCube({ 0.0f,0.0f,0.0F }, 1, 1, 1, BLACK);
				DrawGrid(10, 1.0f);

			EndMode3D();

			rlImGuiBegin();

				ImGui::ShowDemoWindow((bool*)true);

			rlImGuiEnd();

		EndDrawing();
	}

	void Preload() override
	{
		Drawing::HL_LoadModel(0, "Data/Models/castle.obj");
		dataPack.AddShader(0, LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs"));
		Drawing::HL_CreateCamera(0, gcnew Engine::Internal::Components::Vector3(10, 5, 5), gcnew Engine::Internal::Components::Vector3(0,0,1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);
		//dataPack.AddCamera(0, c3d, Engine::Internal::Components::C3D);
		dataPack.AddTexture2D(0,LoadTexture("Data/Models/castle_diffuse.png"));
		Drawing::HL_CreateMaterial(0);
		Drawing::HL_SetMaterialShader(0, 0);
		Drawing::HL_SetMaterialTexture(0, 0);
		Drawing::HL_SetModelTexture(0, 0);
		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;
	}

	void Update() override
	{
		UpdateCamera(&c3d2, CAMERA_ORBITAL);
	}
};


int main()
{
	gcnew EntryPoint();
}