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

// CUSTOM RENDERERS \\

#include "VoxelRenderer.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h";
#include "CubeRenderer.h"
#include "GridRenderer.h"
#include "LightManager.h"
#include "ModelRenderer.h"
#include "PBRModelRenderer.h"
#include "Skybox.h"

// last class to be imported, it must access to all the components included \\

#include "Script.h"

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
bool showCursor = true;
bool b1, b2, b3, b4, b5, b6, b7, b8;
bool readonlyLock = false;
bool fpsCap = true;

char fileName[] = "Level0";

ref class EntryPoint : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
	Engine::Internal::Components::Vector3^ cameraPosition;
	Engine::Internal::Components::Object^ selectedObject;

	void ExecAsIdentifiedObject(Engine::Internal::Components::ObjectType type, System::Object^ object)
	{
		Engine::Internal::Components::Object^ modelRenderer = Cast::Dynamic<Engine::Internal::Components::Object^>(object);
		modelRenderer->Draw();
		modelRenderer->DrawGizmo();

		/*
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
			Engine::Internal::Components::Object^ modelRenderer = Cast::Dynamic<Engine::Internal::Components::Object^>(object);
			modelRenderer->Draw();
			modelRenderer->DrawGizmo();
		}
		*/
	}

public:
	EntryPoint()
	{
		dataPack = DataPacks();
		//packedData = gcnew DataPack();
		Start();
	}


	void Start()
	{
		//WinAPI::FreeCons();
		SetWindowFlags(4096 | 4 | FLAG_MSAA_4X_HINT);
		OpenWindow(1280, 720, (const char*)"GoldEngine Editor editor-ver0.5c");

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
					b1 = true;
				}
				if (ImGui::MenuItem("Open", "", false, true))
				{
					b3 = true;
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
				ImGui::SeparatorText("AssetPacks");
				if (ImGui::MenuItem("AssetPack Editor"))
				{
					b4 = true;
				}
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
				ImGui::Checkbox("FPS", &fpsCap);
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("Hierarchy", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{
			for each (auto obj in scene->GetRenderQueue())
			{
				Engine::Management::MiddleLevel::SceneObject^ object = (Engine::Management::MiddleLevel::SceneObject^)obj;
				auto objectType = object->objectType;
				auto reference = object->GetReference();

				if (objectType == Engine::Internal::Components::Datamodel)
				{
					if (reference != nullptr)
					{
						if (ImGui::Selectable(CastToNative(reference->name + " (READONLY)")))
						{
							readonlyLock = true;
							selectedObject = reference;
						}

						for each (auto child_obj in scene->GetRenderQueue())
						{
							Engine::Management::MiddleLevel::SceneObject^ tmp = (Engine::Management::MiddleLevel::SceneObject^)child_obj;
							auto _objectType = tmp->objectType;
							auto _reference = tmp->GetReference();

							if (_reference->GetTransform()->parent != nullptr) 
							{
								if (_reference->GetTransform()->parent->name == reference->transform->name)
								{
									if (ImGui::Selectable(CastToNative("\t" + _reference->name)))
									{
										readonlyLock = false;
										selectedObject = _reference;
									}
								}
							}
						}
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Properties", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{
			if (selectedObject == nullptr)
			{
				ImGui::Text("Select an object to edit it's properties.");
			}
			else
			{
				ImGui::SeparatorText("Object Properties");
				char* objectName = new char[128];

				strcpy(objectName, CastToNative(selectedObject->name));

				if (ImGui::InputText("Name", objectName, 128, ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->name = gcnew String(objectName);
				}

				ImGui::SeparatorText("Transform");

				// position
				float pos[3] = {
					selectedObject->GetTransform()->position->x,
					selectedObject->GetTransform()->position->y,
					selectedObject->GetTransform()->position->z
				};

				if (ImGui::DragFloat3("Position", pos, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->GetTransform()->position = gcnew Engine::Internal::Components::Vector3(pos[0], pos[1], pos[2]);
				}

				// rotation
				float rot[3] = {
					selectedObject->GetTransform()->rotation->x,
					selectedObject->GetTransform()->rotation->y,
					selectedObject->GetTransform()->rotation->z
				};

				if (ImGui::DragFloat3("Rotation", rot, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->GetTransform()->rotation = gcnew Engine::Internal::Components::Vector3(rot[0], rot[1], rot[2]);
				}

				float rot_angle = selectedObject->GetTransform()->rotationValue;

				if (ImGui::DragFloat("Rotation Value", &rot_angle, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->GetTransform()->rotationValue = rot_angle;
				}

				// scale

				float scale[3] = {
					selectedObject->GetTransform()->scale->x,
					selectedObject->GetTransform()->scale->y,
					selectedObject->GetTransform()->scale->z
				};

				if (ImGui::DragFloat3("Scale", scale, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->GetTransform()->scale = gcnew Engine::Internal::Components::Vector3(scale[0], scale[1], scale[2]);
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Assets", &isOpen, ImGuiWindowFlags_DockNodeHost | ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Assets"))
				{
					if (ImGui::MenuItem("Pack Setup"))
					{
						b5 = true;
					}
					if (ImGui::MenuItem("Scene Loader Setup"))
					{
						b6 = true;
					}
				}

				ImGui::EndMenu();
			}

			ImGui::End();
		}

		if (styleEditor)
		{
			ImGui::ShowStyleEditor(&ImGui::GetStyle());
		}

		if (ImGui::Begin("Game Viewport", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{
			bool showing = showCursor;

			if (IsKeyPressed(KEY_ESCAPE))
			{
				showCursor = !showCursor;
			}

			if (!showCursor)
			{
				if (showCursor != showing)
				{
					EnableCursor();
				}
			}
			else
			{
				DisableCursor();
			}

			rlImGuiImageRenderTextureCustom(&viewportTexture, new int[2] { (int)ImGui::GetWindowSize().x, (int)ImGui::GetWindowSize().y }, new float[2] {17.5f, 35.0f});

			ImGui::End();
		}

		if (ImGui::Begin("Toolbox", &isOpen, ImGuiWindowFlags_AlwaysVerticalScrollbar))
		{
			ImGui::SeparatorText("Engine Internal Objects");

			if (ImGui::BeginListBox(""))
			{
				if (ImGui::Button("Model Renderer"))
				{
					auto meshRenderer = gcnew Engine::EngineObjects::ModelRenderer(
						"ModelRenderer",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Internal::Components::Vector3(1, 1, 1),
							nullptr
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
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Internal::Components::Vector3(1, 1, 1),
							nullptr
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

				if (ImGui::Button("Cube Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::CubeRenderer(
						"Cube",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Internal::Components::Vector3(1, 1, 1),
							nullptr
						),
						0xFFFFFFFF
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

				if (ImGui::Button("Grid Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::GridRenderer(
						"Grid",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Internal::Components::Vector3(1, 1, 1),
							nullptr
						),
						8,
						1.0f
					);
					skyBox->SetParent(scene->GetDatamodelMember("editor only"));
					scene->AddObjectToScene(skyBox);
					scene->GetRenderQueue()->Add(
						gcnew Engine::Management::MiddleLevel::SceneObject(
							skyBox->type,
							skyBox,
							""
						)
					);
				}

				ImGui::EndListBox();
			}

			ImGui::End();
		}

		if (ImGui::Begin("Console", &isOpen, ImGuiWindowFlags_DockNodeHost))
		{

			ImGui::End();
		}

		if (b1)
		{
			ImGui::OpenPopup("New Scene");
		}
		else if (b2)
		{
			ImGui::OpenPopup("Create New Scene");
		}
		else if (b3)
		{
			ImGui::OpenPopup("Open Scene");
		}
		else if (b4)
		{
			ImGui::OpenPopup("AssetPack Editor");
		}
		else if (b6)
		{
			ImGui::OpenPopup("Scene Loader Editor");
		}

		// popups

		if (ImGui::BeginPopupModal("Scene Loader Editor", (bool*)false, ImGuiWindowFlags_NoScrollbar))
		{
			char* data = new char;

			ImGui::InputTextMultiline("", data, sizeof(data+256), {ImGui::GetWindowSize().x - 20, ImGui::GetWindowSize().y - 60});

			if (ImGui::Button("Close"))
			{
				b6 = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("New Scene", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Do you want to create a new scene?\nAll the unsaved changes will be discarded.");
			ImGui::Separator();
			if (ImGui::Button("New"))
			{
				b1 = false;
				b2 = true;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				b1 = false;
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Create New Scene", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Input Scene Name: ");
			ImGui::SameLine();
			ImGui::InputText("", fileName, 1024);

			if (ImGui::Button("Create Scene"))
			{
				scene = SceneManager::CreateScene(gcnew System::String(fileName));
				ImGui::CloseCurrentPopup();
				b2 = false;
			}
			if (ImGui::Button("Cancel"))
			{
				b2 = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Open Scene", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Input Scene Name: ");
			ImGui::SameLine();
			ImGui::InputText("", fileName, 1024);

			if (ImGui::Button("Open Scene"))
			{
				SceneManager::UnloadScene(scene);
				scene = SceneManager::LoadSceneFromFile(gcnew System::String(fileName), scene);
				ImGui::CloseCurrentPopup();
				b3 = false;
			}
			if (ImGui::Button("Cancel"))
			{
				b3 = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("AssetPack Editor", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize))
		{
			char* data = new char[512];
			for (int x = 0; x < scene->assetPacks->Count; x++)
			{
				if (data != "")
					free(data);

				data = new char[512];
				strcpy(data, CastToNative(scene->assetPacks[x]));
				std::string name = "###" + std::string(CastToNative(scene->assetPacks[x]));
				if (ImGui::InputText(name.c_str(), data, 512, ImGuiInputTextFlags_EnterReturnsTrue))
				{
					scene->assetPacks[x] = gcnew String(data);
				}
			}

			ImGui::Separator();
			{
				if (ImGui::Button("+"))
				{
					scene->assetPacks->Add("");
				}
				ImGui::SameLine();
				if (ImGui::Button("-"))
				{
					if (scene->assetPacks->Count > 1)
					{
						scene->assetPacks->RemoveAt(scene->assetPacks->Count);
					}
				}
			}
			ImGui::Separator();

			if (ImGui::Button("Close"))
			{
				b4 = false;
				ImGui::CloseCurrentPopup();
			}

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

			for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
			{
				ExecAsIdentifiedObject(obj->objectType, (System::Object^)obj->GetReference());
			}
			//DrawModel(mod, { 0.0f, 0.0f, 0.0f }, 1, WHITE);

			EndMode3D();

			DrawFPS(0, 0);

			EndTextureMode();

			rlImGuiBegin();

			ImGui::Begin("DemoVer", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
			{
				ImGui::SetWindowSize(ImVec2(285, 20), 0);
				ImGui::SetWindowPos(ImVec2(0, GetScreenHeight() - 25), 0);
				ImGui::TextColored(ImVec4(255, 255, 255, 255), "Gold Engine Ver: editor-0.5c");
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

		if (scene->sceneObjects->Count <= 0)
		{
			SceneManager::SaveSceneToFile(scene, 1234);
		}

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
		//Directory::Delete("Data/tmp/", true);
		//SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);
		Engine::Drawing::Drawing::HL_CreateCamera(0, cameraPosition, gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);

		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;

		scene->GetDatamodelMember("workspace");
		scene->GetDatamodelMember("editor only");
		scene->GetDatamodelMember("user interface");

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
		/*
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
		*/

		Init();
	}

	void Update() override
	{
		if (showCursor)
			UpdateCamera(&c3d2, CAMERA_FREE);

		if (fpsCap)
		{
			SetFPS(60);
		}
		else
		{
			SetFPS(-1);
		}


		Shader lightShader = dataPack.GetShader(1);
		float cameraPos[3] = { c3d2.position.x, c3d2.position.y, c3d2.position.z };
		SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
		rPBR::PBRLightUpdate(lightShader, lights[0]);

		for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->sceneObjects)
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