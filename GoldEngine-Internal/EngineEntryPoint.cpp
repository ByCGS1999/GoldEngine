#include "Macros.h"

#include "Window.h"

#include "Includes.h"
#include "Drawing.h"
#include "Transform.h"
#include "CypherLib.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Raylib/include/rlImGui.h"
#include "DataPacks.h"
#include "DataPack.h"
#include "WinAPI.h"

// INCLUDE ENGINE CLASSES \\

#include "ModelRenderer.h"
#include "Skybox.h"

using namespace Engine::Drawing;
using namespace Engine::Management;
using namespace Engine::Managers;

DataPacks dataPack;
Model mod;
Camera3D c3d2;
unsigned int passwd;
unsigned int ambient_color = 0xFFFFFFFF;
float cameraSpeed = 1.25f;
bool controlCamera = true;

ref class EntryPoint : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::Skybox^ skybox;
	Engine::EngineObjects::ModelRenderer^ modelRenderer;
	Engine::Internal::Components::Vector3^ cameraPosition;

public:
	EntryPoint()
	{
		dataPack = DataPacks();
		packedData = gcnew DataPack();
		Start();
	}


	void Start()
	{
		WinAPI::FreeCons();
		SetWindowFlags(4096 | 4);
		OpenWindow(640, 480, (const char*)"GoldEngine Technical Demo");

		SetFPS(60);
		Preload();

		Loop();
	}

	void PrebuildAssets()
	{

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

			ImGui::Begin("DemoVer", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
			{
				ImGui::SetWindowSize(ImVec2(285, 20), 0);
				ImGui::SetWindowPos(ImVec2(0, GetScreenHeight()-25), 0);
				ImGui::TextColored(ImVec4(0, 0, 0, 255), "Gold Engine Ver: dev-0.0.2-early");
			}
			ImGui::End();

			rlImGuiEnd();
		}
		EndDrawing();
	}

	void Init() override
	{
		scene = SceneManager::CreateScene();
		scene->sceneName = "Level0";

		skybox = gcnew Engine::EngineObjects::Skybox(
			"Skybox",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 2, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				25.0f
			),
			1
		);

		modelRenderer = gcnew Engine::EngineObjects::ModelRenderer(
			"Model1",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				0.1f
			),
			0,
			0,
			ambient_color
		);

		skybox->SetupSkyboxImage(0, LoadTexture("Data/Engine/Skybox/Daylight.png"));

		//modelRenderer->SetNativeRenderer(new Engine::EngineObjects::Native::NativeModelRenderer(model, m, ambient_color));

		scene->AddObjectToScene(skybox);
		scene->AddObjectToScene(modelRenderer);

		cameraPosition = gcnew Engine::Internal::Components::Vector3(0, 0, 0);

		SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);
		Drawing::HL_CreateCamera(0, cameraPosition, gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);
		//dataPack.AddCamera(0, c3d, Engine::Internal::Components::C3D);
		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;
	}

	void Preload() override
	{
		if (FirstTimeBoot())
		{
			WinAPI::MBOX(GetWindowHandle(), "Nigger", "GoldEngine - Ver 0.0.2 - early", 0x00000040L | 0x00000000L);
			Boot();
		}

		Model model;
		MaterialMap matMap;

		packedData->ReadFromFile("Assets1", passwd);

		Texture2D t = dataPack.GetTexture2D(1);

		matMap.texture = dataPack.GetTexture2D(1);

		Material m = dataPack.GetMaterial(0);
		m.maps = &matMap;

		Shader s = dataPack.GetShader(0);

		t = dataPack.GetTexture2D(0);
		SetShaderValueTexture(s, GetShaderLocation(s, "texture0"), dataPack.GetTexture2D(0));

		model = dataPack.GetModel(0);
		m = { dataPack.GetShader(0), 0,0,0,0, {0}};

		model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = dataPack.GetTexture2D(0);

		Init();
	}

	void Update() override
	{
		if(controlCamera)
			UpdateCamera(&c3d2, CAMERA_FREE);

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
		if (IsKeyPressed(KEY_F3))
		{
			controlCamera = !controlCamera;
		}

		if (IsKeyPressed(KEY_F4))
		{
			scene = SceneManager::CreateScene();
			scene->sceneName = "Level0";
		}

		if (IsKeyPressed(KEY_F5))
		{
			Scene^ scn = SceneManager::LoadSceneFromFile("Level0");
			if (scn != nullptr)
			{
				scene = scn;
				TraceLog(LOG_INFO, CastToNative(scene->sceneName));
			}
			else
			{
				TraceLog(LOG_FATAL, "[CORE ERROR]: Failed loading scene");
			}
		}

		if (IsKeyPressed(KEY_F6))
		{
			SceneManager::SaveSceneToFile(scene, passwd);
		}
	}
};


int main()
{
	passwd = CypherLib::GetPasswordBytes(ENCRYPTION_PASSWORD);
	gcnew EntryPoint();
}