#include "Macros.h"

#include "Window.h"

#include "Includes.h"
#include "Drawing.h"
#include "Transform.h"
#include "CypherLib.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Raylib/include/rlImGui.h"

// INCLUDE ENGINE CLASSES \\

#include "ModelRenderer.h"
#include "Skybox.h"

using namespace Engine::Drawing;
using namespace Engine::Management;
using namespace Engine::Managers;

DataPacks dataPack;
Model mod;
Camera3D c3d;
Camera3D c3d2;
unsigned int passwd;
unsigned int ambient_color = 0xA2A2A2A2;

ref class EntryPoint : Engine::Window
{
	Scene^ scene;
	Engine::EngineObjects::Skybox^ skybox;
	Engine::EngineObjects::ModelRenderer^ modelRenderer;

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
		{
			ClearBackground(RAYWHITE);

			BeginMode3D(c3d2);

			for each (Object ^ obj in scene->sceneObjects)
			{
				// try cast
				auto newObj = reinterpret_cast<Engine::Internal::Components::Object^>(obj);

				if (newObj != nullptr)
				{
					newObj->Draw();
				}
			}
			//DrawModel(mod, { 0.0f, 0.0f, 0.0f }, 1, WHITE);
			DrawGrid(10, 1.0f);

			EndMode3D();

			rlImGuiBegin();

			ImGui::ShowDemoWindow((bool*)true);

			rlImGuiEnd();
		}
		EndDrawing();
	}

	void Preload() override
	{
		if (FirstTimeBoot())
		{
			Boot();
		}

		scene = SceneManager::CreateScene();
		scene->sceneName = "Level0";

		skybox = gcnew Engine::EngineObjects::Skybox(
			"Skybox",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				5.0f
			)
		);

		modelRenderer = gcnew Engine::EngineObjects::ModelRenderer(
			"Model1",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				0.1f
			)
		);

		Texture2D t = LoadTexture("Data/Models/castle_diffuse.png");
		Model model = LoadModel("Data/Models/castle.obj");
		Shader s = LoadShader("Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");
		Material m = { s, 0,0,0,0, {0} };
		model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = t;
		SetShaderValueTexture(s, GetShaderLocation(s, "texture0"), t);

		modelRenderer->SetNativeRenderer(new Engine::EngineObjects::Native::NativeModelRenderer(model, m, ambient_color));

		scene->AddObjectToScene(skybox);
		scene->AddObjectToScene(modelRenderer);

		SceneManager::SaveSceneToFile(scene, passwd);
		Drawing::HL_CreateCamera(0, gcnew Engine::Internal::Components::Vector3(10, 5, 5), gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);
		//dataPack.AddCamera(0, c3d, Engine::Internal::Components::C3D);
		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;
	}

	void Update() override
	{
		UpdateCamera(&c3d2, CAMERA_ORBITAL);

		if (IsKeyDown(KEY_F1))
		{
			ambient_color++;
			modelRenderer->SetColorTint(ambient_color);
		}
		if (IsKeyDown(KEY_F2)) 
		{
			ambient_color--;
			modelRenderer->SetColorTint(ambient_color);
		}
	}
};


int main()
{
	passwd = CypherLib::GetPasswordBytes(ENCRYPTION_PASSWORD);
	gcnew EntryPoint();
}