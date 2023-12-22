#include "Macros.h"
#include "WinAPI.h"
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
#include "Raylib/include/rPBR.h"
#include <msclr/gcroot.h>
#include "FileManager.h"

// INCLUDE ENGINE CLASSES \\

#include "LightManager.h"
#include "ModelRenderer.h"
#include "PBRModelRenderer.h"
#include "Skybox.h"

using namespace Engine::Drawing;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::EngineObjects::Native;

DataPacks dataPack;
Model mod;
Camera3D c3d2;
unsigned int passwd = 0;
unsigned int ambient_color = 0x2B2B2BFF;
float cameraSpeed = 1.25f;
bool controlCamera = true;
rPBR::PBRLight lights[1] = { 0 };

ref class EntryPoint : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
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
		//WinAPI::FreeCons();
		SetWindowFlags(4096 | 4 | FLAG_MSAA_4X_HINT);
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
			ClearBackground(BLACK);

			BeginMode3D(c3d2);

			for each (Object ^ obj in scene->sceneObjects)
			{
				// try cast
				auto newObj = reinterpret_cast<Engine::Internal::Components::Object^>(obj);

				if (newObj != nullptr)
				{
					newObj->DrawGizmo();
					newObj->Draw();
				}
			}
			//DrawModel(mod, { 0.0f, 0.0f, 0.0f }, 1, WHITE);
			DrawGrid(10, 1.0f);

			EndMode3D();

			DrawFPS(0, 0);

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

		auto workspace = gcnew Engine::Internal::Components::Object(
			"Workspace",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				0.0f
			),
			Engine::Internal::Components::Generic
		);

		auto skybox = gcnew Engine::EngineObjects::Skybox(
			"Skybox",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 2, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				25.0f
			),
			1
		);

		auto modelRenderer = gcnew Engine::EngineObjects::PBRModelRenderer(
			"Model1",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				0.1f
			),
			0,
			ambient_color
		);

		/*
		lightManager = gcnew Engine::EngineObjects::LightManager(
			"LightManager",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				0.1f
			)
		);

		auto directionalLight = gcnew Engine::EngineObjects::LightSource(
			"DirectionalLight1",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(-2.5f, 5.0f, 0),
				gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
				1
			),
			rPBR::LIGHT_DIRECTIONAL,
			gcnew Engine::Internal::Components::Vector3(2.5f, -1, 0),
			1.0f,
			dataPack.GetShader(1)
		);
		*/

		//directionalLight->lightColor = 0xFF0000FF;

		skybox->SetupSkyboxImage(0, LoadTexture("Data/Engine/Skybox/Daylight.png"));

		//lightManager->AddLight(directionalLight, 1);
		
		int numLights = 1;
		SetShaderValue(dataPack.GetShader(1), GetShaderLocation(dataPack.GetShader(1), "numOfLights"), &numLights, SHADER_UNIFORM_INT);
		rPBR::PBRSetAmbient(dataPack.GetShader(1), {0, 0, 0, 255}, 0.0f);
		lights[0] = rPBR::PBRLightCreate(rPBR::LIGHT_POINT, { -5,5,0 }, { .5f,-.5f,0 }, {255,255,255, 255}, 2.5f, dataPack.GetShader(1));

		modelRenderer->SetupMaterial(1);
		modelRenderer->SetTexture(0, rPBR::PBR_TEXTURE_ALBEDO);
		modelRenderer->SetVector(gcnew Engine::Internal::Components::Vector2(0.5, 0.5), rPBR::PBR_VEC2_TILING);
		modelRenderer->SetColor(rPBR::PBR_COLOR_EMISSIVE, { 255,162,0,255 });
		modelRenderer->SetMaterial(0);

		modelRenderer->SetParent(workspace);

		//modelRenderer->SetNativeRenderer(new Engine::EngineObjects::Native::NativeModelRenderer(model, m, ambient_color));

		//modelRenderer->GetNativeRenderer()->model.get()->materials[0].shader = S;

		scene->AddObjectToScene(workspace);
		scene->AddObjectToScene(skybox);
		//scene->AddObjectToScene(lightManager);
		//scene->AddObjectToScene(directionalLight);
		scene->AddObjectToScene(modelRenderer);

		//FileManager::ReadCustomFileFormat("Data/assets1.gold", passwd);

		cameraPosition = gcnew Engine::Internal::Components::Vector3(0, 0, 0);
		//Directory::Delete("Data/tmp/", true);
		SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);
		Engine::Drawing::Drawing::HL_CreateCamera(0, cameraPosition, gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);
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

		auto files = gcnew List<String^>();
		files->Add("Models/castle.obj");
		files->Add("Engine/Models/Error.obj");
		files->Add("Engine/Models/Skybox_cube.glb");

		FileManager::WriteToCustomFile("Data/assets2.gold", "ThreadcallNull", files->ToArray());
		//FileManager::WriteCustomFileFormat("Data/assets1.gold", "ThereGoesThePasswordGyat", passwd);

		FileManager::ReadCustomFileFormat("Data/assets2.gold", "ThreadcallNull", passwd);

		Model model;
		MaterialMap matMap;

		packedData->ReadFromFile("Assets1", passwd);

		Texture2D t = dataPack.GetTexture2D(1);

		matMap.texture = dataPack.GetTexture2D(1);

		Material m = dataPack.GetMaterial(0);
		m.maps = &matMap;

		Shader s = dataPack.GetShader(0);
		Shader lightShader = dataPack.GetShader(1);

		lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightShader, "viewPos");

		t = dataPack.GetTexture2D(0);

		model = dataPack.GetModel(0);
		m = { dataPack.GetShader(0), 0,0,0,0, {0}};

		dataPack.SetShader(1, lightShader);

		Init();
	}

	void Update() override
	{
		if(controlCamera)
			UpdateCamera(&c3d2, CAMERA_FREE);

		Shader lightShader = dataPack.GetShader(1);
		float cameraPos[3] = { c3d2.position.x, c3d2.position.y, c3d2.position.z };
		SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
		rPBR::PBRLightUpdate(lightShader, lights[0]);

		for each (Object ^ obj in scene->sceneObjects)
		{
			// try cast
			auto newObj = reinterpret_cast<Engine::Internal::Components::Object^>(obj);

			if (newObj != nullptr)
			{
				newObj->Update();
			}
		}

		if (IsKeyDown(KEY_F1))
		{
			ambient_color++;
		}
		if (IsKeyDown(KEY_F2)) 
		{
			ambient_color--;
		}
		if (IsKeyPressed(KEY_F3))
		{
			controlCamera = !controlCamera;
		}

		if (IsKeyPressed(KEY_F4))
		{
			SceneManager::UnloadScene(scene);
			scene = SceneManager::CreateScene();
			scene->sceneName = "Level0";
		}

		if (IsKeyPressed(KEY_F5))
		{
			Scene^ scn = SceneManager::LoadSceneFromFile("Level0");
			if (scn != nullptr)
			{
				SceneManager::UnloadScene(scene);
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