#include "Macros.h"
#include "WinAPI.h"
#include "Window.h"
#include "Includes.h"
#include "GlIncludes.h"
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
#include <msclr/gcroot.h>
#include "FileManager.h"

// CUSTOM RENDERERS \\

#include "VoxelRenderer.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h"
#include "BoundingBoxRenderer.h"
#include "CubeRenderer.h"
#include "GridRenderer.h"
#include "LightManager.h"
#include "ModelRenderer.h"
#include "PBRModelRenderer.h"
#include "Skybox.h"

// last class to be imported, it must access to all the components included \\

#include "Script.h"

// last but not least, the assembly loader.

#include "InputManager.h"
#include "ObjectManager.h"
#include "AsmLoader.h"

// Daemons (Daemons are tasks that are ran mandatory by the engine, these cannot be displayed by the hierarchy)

#include "Daemon.h"
#include "LightDm.h"

// Preload queue & scripts, can be used for loading/unloading certain data or doing operations with shaders, materials, models whatever.

#include "PreloadScript.h"


using namespace Engine;
using namespace Engine::EngineObjects;
using namespace Engine::EngineObjects::Native;
using namespace Engine::Internal;
using namespace Engine::Internal::Components;
using namespace Engine::Management;
using namespace Engine::Management::MiddleLevel;
using namespace Engine::Managers;
using namespace Engine::Scripting;

unsigned int passwd = 0;

#if PRODUCTION_BUILD == false

#pragma region EDITOR ENGINE

DataPacks dataPack;
Model mod;
Camera3D c3d2;
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
bool b1, b2, b3, b4, b5, b6, b7, b8, b9;
bool readonlyLock = false;
bool fpsCap = true;
bool reparentLock = false;

char fileName[] = "Level0";

ref class EditorWindow : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
	Engine::Internal::Components::Vector3^ cameraPosition;
	Engine::Internal::Components::Object^ selectedObject;
	Engine::Internal::Components::Object^ reparentObject;
	System::Collections::Generic::List<EngineAssembly^>^ assemblies;
	Scripting::ObjectManager^ objectManager;

	void DrawHierarchyInherits(Engine::Management::Scene^ scene, Engine::Internal::Components::Object^ reference, int depth)
	{
		for each (SceneObject ^ _obj in scene->GetRenderQueue())
		{
			auto _reference = _obj->GetReference();
			auto _type = _obj->objectType;

			if (_reference != reference)
			{
				if (_reference->transform->parent != nullptr)
				{
					if (_reference->transform->parent->uid == reference->GetTransform()->uid)
					{
						String^ refName = "";
						for (int x = 0; x < depth; x++)
						{
							refName += "\t";
						}

						refName += _reference->name;

						if (_type == ObjectType::Daemon || _type == ObjectType::Datamodel || _type == ObjectType::LightManager)
						{
							if (ImGui::Selectable(CastToNative(refName + " (ENGINE PROTECTED)")))
							{
								if (reparentLock)
									reparentObject = _reference;
								else
								{
									readonlyLock = true;
									selectedObject = _reference;
								}
							}
						}
						else
						{
							if (ImGui::Selectable(CastToNative(refName)))
							{
								if (reparentLock)
									reparentObject = _reference;
								else
								{
									readonlyLock = false;
									selectedObject = _reference;
								}
							}
						}
						DrawHierarchyInherits(scene, _reference, depth + 1);
					}
				}
			}
		}
	}

	void ExecAsIdentifiedObject(Engine::Internal::Components::ObjectType type, System::Object^ object)
	{
		Engine::Internal::Components::Object^ modelRenderer = (Engine::Internal::Components::Object^)object;
		modelRenderer->Draw();
		modelRenderer->DrawGizmo();
	}

public:
	EditorWindow()
	{
		assemblies = gcnew System::Collections::Generic::List<EngineAssembly^>();
		dataPack = DataPacks();
		//packedData = gcnew DataPack();
		auto newAssembly = gcnew EngineAssembly();
		newAssembly->LoadAssemblyFromFile("Data/Asm/GoldEngine_ScriptAssembly.dll");

		assemblies->Add(newAssembly);

		for each (EngineAssembly ^ assembly in assemblies)
		{
			assembly->ListAssemblyTypes();
		}

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

			if (ImGui::BeginMenu("Scripts", true))
			{
				ImGui::SeparatorText("Scene");
				if (ImGui::MenuItem("Scene Assemblies"))
				{

				}
				if (ImGui::MenuItem("Preload Assemblies"))
				{

				}
				ImGui::SeparatorText("Shaders");
				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (ImGui::Begin("Hierarchy", &isOpen))
		{
			ImGui::Text("Scene Objects: %d", scene->GetRenderQueue()->Count);
			ImGui::Separator();

			for each (SceneObject ^ obj in scene->GetRenderQueue())
			{
				auto reference = obj->GetReference();
				auto type = obj->objectType;

				if (reference != nullptr)
				{
					if (type == ObjectType::Datamodel || type == ObjectType::LightManager)
					{
						if (ImGui::Selectable(CastToNative(reference->name + " (ENGINE PROTECTED)")))
						{
							if (reparentLock)
								reparentObject = reference;
							else
							{
								readonlyLock = true;
								selectedObject = reference;
							}
						}
					}
					else if (reference->GetTransform()->parent == nullptr)
					{
						if (ImGui::Selectable(CastToNative(reference->name + " (UNPARENTED)")))
						{
							if (reparentLock)
								reparentObject = reference;
							else
							{
								readonlyLock = false;
								selectedObject = reference;
							}
						}
					}
				}

				DrawHierarchyInherits(scene, reference, 1);
			}


			ImGui::End();
		}

		if (ImGui::Begin("Properties", &isOpen))
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

				ImGui::SeparatorText("Parent");
				if (selectedObject->GetTransform()->parent != nullptr)
				{
					ImGui::Text(CastToNative(selectedObject->GetTransform()->GetParent()->name));
				}
				else
				{
					ImGui::Text(CastToNative("None"));
				}
				if (ImGui::Button("Change Parent"))
				{
					if (!readonlyLock)
						reparentLock = true;
				}

				ImGui::SeparatorText("Object Info");
				ImGui::Text(CastStringToNative(selectedObject->type.ToString()).c_str());
			}

			ImGui::End();
		}

		if (ImGui::Begin("Assets", &isOpen, ImGuiWindowFlags_MenuBar))
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

		if (ImGui::Begin("Game Viewport", &isOpen))
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

			if (ImGui::BeginListBox("###INTERNALSCRIPTS"))
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

				if (ImGui::Button("Point Light"))
				{
					auto meshRenderer = gcnew Engine::EngineObjects::LightSource(
						"Point Light",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							gcnew Engine::Internal::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Internal::Components::Vector3(1, 1, 1),
							nullptr
						),
						0xFF0000FF,
						rPBR::PBRLightType::LIGHT_POINT,
						gcnew Engine::Internal::Components::Vector3(1.0f, 1.0f, 1.0f),
						1.0f,
						1
					);
					meshRenderer->SetParent(lightManager);
					lightManager->AddLight(meshRenderer, 1);
					scene->AddObjectToScene(meshRenderer);
					scene->PushToRenderQueue(meshRenderer);
				}

				if (ImGui::Button("PBR Model Renderer"))
				{
					auto meshRenderer = gcnew Engine::EngineObjects::PBRModelRenderer(
						"PBRModelRenderer",
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

				if (ImGui::Button("BoundingBox Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::BoundingBoxRenderer(
						"BoundingBox",
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

			ImGui::SeparatorText("UserScripts");

			if (ImGui::BeginListBox("###USERSCRIPTS"))
			{
				for each (auto assembly in assemblies)
				{
					for each (auto T in assembly->GetAssemblyTypes())
					{
						if (ImGui::Button(CastToNative(T->Name)))
						{
							WinAPI::MBOX((void*)GetWindowHandle(), "NOT IMPLEMENTED YET", "Gold Editor", 0x00000040L | 0x00000000L);
						}
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Console", &isOpen))
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

		if (reparentLock)
		{
			if (reparentObject != nullptr)
			{
				selectedObject->SetParent(reparentObject);
				reparentLock = false;
			}
		}
		else
		{
			reparentObject = nullptr;
		}

		// popups

		if (ImGui::BeginPopupModal("Scene Loader Editor", (bool*)false, ImGuiWindowFlags_NoScrollbar))
		{
			int heapAlloc = 512;
			char* data = new char[8192];
			auto t = System::IO::File::ReadAllText("Data/" + scene->sceneRequirements + ".asset");

			int size = t->ToCharArray()->Length + heapAlloc;
			data = new char[size];

			strcpy(data, CastToNative(t));

			if (ImGui::InputTextMultiline("", data, size, { ImGui::GetWindowSize().x - 20, ImGui::GetWindowSize().y - 60 }))
			{
				System::String^ str = gcnew System::String(data);

				System::IO::File::WriteAllText("Data/" + scene->sceneRequirements + ".asset", str);
			}

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

			ClearBackground(GetColor(scene->skyColor));

			BeginMode3D(c3d2);

			for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
			{
				ExecAsIdentifiedObject(obj->objectType, (System::Object^)obj->GetReference());
			}

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
		SceneManager::SetAssemblyManager(assemblies);

		scene = SceneManager::LoadSceneFromFile("Level0", scene);

		scene->Preload();

		Shader lightShader = dataPack.GetShader(1);

		lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightShader, "viewPos");

		dataPack.SetShader(1, lightShader);

		scene->GetDatamodelMember("workspace");

		if (!scene->ExistsDatamodelMember("lighting"))
		{
			lightManager = gcnew LightManager("lighting",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					0.0f,
					gcnew Engine::Internal::Components::Vector3(0, 0, 0),
					nullptr
				)
			);

			scene->PushToRenderQueue(lightManager);
			scene->AddObjectToScene(lightManager);
		}
		else
		{
			lightManager = (LightManager^)scene->GetDatamodelMember("lighting");
		}


		scene->GetDatamodelMember("editor only");
		scene->GetDatamodelMember("gui");
		auto daemonParent = scene->GetDatamodelMember("daemons");

		auto lightdm = gcnew Engine::EngineObjects::Daemons::LightDaemon("lightdm",
			gcnew Engine::Internal::Components::Transform(
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				0.0f,
				gcnew Engine::Internal::Components::Vector3(0, 0, 0),
				nullptr
			),
			lightManager
		);
		lightdm->SetParent(daemonParent);
		scene->PushToRenderQueue(lightdm);
		scene->AddObjectToScene(lightdm);

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
		//int numLights = 1;
		//SetShaderValue(dataPack.GetShader(1), GetShaderLocation(dataPack.GetShader(1), "numOfLights"), &numLights, SHADER_UNIFORM_INT);
		rPBR::PBRSetAmbient(dataPack.GetShader(1), { 0, 0, 0, 255 }, 0.0f);
		//lights[0] = rPBR::PBRLightCreate(rPBR::LIGHT_POINT, { -5,5,0 }, { .5f,-.5f,0 }, { 255,255,255, 255 }, 2.5f, dataPack.GetShader(1));

		//FileManager::ReadCustomFileFormat("Data/assets1.gold", passwd);

		cameraPosition = gcnew Engine::Internal::Components::Vector3(0, 0, 0);
		//Directory::Delete("Data/tmp/", true);
		//SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);
		Engine::Drawing::Drawing::HL_CreateCamera(0, cameraPosition, gcnew Engine::Internal::Components::Vector3(0, 0, 1), gcnew Engine::Internal::Components::Vector3(0, 1, 0), Engine::Internal::Components::C3D);

		c3d2 = dataPack.GetCamera3D(0);
		c3d2.projection = CAMERA_PERSPECTIVE;
		c3d2.fovy = 60;

		objectManager = gcnew Scripting::ObjectManager(scene);

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

		auto light = lightManager->getLight(0);
		if (light != nullptr)
		{
			rPBR::PBRLightUpdate(lightShader, light->GetLight());
		}
		//lightManager->Update();

		for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->sceneObjects)
		{
			if (obj->GetReference() != nullptr)
			{
				obj->GetReference()->Update();
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
	}
};



#pragma endregion

#else

#pragma region PLAYBACK ENGINE

public ref class GameWindow : public Engine::Window
{
	Scene^ scene;

public:
	GameWindow()
	{
		OpenWindow(1280, 720, "GameWindow");
	}

	virtual void Init() override
	{

	}

	virtual void Preload() override
	{

	}

	virtual void Draw() override
	{

	}

	virtual void Update() override
	{

	}

	virtual void Draw() override
	{

	}
};

#pragma endregion

#endif



int main()
{
	passwd = CypherLib::GetPasswordBytes(ENCRYPTION_PASSWORD);

#if PRODUCTION_BUILD
	gcnew GameWindow();
#else
	gcnew EditorWindow();
#endif
}
