#include "Macros.h"
#include "WinAPI.h"
#include "Window.h"
#include "Includes.h"
#include "Drawing.h"
#include "Cast.h"
#include "Transform.h"
#include "CypherLib.h"
#include "SceneObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Raylib/include/rlImGui.h"
#include "DataPacks.h"
#include "DataPack.h"
#include "Raylib/include/rPBR.h"
#include <msclr/gcroot.h>
#include "FileManager.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h";
#include "LightManager.h"
#include "ModelRenderer.h"
#include "PBRModelRenderer.h"
#include "Skybox.h"

using namespace Engine::Drawing;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Internal;
using namespace Engine::EngineObjects::Native;

DataPacks dataPack;
Model mod;
Camera3D c3d2;
unsigned int passwd = 0;
unsigned int ambient_color = 0x2B2B2BFF;
float cameraSpeed = 1.25f;
bool controlCamera = true;
rPBR::PBRLight lights[1] = { 0 };
RenderTexture viewportTexture;
bool isOpen = true;
bool toggleControl = true;
bool initSettings = false;
bool styleEditor = false;

ref class EntryPoint : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
	Engine::Internal::Components::Vector3^ cameraPosition;

	void ExecAsIdentifiedObject(Engine::Internal::Components::ObjectType type, System::Object^ object)
	{
		if (type == Engine::Internal::Components::ObjectType::Skybox)
		{
			Engine::EngineObjects::Skybox^ skybox = Cast::Dynamic<Engine::EngineObjects::Skybox^>(object);
			skybox->Draw();
			skybox->DrawGizmo();
		}
		else if (type == Engine::Internal::Components::ObjectType::ModelRenderer)
		{
			Engine::EngineObjects::ModelRenderer^ modelRenderer = Cast::Dynamic<Engine::EngineObjects::ModelRenderer^>(object);
			modelRenderer->Draw();
			modelRenderer->DrawGizmo();
		}
		else
		{

		}
	}

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
		OpenWindow(1280, 720, (const char*)"GoldEngine Editor editor-ver0.5c");

		SetFPS(60);
		Preload();

		Loop();
	}

	void DrawImGui() override
	{
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

		if (!initSettings)
		{
			ImGui::LoadIniSettingsFromDisk("imgui.ini");
			WaitTime(2.5);
			initSettings = true;
		}

		auto viewPort = ImGui::GetMainViewport();
		ImGui::DockSpaceOverViewport(viewPort, ImGuiDockNodeFlags_None);

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Scene", true))
			{
				if (ImGui::MenuItem("New", "", false, true))
				{
					ImGui::OpenPopup("New Scene");
				}
				if (ImGui::MenuItem("Open", "", false, true))
				{

				}
				if (ImGui::MenuItem("Save"))
				{
					SceneManager::SaveSceneToFile(scene, passwd);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					Exit();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit", true))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help", true))
			{

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Editor", true))
			{
				if (ImGui::MenuItem("Style Editor"))
				{
					styleEditor = !styleEditor;
				}
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("Hierarchy", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{
			for each (auto object in scene->sceneObjects)
			{
				auto objectType = object->objectType;
				auto reference = object->reference;

				if (objectType == Engine::Internal::Components::Datamodel)
				{
					if (reference != nullptr)
					{
						ImGui::Selectable(CastToNative(reference->name + " (READONLY)"));
					}
				}
				else
				{
					if (reference != nullptr)
					{
						if (reference->parent != nullptr)
						{
							ImGui::Selectable(CastToNative("\t" + reference->name));
						}
						else
						{
							ImGui::Selectable(CastToNative(reference->name));
						}
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Properties", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{

			ImGui::End();
		}

		if (ImGui::Begin("Assets", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{

			ImGui::End();
		}

		if (styleEditor)
		{
			ImGui::ShowStyleEditor(&ImGui::GetStyle());
		}

		if (ImGui::Begin("Game Viewport", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{
			ImGui::Spacing();
			ImGui::SameLine();

			controlCamera = ImGui::IsWindowFocused();

			rlImGuiImageRenderTextureFit(&viewportTexture, true);

			ImGui::NewLine();

			ImGui::End();
		}

		if (ImGui::Begin("Toolbox", &isOpen, ImGuiWindowFlags_NoScrollbar))
		{
			if (ImGui::Button("Model Renderer"))
			{
				auto meshRenderer = gcnew Engine::EngineObjects::ModelRenderer(
					"ModelRenderer",
					gcnew Engine::Internal::Components::Transform(
						gcnew Engine::Internal::Components::Vector3(0, 0, 0),
						gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
						1.0f
					),
					0,
					0,
					0,
					0xFFFFFFFF
				);
				meshRenderer->SetParent(scene->GetDatamodelMember("workspace"));
				scene->AddObjectToScene(meshRenderer);
				scene->GetRenderQueue()->Add(
					gcnew Engine::Management::MiddleLevel::SceneObject(
						meshRenderer->type,
						meshRenderer,
						""
					)
				);
			}
			if (ImGui::Button("Skybox"))
			{
				auto skyBox = gcnew Engine::EngineObjects::Skybox(
					"Skybox",
					gcnew Engine::Internal::Components::Transform(
						gcnew Engine::Internal::Components::Vector3(0, 0, 0),
						gcnew Engine::Internal::Components::Quaternion(0, 0, 0, 0),
						1.0f
					),
					2,
					0,
					0
				);
				skyBox->SetParent(scene->GetDatamodelMember("workspace"));
				scene->AddObjectToScene(skyBox);
				scene->GetRenderQueue()->Add(
					gcnew Engine::Management::MiddleLevel::SceneObject(
						skyBox->type,
						skyBox,
						""
					)
				);
			}
			ImGui::End();
		}

		if (ImGui::Begin("Console", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{

			ImGui::End();
		}

		// popups

		if (ImGui::BeginPopupModal("#New_Scene", &isOpen))
		{
			ImGui::Text("Do you want to create a new scene?");
			ImGui::Separator();
			if (ImGui::Button("New"))
			{

			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();

			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("#OpenScene", &isOpen))
		{

			ImGui::EndPopup();
		}
	}

	void Exit() override
	{
		UnloadRenderTexture(viewportTexture);
		dataPack.FreeAll();
		exit(0);
	}

	void Draw() override
	{
		BeginDrawing();
		{
			BeginTextureMode(viewportTexture);

			ClearBackground(BLACK);

			BeginMode3D(c3d2);

			for each (Engine::Management::MiddleLevel::SceneObject^ obj in scene->GetRenderQueue())
			{
				ExecAsIdentifiedObject(obj->objectType, (System::Object^)obj->reference);
			}
			//DrawModel(mod, { 0.0f, 0.0f, 0.0f }, 1, WHITE);
			DrawGrid(10, 1.0f);

			EndMode3D();

			DrawFPS(0, 0);

			EndTextureMode();

			rlImGuiBegin();
			
			ImGui::Begin("DemoVer", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
			{
				ImGui::SetWindowSize(ImVec2(285, 20), 0);
				ImGui::SetWindowPos(ImVec2(0, GetScreenHeight() - 25), 0);
				ImGui::TextColored(ImVec4(0, 0, 0, 255), "Gold Engine Ver: editor-0.5c");
				ImGui::End();
			}

			DrawImGui();

			rlImGuiEnd();
		}
		EndDrawing();
	}

	void Init() override
	{
		scene = SceneManager::LoadSceneFromFile("Level0", scene);
		/*
		auto workSpace = gcnew Components::Object(
			"Workspace",
			gcnew Components::Transform(
				gcnew Components::Vector3(0,0,0),
				gcnew Components::Quaternion(0, 0, 0, 0),
				1.0f
			),
			Components::Datamodel,
			nullptr
		);
		
		auto skyBox = gcnew Engine::EngineObjects::Skybox(
			"Skybox",
			gcnew Components::Transform(
				gcnew Components::Vector3(0, 0, 0),
				gcnew Components::Quaternion(0, 0, 0, 0),
				5.0f
			),
			0
		);

		scene->AddObjectToScene(workSpace);
		scene->AddObjectToScene(skyBox);
		*/

		/*
		auto midLevSkybox = (Engine::Management::MiddleLevel::SceneObject^)scene->GetRenderQueue()[1];
		auto skyBox = (Engine::EngineObjects::Skybox^)midLevSkybox->reference;
		auto parent = (Engine::Management::MiddleLevel::SceneObject^)scene->GetRenderQueue()[0];
		
		skyBox->Init(skyBox->materialId, skyBox->texPath);
		skyBox->SetupSkyboxImage(0);
		*/
		int numLights = 1;
		SetShaderValue(dataPack.GetShader(1), GetShaderLocation(dataPack.GetShader(1), "numOfLights"), &numLights, SHADER_UNIFORM_INT);
		rPBR::PBRSetAmbient(dataPack.GetShader(1), { 0, 0, 0, 255 }, 0.0f);
		lights[0] = rPBR::PBRLightCreate(rPBR::LIGHT_POINT, { -5,5,0 }, { .5f,-.5f,0 }, { 255,255,255, 255 }, 2.5f, dataPack.GetShader(1));

		//FileManager::ReadCustomFileFormat("Data/assets1.gold", passwd);
		 
		cameraPosition = gcnew Engine::Internal::Components::Vector3(0, 0, 0);
		Directory::Delete("Data/tmp/", true);
		//SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);
		Engine::Drawing::Drawing::HL_CreateCamera(0, cameraPosition, gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);

		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;
		
	}

	void Preload() override
	{
		if (FirstTimeBoot())
		{
			WinAPI::MBOX(GetWindowHandle(), "LPVOID* voidFunc = (LPVOID*)nativeData;\nvoidFunc->Test();", "GoldEngine - Ver 0.5c - editor", 0x00000040L | 0x00000000L);
			Boot();
		}
		SetExitKey(KEY_NULL);
		viewportTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		/*
		auto files = gcnew List<String^>();
		files->Add("Models/castle.obj");
		files->Add("Models/castle_diffuse.png");
		files->Add("Engine/Models/Error.obj");
		files->Add("Engine/Models/Skybox_cube.glb");
		files->Add("Engine/Textures/Error.png");
		files->Add("Engine/Skybox/Daylight.png");

		FileManager::WriteToCustomFile("Data/engineassets.gold", "ThreadcallNull", files->ToArray());*/
		//FileManager::WriteCustomFileFormat("Data/assets1.gold", "ThereGoesThePasswordGyat", passwd);

		scene = SceneManager::CreateScene("GoldBootManager");
		FileManager::ReadCustomFileFormat("Data/engineassets.gold", "ThreadcallNull");

		Model model;
		MaterialMap matMap;

		packedData->ReadFromFile("Assets0", passwd);

		Texture2D t = dataPack.GetTexture2D(1);

		matMap.texture = dataPack.GetTexture2D(1);

		Material m = dataPack.GetMaterial(0);
		m.maps = &matMap;

		Shader s = dataPack.GetShader(0);
		Shader lightShader = dataPack.GetShader(1);

		lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightShader, "viewPos");

		t = dataPack.GetTexture2D(0);

		model = dataPack.GetModel(0);
		m = { dataPack.GetShader(0), 0,0,0,0, {0} };

		dataPack.SetShader(1, lightShader);

		Init();
	}

	void Update() override
	{
		if (controlCamera && toggleControl)
			UpdateCamera(&c3d2, CAMERA_FREE);

		if (IsKeyPressed(KEY_F3))
			toggleControl = !toggleControl;

		Shader lightShader = dataPack.GetShader(1);
		float cameraPos[3] = { c3d2.position.x, c3d2.position.y, c3d2.position.z };
		SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
		rPBR::PBRLightUpdate(lightShader, lights[0]);

		for each (Engine::Management::MiddleLevel::SceneObject^ obj in scene->sceneObjects)
		{

		}

		if (IsKeyDown(KEY_F1))
		{
			ambient_color++;
		}
		if (IsKeyDown(KEY_F2))
		{
			ambient_color--;
		}

		if (IsKeyPressed(KEY_F4))
		{
			SceneManager::UnloadScene(scene);
			scene = SceneManager::CreateScene("Level0");
			scene->sceneName = "Level0";
		}

		if (IsKeyPressed(KEY_F5))
		{
			scene = SceneManager::LoadSceneFromFile("Level0", scene);
			if (scene != nullptr)
			{
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