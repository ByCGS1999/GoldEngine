#include "Macros.h"

const int max_lights = 4;
unsigned int passwd = 0;

#include "WinAPI.h"
#include "Window.h"
#include "Includes.h"
#include "GlIncludes.h"
#include "LoggingAPI.h"
#include "DataManager.h"
#include "Cast.h"
#include "EngineConfig.h"
#include "Object/Transform.h"
#include "CypherLib.h"
#include "SceneObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Raylib/include/rlImGui.h"
#include "DataPacks.h"
#include "DataPack.h"
#include "FileManager.h"
#include "imgui/FileExplorer/filedialog.h"

// CUSTOM RENDERERS \\

#include "VoxelRenderer.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h"
#include "EngineIncludes.h"

// last but not least, the assembly loader.

#include "ImguiHook.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "ShaderManager.h"
#include "LogFileReporter.h"
#include "AsmLoader.h"

// Daemons (Daemons are tasks that are ran mandatory by the engine, these cannot be displayed by the hierarchy)

#include "Objects/LightDm.h"

// Preload queue & scripts, can be used for loading/unloading certain data or doing operations with shaders, materials, models whatever.

#include "PreloadScript.h"

#include "native/json.hpp"
#include "imguistyleserializer.h"

// lua virtual machine
#include "LuaVM.h"
// lua script object
#include "Objects/LuaScript.h"


using namespace Engine;
using namespace Engine::EngineObjects;
using namespace Engine::EngineObjects::Native;
using namespace Engine::Internal;
using namespace Engine::Internal::Components;
using namespace Engine::Management;
using namespace Engine::Management::MiddleLevel;
using namespace Engine::Managers;
using namespace Engine::Scripting;

DataPacks dataPack;

#if PRODUCTION_BUILD == false

#pragma region EDITOR ENGINE

Model mod;
unsigned int ambient_color = 0x2B2B2BFF;
float cameraSpeed = 1.25f;
bool controlCamera = true;
RenderTexture viewportTexture;
bool isOpen = true;
bool toggleControl = true;
bool initSettings = false;
bool styleEditor = false;
bool showCursor = true;
bool b1, b2, b3, b4, b5, b6, b7, b8, b9;
bool readonlyLock = false;
bool fpsCap = true;
bool fpsCheck = true;
bool reparentLock = false;
TextEditor* codeEditor = new TextEditor();
ImVec2 codeEditorSize;
auto language = TextEditor::LanguageDefinition::CPlusPlus();
bool codeEditorOpen = false;
VoxelRenderer* renderer;
std::string styleFN;
Texture modelTexture;
Texture materialTexture;
std::string codeEditorFile = "";
bool fileDialogOpen = false;
int tmp1;
char* password = new char[512];

bool ce1, ce2, ce3, ce4, ce5, ce6;

std::string codeEditorChunk;

char fileName[] = "Level0";

ref class EditorWindow : Engine::Window
{
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
	Engine::Components::Vector3^ cameraPosition;
	Engine::Internal::Components::Object^ selectedObject;
	Engine::Internal::Components::Object^ reparentObject;
	System::Collections::Generic::List<EngineAssembly^>^ assemblies;
	Scripting::ObjectManager^ objectManager;
	Engine::Lua::VM::LuaVM^ luaVM;
	Engine::Editor::Gui::fileExplorer^ fileExplorer = gcnew Engine::Editor::Gui::fileExplorer(std::string("File Explorer"));

private:
	void SaveEditorCode()
	{
		if (codeEditorFile != "")
		{
			File::WriteAllText(gcnew String(codeEditorFile.c_str()), gcnew String(codeEditor->GetText().c_str()));
		}
	}

	void createAssetEntries(String^ path)
	{
		for each (String ^ f in Directory::GetFiles(path))
		{
			f = f->Replace(R"(\)", "/");
			array<String^>^ tmp = f->Split('/');
			auto t = tmp[tmp->Length - 1] + "\n";

			if (f->Contains(".obj") || f->Contains(".glb") || f->Contains(".gltf") || f->Contains(".fbx") || f->Contains(".vox")) // model types
			{
				if (rlImGuiImageButton(CastStringToNative("###" + t).c_str(), &modelTexture))
				{
					unsigned int assetId = 0;
					auto res = packedData->hasAsset(Engine::Assets::Management::assetType::_Model, f);
					if (!std::get<0>(res))
					{
						assetId = packedData->GetAssetID(Engine::Assets::Management::assetType::_Model);

						packedData->AddModel(assetId, CastStringToNative(f).c_str());

						packedData->WriteToFile(packedData->getFile(), passwd);
					}
					else
					{
						assetId = std::get<1>(res);
					}

					auto meshRenderer = gcnew Engine::EngineObjects::ModelRenderer(
						"ModelRenderer",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						assetId,
						1,
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
				ImGui::SameLine();
				ImGui::Text(CastStringToNative(t).c_str());
			}
			else if (f->Contains(".mat"))
			{
				if (rlImGuiImageButton(CastStringToNative("###" + t).c_str(), &modelTexture))
				{

				}
			}
		}

		for each (String ^ dir in Directory::GetDirectories(path))
		{
			createAssetEntries(dir);
		}
	}

private:
	void SpecializedPropertyEditor(Engine::Internal::Components::Object^ object)
	{
		if (object != nullptr)
		{
			auto type = object->type;

			switch (type)
			{
			case ObjectType::ModelRenderer: // MODEL RENDERER
			{
				Engine::EngineObjects::ModelRenderer^ renderer = Cast::Dynamic<Engine::EngineObjects::ModelRenderer^>(object);
				unsigned int modelId = renderer->model;
				unsigned int materialId = renderer->shader;
				unsigned int textureId = renderer->texture;
				unsigned long tint = renderer->tint;

				int mId = (int)modelId;
				int matId = (int)materialId;
				int texId = (int)textureId;

				ImGui::SeparatorText("Model Renderer");
				ImGui::Text("Model ID: ");
				ImGui::SameLine();
				if (ImGui::InputInt("###MODELID_SETTER", &mId, 1, 1))
				{
					renderer->model = (unsigned int)mId;
				}
				ImGui::Text("Shader ID: ");
				ImGui::SameLine();
				if (ImGui::InputInt("###SHADERID_SETTER", &matId, 1, 1))
				{
					renderer->shader = (unsigned int)matId;
				}
				ImGui::Text("Texture ID: ");
				ImGui::SameLine();
				if (ImGui::InputInt("###TEXTUREID_SETTER", &texId, 1, 1))
				{
					renderer->texture = (unsigned int)texId;
				}

				auto float4 = ImGui::ColorConvertU32ToFloat4(ImU32(tint));

				float rawData[4] =
				{
					float4.x,
					float4.y,
					float4.z,
					float4.w
				};

				ImGui::Text("Tint Editor: ");
				ImGui::SameLine();
				if (ImGui::ColorPicker4("###TINT_SETTER", rawData))
				{
					renderer->SetColorTint(ImGui::ColorConvertFloat4ToU32(ImVec4(rawData[0], rawData[1], rawData[2], rawData[3])));
				}

			}
			break;

			case ObjectType::LightSource:
			{
				Engine::EngineObjects::LightSource^ light = Cast::Dynamic<Engine::EngineObjects::LightSource^>(object);
				bool lightEnabled = light->enabled;
				ImGui::SeparatorText("Light Source");
				ImGui::Text("Enabled: ");
				ImGui::SameLine();
				if (ImGui::Checkbox("###LIGHT_ENABLED", &lightEnabled))
				{
					if (lightEnabled)
					{
						light->enabled = true;
					}
					else
					{
						light->enabled = false;
					}
				}
				float intensity = light->intensity;
				ImGui::Text("Intensity:");
				ImGui::SameLine();
				if (ImGui::InputFloat("###LIGHT_INTENSITY", &intensity, 0.1f, 1.0f, "%.1f"))
				{
					light->intensity = intensity;
				}
			}
			break;

			case ObjectType::Script:
			{
				Engine::EngineObjects::Script^ script = (Engine::EngineObjects::Script^)object;

				if (script->assemblyReference->Contains("LuaScript"))
				{
					auto scr = (Engine::EngineObjects::LuaScript^)script;

					ImGui::SeparatorText("Linked Source");

					ImGui::Text("Binary file path: ");
					ImGui::SameLine();

					std::string nativePath = CastStringToNative(scr->luaFilePath);

					char* data = nativePath.data();

					if (ImGui::InputText("###LUA_LINKEDSOURCE",
						data, scr->luaFilePath->Length + 8, ImGuiInputTextFlags_CallbackCompletion))
					{
						scr->luaFilePath = gcnew String(data);
					}

					if (ImGui::Button("Reload lua source"))
					{
						scr->Reset();
					}
				}

				ImGui::SeparatorText("Attributes");

				if (ImGui::BeginListBox("###ATTRIBUTE_LISTBOX"))
				{
					for each (Engine::Scripting::Attribute ^ attrib in script->attributes->attributes)
					{
						if (attrib != nullptr)
						{
							ImGui::Text(CastStringToNative(attrib->name + " (" + attrib->type + ")").c_str());
							if (attrib->userData->GetType()->Equals(String::typeid))
							{

							}
							else if (attrib->userData->GetType()->Equals(UInt32::typeid))
							{
								int value = (unsigned int)attrib->userData;

								if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
								{
									attrib->setValue(gcnew UInt32(value), true);
								}
							}
							else if (attrib->userData->GetType()->Equals(Int64::typeid))
							{
								long long tmp = (Int64)attrib->userData;

								int value = (int)tmp;

								if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
								{
									attrib->setValue(gcnew Int64(value), true);
								}
							}
							else if (attrib->getValueType()->Equals(Engine::Components::Color::typeid))
							{
								Engine::Components::Color^ value = nullptr;

								if (attrib->getValue()->GetType() == Newtonsoft::Json::Linq::JObject::typeid)
								{
									auto v = (Newtonsoft::Json::Linq::JObject^)attrib->getValue();
									value = v->ToObject<Engine::Components::Color^>();
								}
								else
								{
									value = (Engine::Components::Color^)attrib->getValue();
								}


								auto float4 = ImGui::ColorConvertU32ToFloat4(ImU32(value->toHex()));

								float rawData[4] =
								{
									float4.x,
									float4.y,
									float4.z,
									float4.w
								};

								if (ImGui::ColorEdit4(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), rawData))
								{
									attrib->setValue(gcnew Engine::Components::Color(ImGui::ColorConvertFloat4ToU32(ImVec4(rawData[0], rawData[1], rawData[2], rawData[3]))), false);
								}
							}

							ImGui::Separator();
						}
					}

					ImGui::EndListBox();
				}
			}
			break;

			}
		}
	}

private:
	void SetEditorCode(std::string fileName, std::string fileContents)
	{
		SaveEditorCode();

		std::ofstream stream(codeEditorFile.c_str());

		if (stream.good())
		{
			stream.clear();
			stream << codeEditor->GetText();
		}

		stream.close();

		codeEditorFile = fileName;

		codeEditor->SetText(fileContents.c_str());

		codeEditorChunk = fileContents.c_str();
	}

	void SetEditorCode(String^ fileName)
	{
		SaveEditorCode();

		codeEditorFile = CastStringToNative(fileName);
		String^ content = File::ReadAllText(fileName);

		if (content->Equals(""))
		{
			codeEditor->SetText("");
			codeEditorChunk = "";
		}
		else
		{
			codeEditor->SetText(CastStringToNative(content));
			codeEditorChunk = CastStringToNative(content);
		}
	}

	void SaveEditorContents(String^ path)
	{
		File::WriteAllText(path, gcnew String(codeEditorChunk.c_str()));
	}

	void OpenFileExplorer(std::string name, Engine::Editor::Gui::explorerMode mode, Action<String^>^ callback)
	{
		fileExplorer->SetWindowName(name);
		fileExplorer->setExplorerMode(mode);
		fileExplorer->Open();

		fileExplorer->OnCompleted(callback);
	}

	void CodeEditor()
	{
		if (ImGui::Begin("Embedded Code Editor", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Open"))
					{
						if (codeEditorFile != "")
						{
							String^ chunk = gcnew String(codeEditorChunk.c_str());
							String^ currentChunk = gcnew String(codeEditor->GetText().c_str());

							if (!currentChunk->Equals(chunk))
							{
								ce1 = true;
							}
							else
							{
								OpenFileExplorer("Open File", Engine::Editor::Gui::explorerMode::Open, (gcnew Action<String^>(this, &EditorWindow::SetEditorCode)));
							}

							delete chunk;
							delete currentChunk;
						}
						else
						{
							OpenFileExplorer("Open File", Engine::Editor::Gui::explorerMode::Open, (gcnew Action<String^>(this, &EditorWindow::SetEditorCode)));
						}
					}
					if (ImGui::MenuItem("Save"))
					{
						codeEditorChunk = codeEditor->GetText();

						OpenFileExplorer("Save File", Engine::Editor::Gui::explorerMode::Save, (gcnew Action<String^>(this, &EditorWindow::SaveEditorContents)));
					}
					ImGui::Separator();
					if (ImGui::BeginMenu("Lua Compiler"))
					{
						if (ImGui::MenuItem("Save Lua Bytecode"))
						{
							luaVM->LoadSource(gcnew String(codeEditor->GetText().c_str()));

							OpenFileExplorer("Save Lua Compiled File", Engine::Editor::Gui::explorerMode::Save, (gcnew Action<String^>(luaVM, &Engine::Lua::VM::LuaVM::WriteLuaCodeToFile)));
						}

						ImGui::EndMenu();
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit"))
					{
						codeEditorOpen = false;
					}
					ImGui::EndMenu();
				}


				if (ImGui::BeginMenu("Language"))
				{
					if (ImGui::MenuItem("C++"))
					{
						language = TextEditor::LanguageDefinition::CPlusPlus();
					}
					if (ImGui::MenuItem("GLSL"))
					{
						language = TextEditor::LanguageDefinition::GLSL();
					}
					if (ImGui::MenuItem("Lua"))
					{
						language = TextEditor::LanguageDefinition::Lua();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Templates"))
				{
					if (ImGui::MenuItem("C++ Script Template"))
					{
						language = TextEditor::LanguageDefinition::CPlusPlus();
						codeEditor->SetText(std::string(R"(#include <cstdio>
#include <Windows.h>
#include "Typedefs.h"

using namespace Engine::EngineObjects;
using namespace Engine::Management;
using namespace Engine::Managers;
using namespace Engine::Scripting;
using namespace Engine::Internal::Components;

namespace UserScripts
{
	public ref class NewBehaviour : public Engine::EngineObjects::Script
	{
	public:
		NewBehaviour(System::String^ name, Engine::Internal::Components::Transform^ transform) : Script(name, transform)
		{

		}

	public:
		void Update() override
		{

		}

		void Draw() override
		{

		}

		void DrawGizmo() override
		{

		}

		void DrawImGUI() override
		{

		}
	};
}
)"));
					}
					if (ImGui::MenuItem("GLSL"))
					{
						language = TextEditor::LanguageDefinition::GLSL();
					}
					if (ImGui::MenuItem("Lua Script Template"))
					{
						language = TextEditor::LanguageDefinition::Lua();
						codeEditor->SetText(std::string(R"(-- Called when the object gets instantiated
function Start()

end

-- Called each time the physics engine does a tick (1/2 of the normal framerate by default).
function PhysicsUpdate()

end

-- Called every frame (after draw).
function Update()

end

-- Called every frame (before update).
function Draw()

end

-- Called when the ImGUI execution cycle is began (allows for using the imgui hook)
function DrawImGUI()

end

-- Called every frame (Renders only on editor (not client)).
function DrawGizmos()

end
)"));
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			if (ce1)
			{
				ImGui::OpenPopup("Code Editor - Save Changes");
			}

			if (ImGui::BeginPopupModal("Code Editor - Save Changes", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("You have unsaved changes, do you want to save them?");

				if (ImGui::Button("Yes"))
				{
					ce1 = false;
					SaveEditorCode();
					codeEditorChunk = codeEditor->GetText();
					ImGui::CloseCurrentPopup();
				}
				ImGui::SameLine();
				if (ImGui::Button("No"))
				{
					ce1 = false;
					codeEditorChunk = codeEditor->GetText();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			codeEditor->SetLanguageDefinition(language);

			codeEditor->Render("Embedded Code Editor");

			ImGui::End();
		}
	}

	void DrawHierarchyInherits(Engine::Management::Scene^ scene, Engine::Internal::Components::Object^ parent, int depth)
	{
		for each (SceneObject ^ _obj in scene->GetRenderQueue())
		{
			auto _reference = _obj->GetReference();
			auto _type = _obj->objectType;

			if (parent->Equals(_reference))
				continue;

			if (_reference->GetTransform() == nullptr)
				continue;

			if (_reference->GetTransform()->parent != nullptr)
			{
				if (_reference->GetTransform()->parent->GetUID() == parent->GetTransform()->GetUID())
				{
					String^ refName = "";
					for (int x = 0; x < depth; x++)
					{
						refName += "\t";
					}

					refName += _reference->name;

					if (_type == ObjectType::Daemon || _type == ObjectType::Datamodel || _type == ObjectType::LightManager)
					{
						if (ImGui::Selectable(CastStringToNative(refName + " (ENGINE PROTECTED)" + "###" + _reference->GetTransform()->GetUID()).c_str()))
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
						if (ImGui::Selectable(CastStringToNative(refName + "###" + _reference->GetTransform()->GetUID()).c_str()))
						{
							if (reparentLock)
								reparentObject = _reference;
							else
							{
								readonlyLock = false;
								selectedObject = _reference;
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
		if (scene->sceneLoaded())
		{
			Engine::Internal::Components::Object^ modelRenderer = (Engine::Internal::Components::Object^)object;
			modelRenderer->Draw();
			modelRenderer->DrawGizmo();
		}
	}

public:
	EditorWindow()
	{
		strcpy(password, ENCRYPTION_PASSWORD);

		luaVM = gcnew Engine::Lua::VM::LuaVM();
		assemblies = gcnew System::Collections::Generic::List<EngineAssembly^>();
		dataPack = DataPacks();

		assemblies->Add(gcnew EngineAssembly("Data/Asm/GoldEngine_ScriptAssembly.dll"));
		assemblies->Add(gcnew EngineAssembly(System::Reflection::Assembly::GetExecutingAssembly()));

		SceneManager::SetAssemblyManager(assemblies);

		for each (EngineAssembly ^ assembly in assemblies)
		{
			assembly->ListAssemblyTypes();
		}

		Start();
	}


	void Start()
	{
		//WinAPI::FreeCons();
		SetWindowFlags(FLAG_INTERLACED_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
		OpenWindow(1280, 720, (const char*)EDITOR_VERSION);

		auto secrets = gcnew Engine::Config::EngineSecrets(ENCRYPTION_PASSWORD);

		secrets->ExportSecrets("./Data/Keys/secrets.dat");

		auto config = gcnew Engine::Config::EngineConfiguration("Gold Engine Window", gcnew Engine::Config::Resolution(0, 0, 1280, 720), "GoldEngine/main.log", FLAG_INTERLACED_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
		config->ExportConfig("./Data/appinfo.dat");


		if (Directory::Exists(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/'))))
		{
			if (File::Exists(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath))
			{
				File::Delete(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath);
			}

			Directory::Delete(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/')));
		}

		Directory::CreateDirectory(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/')));
		gcnew Engine::Utils::LogReporter(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath);

		LayerManager::RegisterDefaultLayers();

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
			if (ImGui::BeginMenu("Engine", true))
			{
				ImGui::SeparatorText("Encryption");

				ImGui::SeparatorText("Asymmetric");

				if (ImGui::MenuItem("Generate new KeyPair"))
				{
					Engine::Config::EngineSecrets::singleton()->ExportSecrets("./Data/Keys/secrets.dat");
				}

				ImGui::SeparatorText("Symmetric");

				if (ImGui::BeginMenu("Set encryption password"))
				{
					if (ImGui::InputText("###PASSWORD_SETTER", password, 512))
					{
						passwd = CypherLib::GetPasswordBytes(gcnew String(password));
					}

					ImGui::EndMenu();
				}

				ImGui::SeparatorText("Project");

				if (ImGui::MenuItem("Generate project"))
				{

				}

				if (ImGui::MenuItem("Build Settings"))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Exit();
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Scene", true))
			{
				if (ImGui::MenuItem("New Scene", "", false, true))
				{
					b1 = true;
				}
				if (ImGui::MenuItem("Open Scene", "", false, true))
				{
					b3 = true;
				}
				if (ImGui::MenuItem("Save Scene"))
				{
					SceneManager::SaveSceneToFile(scene, passwd);
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
				ImGui::SeparatorText("Engine");
				if (ImGui::MenuItem("Layer Editor"))
				{
					b8 = true;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Views", true))
			{
				if (ImGui::MenuItem("Editor View"))
				{

				}
				if (ImGui::MenuItem("Game View"))
				{

				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Object", true))
			{
				if (ImGui::BeginMenu("Cameras"))
				{
					if (ImGui::MenuItem("Camera3D"))
					{
						Engine::EngineObjects::Camera^ newCamera = gcnew Engine::EngineObjects::Camera3D("Camera",
							gcnew Engine::Internal::Components::Transform(
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 0,0,0 }),
								0.0f,
								Engine::Components::Vector3::create({ 1,1,1 }),
								nullptr
							));

						scene->AddObjectToScene(newCamera);
						scene->PushToRenderQueue(newCamera);
					}
					if (ImGui::MenuItem("Camera2D"))
					{

					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Scripts", true))
			{
				ImGui::SeparatorText("Scene");
				if (ImGui::MenuItem("Scene Assemblies"))
				{

				}
				if (ImGui::MenuItem("Preload Assemblies"))
				{

				}
				ImGui::SeparatorText("Tools");
				if (ImGui::MenuItem("Script Editor"))
				{
					codeEditorOpen = !codeEditorOpen;
				}
				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Editor", true))
			{
				if (ImGui::BeginMenu("Style", true))
				{
					if (ImGui::MenuItem("Style Editor"))
					{
						styleEditor = !styleEditor;
					}
					if (ImGui::MenuItem("Save/Load Style"))
					{
						b7 = true;
					}

					ImGui::EndMenu();
				}
				if (ImGui::Checkbox("FPS", &fpsCheck))
				{
					fpsCap = fpsCheck;
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help", true))
			{

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
						if (ImGui::Selectable(CastToNative(reference->name + " (ENGINE PROTECTED)" + "###" + reference->GetTransform()->GetUID())))
						{
							if (reparentLock)
								reparentObject = reference;
							else
							{
								readonlyLock = true;
								selectedObject = reference;
							}
						}

						DrawHierarchyInherits(scene, reference, 1);
					}
					else if (reference->GetTransform()->parent == nullptr)
					{
						if (ImGui::Selectable(CastToNative(reference->name + " (UNPARENTED)" + "###" + reference->GetTransform()->GetUID())))
						{
							if (reparentLock)
								reparentObject = reference;
							else
							{
								readonlyLock = false;
								selectedObject = reference;
							}
						}

						DrawHierarchyInherits(scene, reference, 1);
					}
				}
			}


			ImGui::End();
		}

		if (codeEditorOpen)
		{
			CodeEditor();
		}

		if (ImGui::Begin("Properties", &isOpen, ImGuiWindowFlags_MenuBar))
		{
			if (selectedObject == nullptr)
			{
				ImGui::Text("Select an object to edit it's properties.");
			}
			else
			{
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Instance"))
					{
						ImGui::SeparatorText("Actions");

						if (ImGui::MenuItem("Destroy Object"))
						{
							ObjectManager::singleton()->Destroy(selectedObject);
						}

						ImGui::Separator();

						if (ImGui::BeginMenu("Rendering"))
						{
							switch (selectedObject->viewSpace)
							{
							case ViewSpace::VNone:
								tmp1 = 0;
								break;
							case ViewSpace::V2D:
								tmp1 = 1;
								break;
							case ViewSpace::V3D:
								tmp1 = 2;
								break;
							}

							const char* types[]{ "None", "2D", "3D" };
							if (ImGui::Combo("###CURR_MODE", &tmp1, types, IM_ARRAYSIZE(types)))
							{
								switch (tmp1)
								{
								case 0:
									selectedObject->viewSpace = ViewSpace::VNone;
									break;
								case 1:
									selectedObject->viewSpace = ViewSpace::V2D;
									break;
								case 2:
									selectedObject->viewSpace = ViewSpace::V3D;
									break;
								}
							}
							std::string layer;

							if (selectedObject->layerMask != nullptr)
								layer = CastStringToNative(LayerManager::GetLayerFromId(selectedObject->layerMask->layerMask)->layerName);
							else
								layer = "Select Layer";

							std::vector<std::string> layers = LayerManager::getLayerNames();

							if (ImGui::BeginCombo("###CURR_LAYER", layer.c_str(), ImGuiComboFlags_None))
							{
								for (std::string tmp : layers)
								{
									bool isSelected = false;

									String^ managedType = gcnew String(layer.c_str());
									String^ data = gcnew String(tmp.c_str());

									data = data->Substring(data->IndexOf("- ") + 2);

									if (data->CompareTo(managedType) == 0)
									{
										isSelected = true;
									}

									if (ImGui::Selectable(tmp.c_str(), isSelected))
									{
										data = gcnew String(tmp.c_str());

										Console::WriteLine(data);

										String^ buffer = data->Substring(0, data->IndexOf(" - "));

										Console::WriteLine(buffer);

										Layer^ l = LayerManager::GetLayerFromId(int::Parse(buffer));

										Console::WriteLine(l->layerName);

										selectedObject->layerMask = l;
									}
								}

								ImGui::EndCombo();
							}

							ImGui::EndMenu();
						}

						ImGui::SeparatorText("Instance Properties");

						if (ImGui::MenuItem("Change Parent"))
						{
							if (!readonlyLock)
								reparentLock = true;
						}

						if (ImGui::BeginMenu("Tagging"))
						{
							const char* c = CastToNative(selectedObject->GetTag());

							char* objectName = new char[sizeof(c) + 8];

							strcpy(objectName, c);

							if (ImGui::InputText("Tag", objectName, selectedObject->GetTag()->Length + 8))
							{
								selectedObject->SetTag(gcnew String(objectName));
							}

							ImGui::EndMenu();
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenuBar();
				}


				ImGui::SeparatorText("Object Properties");
				char* objectName = new char[128];

				strcpy(objectName, CastToNative(selectedObject->name));

				if (ImGui::InputText("Name", objectName, 128, ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
				{
					selectedObject->name = gcnew String(objectName);
				}

				ImGui::SeparatorText("Transform");

				{
					// position
					float pos[3] = {
						selectedObject->GetTransform()->position->x,
						selectedObject->GetTransform()->position->y,
						selectedObject->GetTransform()->position->z
					};

					if (ImGui::DragFloat3("Position", pos, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
					{
						selectedObject->GetTransform()->position = gcnew Engine::Components::Vector3(pos[0], pos[1], pos[2]);
					}

					// rotation
					float rot[3] = {
						selectedObject->GetTransform()->rotation->x,
						selectedObject->GetTransform()->rotation->y,
						selectedObject->GetTransform()->rotation->z
					};

					if (ImGui::DragFloat3("Rotation", rot, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
					{
						selectedObject->GetTransform()->rotation = gcnew Engine::Components::Vector3(rot[0], rot[1], rot[2]);
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
						selectedObject->GetTransform()->scale = gcnew Engine::Components::Vector3(scale[0], scale[1], scale[2]);
					}
				} // Transform

				SpecializedPropertyEditor(selectedObject);
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
						SetEditorCode(CastStringToNative("Data/" + scene->sceneRequirements + ".asset"), CastStringToNative(System::IO::File::ReadAllText("Data/" + scene->sceneRequirements + ".asset")));
						codeEditorOpen = true;
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImVec2 size = ImGui::GetWindowSize();

			ImGui::BeginListBox("###ASSETS", { size.x - 20, size.y - 55 });

			createAssetEntries("./Data/");

			ImGui::EndListBox();


			ImGui::End();
		}

		if (styleEditor)
		{
			ImGui::Begin("StyleEditor");
			{
				ImGui::ShowStyleEditor(&ImGui::GetStyle());
				ImGui::End();
			}
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
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						0,
						0,
						0,
						0xFFFFFFFF
					);
					meshRenderer->SetParent(scene->GetDatamodelMember("workspace"));
					scene->AddObjectToScene(meshRenderer);
					scene->PushToRenderQueue(meshRenderer);
				}

				if (ImGui::Button("Point Light"))
				{
					auto meshRenderer = gcnew Engine::EngineObjects::LightSource(
						"Point Light",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						0xFF0000FF,
						rPBR::LightType::LIGHT_POINT,
						gcnew Engine::Components::Vector3(1.0f, 1.0f, 1.0f),
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
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						)
					);
					meshRenderer->SetParent(scene->GetDatamodelMember("workspace"));
					scene->AddObjectToScene(meshRenderer);
					scene->PushToRenderQueue(meshRenderer);
				}

				if (ImGui::Button("Skybox"))
				{
					auto skyBox = gcnew Engine::EngineObjects::Skybox(
						"Skybox",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						2,
						0,
						0
					);
					skyBox->SetParent(scene->GetDatamodelMember("workspace"));
					scene->AddObjectToScene(skyBox);
					scene->PushToRenderQueue(skyBox);
				}

				if (ImGui::Button("Cube Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::CubeRenderer(
						"Cube",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						0xFFFFFFFF
					);
					skyBox->SetParent(scene->GetDatamodelMember("workspace"));
					scene->AddObjectToScene(skyBox);
					scene->PushToRenderQueue(skyBox);
				}

				if (ImGui::Button("BoundingBox Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::BoundingBoxRenderer(
						"BoundingBox",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						0xFFFFFFFF
					);
					skyBox->SetParent(scene->GetDatamodelMember("workspace"));
					scene->AddObjectToScene(skyBox);
					scene->PushToRenderQueue(skyBox);
				}

				if (ImGui::Button("Grid Renderer"))
				{
					auto skyBox = gcnew Engine::EngineObjects::GridRenderer(
						"Grid",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							nullptr
						),
						8,
						1.0f
					);
					skyBox->SetParent(scene->GetDatamodelMember("editor only"));
					scene->AddObjectToScene(skyBox);
					scene->PushToRenderQueue(skyBox);
				}

				if (ImGui::Button("Lua Script"))
				{
					auto luaScript = gcnew Engine::EngineObjects::LuaScript(
						"LuaScript",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							scene->GetDatamodelMember("workspace")->GetTransform()
						)
					);
					scene->AddObjectToScene(luaScript);
					scene->PushToRenderQueue(luaScript);
				}

				if (ImGui::Button("Mesh Renderer"))
				{
					auto meshRenderer = gcnew Engine::EngineObjects::MeshRenderer(
						"MeshRenderer",
						gcnew Engine::Internal::Components::Transform(
							gcnew Engine::Components::Vector3(0, 0, 0),
							gcnew Engine::Components::Vector3(0, 0, 0),
							0.0f,
							gcnew Engine::Components::Vector3(1, 1, 1),
							scene->GetDatamodelMember("workspace")->GetTransform()
						),
						0,
						gcnew List<unsigned int>(),
						0xFFFFFFFF
					);
					scene->AddObjectToScene(meshRenderer);
					scene->PushToRenderQueue(meshRenderer);
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
							Engine::EngineObjects::Script^ retn = assembly->Create<Engine::EngineObjects::Script^>(T->FullName);
							scene->PushToRenderQueue(retn);
							scene->AddObjectToScene(retn);
							//WinAPI::MBOX((void*)GetWindowHandle(), "NOT IMPLEMENTED YET", "Gold Editor", 0x00000040L | 0x00000000L);
						}
					}
				}
			}

			ImGui::End();
		}

		if (ImGui::Begin("Console", &isOpen))
		{
			if (ImGui::Button("Clear"))
			{
				Engine::Scripting::Logging::clearLogs();
			}

			const ImVec2 windowSize = ImGui::GetWindowSize();

			if (ImGui::BeginListBox("###CONSOLE_OUTPUT", { windowSize.x - 20, windowSize.y - 60 }))
			{
				for each (Engine::Scripting::Log ^ log in Engine::Scripting::Logging::getLogs())
				{
					switch (log->logType)
					{
					case TraceLogLevel::LOG_INFO:
						ImGui::TextColored({ 1.0f,1.0f, 1.0f, 1.0f }, CastStringToNative(log->message).c_str());
						break;
					case TraceLogLevel::LOG_DEBUG:
						ImGui::TextColored({ 0.141f, 0.851f, 0.929f, 1.0f }, CastStringToNative(log->message).c_str());
						break;
					case TraceLogLevel::LOG_FATAL:
						ImGui::TextColored({ 0,0,0,1.0f }, CastStringToNative(log->message).c_str());
						break;
					case TraceLogLevel::LOG_ERROR:
						ImGui::TextColored({ 1.0f,0,0,1.0f }, CastStringToNative(log->message).c_str());
						break;
					case TraceLogLevel::LOG_WARNING:
						ImGui::TextColored({ 1.0f, 0.533f, 0.0f, 1.0f }, CastStringToNative(log->message).c_str());
						break;
					}
				}

				ImGui::EndListBox();
			}

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
			//ImGui::OpenPopup("Scene Loader Editor");
		}
		else if (b7)
		{
			ImGui::OpenPopup("Save/Load Style");
		}
		else if (b8)
		{
			ImGui::OpenPopup("Layer Editor");
		}

		fileExplorer->DrawExplorer();
		fileExplorer->TryComplete();

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

		if (ImGui::BeginPopupModal("Layer Editor", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Save/Load Style", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{
			char* tmp = new char[styleFN.size() + 32] {};

			strcpy(tmp, styleFN.c_str());

			ImGui::Text("File Name:");
			ImGui::SameLine();
			if (ImGui::InputText("###FILE_NAME", tmp, styleFN.size() + 32, ImGuiInputTextFlags_CallbackCompletion))
			{
				styleFN = tmp;
			}

			ImGui::NewLine();

			if (ImGui::Button("Save"))
			{
				ImGui::SaveStyle(styleFN.c_str(), ImGui::GetStyle());
				b7 = false;
				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Load"))
			{
				ImGui::LoadStyle(styleFN.c_str(), ImGui::GetStyle());
				b7 = false;
				ImGui::CloseCurrentPopup();
			}

			delete tmp;

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Scene Loader Editor", (bool*)false, ImGuiWindowFlags_NoScrollbar))
		{
			int heapAlloc = 512;
			char* data = new char[8192];
			auto t = System::IO::File::ReadAllText("Data/" + scene->sceneRequirements + ".asset");

			int size = t->ToCharArray()->Length + heapAlloc;
			data = new char[size];

			strcpy(data, CastStringToNative(t).c_str());

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
				scene->LoadScene();
				create();
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
				scene = SceneManager::LoadSceneFromFile(gcnew System::String(fileName), scene, passwd);
				scene->LoadScene();
				create();
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
		Engine::Utils::LogReporter::singleton->CloseThread();
		UnloadRenderTexture(viewportTexture);
		dataPack.FreeAll();
		exit(0);
	}

	void render(int currentLayer)
	{
		while (currentLayer != LayerManager::getHigherLayer())
		{
			Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

			if (cL == nullptr)
				break;

			for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
			{
				if (scene->sceneLoaded())
				{
					Engine::Internal::Components::Object^ reference = (Engine::Internal::Components::Object^)sceneObject->GetReference();

					if (reference->layerMask = cL)
					{
						reference->Draw();
						reference->DrawGizmo();
					}
				}
			}

			Layer^ nextLayer = LayerManager::getNextHigherLayer(cL);

			if (nextLayer != nullptr)
				currentLayer = nextLayer->layerMask;
			else
				break;
		}
	}

	void Draw() override
	{
		BeginDrawing();
		{
			BeginTextureMode(viewportTexture);

			ClearBackground(GetColor(scene->skyColor));

			Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetFirstObjectOfType<Engine::EngineObjects::Camera^>();

			if (camera == nullptr)
				return;

			BeginMode3D((::Camera3D)*camera->get());

			int currentLayer = 1;
			render(currentLayer);

			EndMode3D();

			DrawFPS(0, 0);

			EndTextureMode();

			rlImGuiBegin();

			ImGui::Begin("DemoVer", (bool*)true, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking);
			{
				ImGui::SetWindowSize(ImVec2(285, 20), 0);
				ImGui::SetWindowPos(ImVec2(0, GetScreenHeight() - 25), 0);
				ImGui::TextColored(ImVec4(255, 255, 255, 255), ENGINE_VERSION);
				ImGui::End();
			}

			for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
			{
				if (obj != nullptr)
				{
					if (obj->GetReference() != nullptr)
					{
						obj->GetReference()->DrawImGUI();
					}
				}
			}

			DrawImGui();

			rlImGuiEnd();
		}
		EndDrawing();
	}

private:
	void create()
	{
		scene->GetDatamodelMember("workspace");

		if (!scene->ExistsDatamodelMember("lighting"))
		{
			lightManager = gcnew LightManager("lighting",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					0.0f,
					gcnew Engine::Components::Vector3(0, 0, 0),
					nullptr
				),
				"Data/Engine/Shaders/rPBR/pbr.vert",
				"Data/Engine/Shaders/rPBR/pbr.frag"
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
				gcnew Engine::Components::Vector3(0, 0, 0),
				gcnew Engine::Components::Vector3(0, 0, 0),
				0.0f,
				gcnew Engine::Components::Vector3(0, 0, 0),
				nullptr
			),
			lightManager
		);
		lightdm->SetParent(daemonParent);
		scene->PushToRenderQueue(lightdm);
		scene->AddObjectToScene(lightdm);

		if(!ObjectManager::singleton()->GetGameObjectsByName("EditorCamera")[0])
		{

			auto camera3D = gcnew Engine::EngineObjects::Camera("EditorCamera",
				gcnew Engine::Internal::Components::Transform(
					Engine::Components::Vector3::create({ 0,0,0 }),
					Engine::Components::Vector3::create({ 0,0,0 }),
					0.0f,
					Engine::Components::Vector3::create({ 1,1,1 }),
					nullptr
				),
				CAMERA_PERSPECTIVE
			);

			scene->PushToRenderQueue(camera3D);
			scene->PushToRenderQueue(camera3D);
		}
	}

public:
	void Init() override
	{
		DataPack::SetSingletonReference(packedData);

		modelTexture = DataPack::singleton()->AddTextures2D(256, "./Data/EditorAssets/Icons/Model.png");
		DataPack::singleton()->AddTextures2D(257, "Data/EditorAssets/Icons/Run.png");
		DataPack::singleton()->AddTextures2D(258, "Data/EditorAssets/Icons/Stop.png");
		DataPack::singleton()->AddTextures2D(259, "Data/EditorAssets/Icons/Material.png");
		DataPack::singleton()->AddShader(0, "Data/Engine/Shaders/base.vs", "Data/Engine/Shaders/base.fs");

		materialTexture = DataPacks::singleton().GetTexture2D(259);

		packedData->WriteToFile(packedData->getFile(), passwd);

		/*
		Shader lightShader = dataPack.GetShader(1);

		lightShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(lightShader, "viewPos");

		dataPack.SetShader(1, lightShader);
		*/

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

		//int numLights = 1;
		//SetShaderValue(dataPack.GetShader(1), GetShaderLocation(dataPack.GetShader(1), "numOfLights"), &numLights, SHADER_UNIFORM_INT);
		//rPBR::PBRSetAmbient(dataPack.GetShader(1), { 0, 0, 0, 255 }, 0.0f);
		//lights[0] = rPBR::PBRLightCreate(rPBR::LIGHT_POINT, { -5,5,0 }, { .5f,-.5f,0 }, { 255,255,255, 255 }, 2.5f, dataPack.GetShader(1));

		//FileManager::ReadCustomFileFormat("Data/assets1.gold", passwd);

		cameraPosition = gcnew Engine::Components::Vector3(0, 0, 0);
		//Directory::Delete("Data/tmp/", true);
		//SceneManager::SaveSceneToFile(scene, passwd);
		//packedData->WriteToFile("Assets1", passwd);

		objectManager = gcnew Scripting::ObjectManager(scene);

		create();
	}

	void Preload() override
	{
		ImGui::LoadStyle("EditorStyle.ini");

		if (FirstTimeBoot())
		{
			WinAPI::MBOXA(GetWindowHandle(), "LPVOID* voidFunc = (LPVOID*)nativeData;\nvoidFunc->Test();", "GoldEngine - Ver 0.5c - editor", 0x00000040L | 0x00000000L);
			Boot();
		}
		SetExitKey(KEY_NULL);
		viewportTexture = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

		scene = SceneManager::CreateScene("GoldEngineBoot");

		scene = SceneManager::LoadSceneFromFile("Level0", scene, passwd);
		scene->LoadScene();

		while (!scene->sceneLoaded())
		{
			DataManager::HL_Wait(1.0f);
		}

		packedData = scene->getSceneDataPack();

		for each (EngineAssembly ^ assm in assemblies)
		{
			for each (Type ^ asmType in assm->getPreloadScripts())
			{
				try
				{
					asmType->GetMethod("Preload")->Invoke(nullptr, nullptr);
				}
				catch (Exception^ ex)
				{
					printError(ex->Message);
					printError(ex->StackTrace);
				}
			}
		}

		// initialize editor assets

		// end of editor assets

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
		Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetFirstObjectOfType<Engine::EngineObjects::Camera^>();

		if (camera == nullptr)
			return;

		if (showCursor)
			//UpdateCamera(camera->get(), CAMERA_FREE);

		if (fpsCap)
		{
			SetFPS(60);
		}
		else
		{
			SetFPS(-1);
		}

		Shader lightShader = dataPack.GetShader(1);
		float cameraPos[3] = { camera->transform->position->x, camera->transform->position->y, camera->transform->position->z };
		SetShaderValue(lightShader, lightShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

		dataPack.AddShader(1, lightShader);

		for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
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
	System::Collections::Generic::List<EngineAssembly^>^ assemblies;
	Scene^ scene;
	DataPack^ packedData;
	Camera* defaultCamera;

public:
	GameWindow()
	{
		dataPack = DataPacks();
		assemblies = gcnew System::Collections::Generic::List<EngineAssembly^>();

		assemblies->Add(gcnew EngineAssembly("Data/Asm/GoldEngine_ScriptAssembly.dll"));
		assemblies->Add(gcnew EngineAssembly(System::Reflection::Assembly::GetExecutingAssembly()));

		SceneManager::SetAssemblyManager(assemblies);

		Start();
	}

	virtual void Start() override
	{
		auto secrets = Engine::Config::EngineSecrets::ImportSecrets("./Data/Keys/secrets.dat");

		passwd = CypherLib::GetPasswordBytes(secrets->encryptionPassword);

		auto config = Engine::Config::EngineConfiguration::ImportConfig("./Data/appinfo.dat");

		SetWindowFlags(config->windowFlags);
		OpenWindow(config->resolution->w, config->resolution->h, config->getWindowName().c_str());
		//SetWindowPosition(config->resolution->x, config->resolution->y);

		if (Directory::Exists(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/'))))
		{
			if (File::Exists(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath))
			{
				File::Delete(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath);
			}

			Directory::Delete(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/')));
		}

		Directory::CreateDirectory(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath->Substring(0, config->logPath->IndexOf('/')));
		gcnew Engine::Utils::LogReporter(System::Environment::GetFolderPath(Environment::SpecialFolder::ApplicationData) + "/../LocalLow/" + config->logPath);

		LayerManager::RegisterDefaultLayers();

		Preload();

		Loop();
	}

	virtual void Init() override
	{
		DataPack::SetSingletonReference(packedData);
	}

	virtual void Preload() override
	{
		scene = SceneManager::CreateScene("GoldEngineBoot");

		scene = SceneManager::LoadSceneFromFile("Level0", scene, passwd);
		scene->LoadScene();

		while (!scene->sceneLoaded())
		{
			DataManager::HL_Wait(1.0f);
		}

		packedData = scene->getSceneDataPack();

		DataManager::HL_CreateCamera(0, gcnew Engine::Components::Vector3(0, 0, 0), CameraType::C3D); // create 3d camera

		defaultCamera = &DataPacks::singleton().GetCamera3D(0);
		defaultCamera->projection = CAMERA_PERSPECTIVE;
		defaultCamera->fovy = 60;

		Init();
	}

	void render(int currentLayer)
	{
		while (currentLayer != LayerManager::getHigherLayer())
		{
			Layer^ cL = LayerManager::GetLayerFromId(currentLayer);

			if (cL == nullptr)
				break;

			for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
			{
				if (scene->sceneLoaded())
				{
					Engine::Internal::Components::Object^ reference = (Engine::Internal::Components::Object^)sceneObject->GetReference();

					if (reference->layerMask = cL)
					{
						reference->Draw();
					}
				}
			}

			Layer^ nextLayer = LayerManager::getNextHigherLayer(cL);

			if (nextLayer != nullptr)
				currentLayer = nextLayer->layerMask;
			else
				break;
		}
	}

	virtual void Draw() override
	{
		BeginDrawing();

		ClearBackground(BLACK);

		BeginMode3D((Camera3D)*defaultCamera);

		int currentLayer = 0;

		render(currentLayer);

		EndMode3D();

		rlImGuiBegin();

		for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
		{
			sceneObject->GetReference()->DrawImGUI();
		}

		rlImGuiEnd();

		EndDrawing();
	}

	virtual void Update() override
	{
		UpdateCamera(defaultCamera, CAMERA_FREE);

		for each (Engine::Management::MiddleLevel::SceneObject ^ sceneObject in scene->GetRenderQueue())
		{
			sceneObject->GetReference()->Update();
		}
	}

	virtual void Exit() override
	{
		DataManager::HL_FreeAll();
		Engine::Utils::LogReporter::singleton->CloseThread();
	}
};

#pragma endregion

#endif

int main()
{
	passwd = CypherLib::GetPasswordBytes(gcnew String(ENCRYPTION_PASSWORD));

#if PRODUCTION_BUILD
	gcnew GameWindow();
#else
	gcnew EditorWindow();
#endif
}
