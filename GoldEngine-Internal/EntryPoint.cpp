#include "Macros.h"

int max_lights = 4;
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
#include "EngineGC.h"
#include "imgui/FileExplorer/filedialog.h"

// CUSTOM RENDERERS \\

#include "VoxelRenderer.h"

// INCLUDE ENGINE CLASSES \\

#include "SceneFormat.h"
#include "EngineIncludes.h"

// last but not least, the assembly loader.

#include "Time.h"
#include "ImguiHook.h"
#include "InputManager.h"
#include "ObjectManager.h"
#include "ShaderManager.h"
#include "LogFileReporter.h"
#include "AsmLoader.h"
#include "GraphicsWrapper.h"

// Daemons (Daemons are tasks that are ran mandatory by the engine, these cannot be displayed by the hierarchy)

#include "Objects/LightDm.h"

// Preload queue & scripts, can be used for loading/unloading certain data or doing operations with shaders, materials, models whatever.

#include "PreloadScript.h"

#include "native/json.hpp"
#include "imguistyleserializer.h"

// lua virtual machine
#include "LuaVM.h"
#include "ManagedSignal.h"
// lua script object
#include "Objects/Script.h"
#include "Objects/LuaScript.h"

#include "Objects/Pipeline/ScriptableRenderPipeline.hpp"
#include "RenderPipelines/LitPBR_SRP.h"
#include "RenderPipelines/RaymarchSRP.h"
#include "RenderPipelines/LightweightSRP.h"

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

void engine_bootstrap()
{
	if (!Directory::Exists("Bin/"))
		Directory::CreateDirectory("Bin");

	if(!Directory::Exists("Bin/Asm"))
		Directory::CreateDirectory("Bin/Asm");

	if (!Directory::Exists("Data"))
		Directory::CreateDirectory("Data");

	if (!Directory::Exists("Data/Keys/"))
		Directory::CreateDirectory("Data/Keys/");

	if (!File::Exists("Data/Keys/map.iv"))
	{
		FileStream^ fs = File::Open("Data/Keys/map.iv", System::IO::FileMode::OpenOrCreate);
		BinaryWriter^ writer = gcnew BinaryWriter(fs);

		Random^ r = gcnew Random();

		writer->Write(System::Guid::NewGuid().ToString());
		writer->Close();
		fs->Close();
	}

	if (!Directory::Exists("Data/UserData/"))
		Directory::CreateDirectory("Data/UserData/");
}

#if !defined(PRODUCTION_BUILD)

#pragma region EDITOR ENGINE

Model mod;
float cameraSpeed = 1.25f;
bool controlCamera = true;
RenderTexture viewportTexture;
bool isOpen = true;
bool toggleControl = true;
bool initSettings = false;
bool styleEditor = false;
bool showCursor = true;
bool b1, b2, b3, b4, b5, b6, b7, b8, b9;
bool visualizeError;
bool readonlyLock = false;
bool fpsCap = true;
bool fpsCheck = true;
bool reparentLock = false;
char* consoleBufferData = new char[4096];

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
char* packDataFileName = new char[64];
int positionSelector = 0;

bool gameViewMode = false;

// error handling
char* errorReason;

bool ce1, ce2, ce3, ce4, ce5, ce6;

std::string codeEditorChunk;

char fileName[] = "Level0";

typedef enum assetDisplay
{
	ALL,
	MODELS,
	TEXTURES,
	AUDIO
};

assetDisplay displayingAssets;

int displayingAsset = 0;


ref class EditorWindow : Engine::Window
{
private:
	Layer^ selectedLayer;
	DataPack^ packedData;
	Scene^ scene;
	Engine::EngineObjects::LightManager^ lightManager;
	Engine::Components::Vector3^ cameraPosition;
	Engine::Internal::Components::Object^ selectedObject;
	int selectedObjectIndex;
	bool selectionLock = false;
	Engine::Internal::Components::Object^ selectionObject;

	Engine::Internal::Components::Object^ reparentObject;
	System::Collections::Generic::List<EngineAssembly^>^ assemblies;
	Engine::Lua::VM::LuaVM^ luaVM;
	Engine::Editor::Gui::fileExplorer^ fileExplorer = gcnew Engine::Editor::Gui::fileExplorer(std::string("File Explorer"));
	System::Collections::Generic::List<String^>^ loadedAssets;
	Engine::Render::ScriptableRenderPipeline^ renderPipeline;

private:
	void SaveEditorCode()
	{
		if (codeEditorFile != "")
		{
			File::WriteAllText(gcnew String(codeEditorFile.c_str()), gcnew String(codeEditor->GetText().c_str()));
		}
	}

	String^ GetParentRoute(Engine::Internal::Components::Transform^ transform)
	{
		if (transform != nullptr && transform->parent != nullptr)
		{
			auto parentInstance = Singleton<ObjectManager^>::Instance->GetObjectByUid(transform->parent->GetUID());
			return GetParentRoute(parentInstance->transform) + "/" + parentInstance->name;
		}
		else
			return "World";
	}

	String^ GetAccessRoute(Engine::Internal::Components::Object^ object)
	{
		return GetParentRoute(object->transform) + "/" + object->name;
	}

	void createAssetEntries(String^ path)
	{
		for each (String ^ f in Directory::GetFiles(path))
		{
			f = f->Replace(R"(\)", "/");
			array<String^>^ tmp = f->Split('/');
			auto t = tmp[tmp->Length - 1] + "\n";

			if ((f->Contains(".obj") || f->Contains(".glb") || f->Contains(".gltf") || f->Contains(".vox")) && (displayingAssets == ALL || displayingAssets == MODELS)) // model types
			{
				if (rlImGuiImageButton(CastStringToNative("###" + t).c_str(), &modelTexture))
				{
					unsigned int assetId = 0;
					auto res = packedData->hasAsset(Engine::Assets::Management::assetType::_Model, f);
					if (!std::get<0>(res))
					{
						assetId = packedData->GetAssetID(Engine::Assets::Management::assetType::_Model);

						packedData->AddModel(assetId, f);

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

			if ((f->Contains(".png") || f->Contains(".jpg") || f->Contains(".bmp") || f->Contains(".hdr")) && (displayingAssets == ALL || displayingAssets == TEXTURES))
			{
				if (rlImGuiImageButton(CastStringToNative("###" + t).c_str(), &materialTexture))
				{
					unsigned int assetId = 0;
					auto res = packedData->hasAsset(Engine::Assets::Management::assetType::_Texture2D, f);
					if (!std::get<0>(res))
					{
						assetId = packedData->GetAssetID(Engine::Assets::Management::assetType::_Texture2D);

						packedData->AddTextures2D(assetId, f);

						packedData->WriteToFile(packedData->getFile(), passwd);
					}
					else
					{
						assetId = std::get<1>(res);
					}
				}
				ImGui::SameLine();
				ImGui::Text(CastStringToNative(t).c_str());
			}
		}

		for each (String ^ dir in Directory::GetDirectories(path))
		{
			createAssetEntries(dir);
		}
	}

	void SpecializedPropertyEditor(Engine::Internal::Components::Object^ object)
	{
		if (object != nullptr)
		{
			auto type = object->type;

			switch (type)
			{
			case ObjectType::CubeRenderer:
			{
				Engine::EngineObjects::CubeRenderer^ renderer = Cast::Dynamic<Engine::EngineObjects::CubeRenderer^>(object);

				unsigned int tint = renderer->color;

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
					renderer->SetColor(ImGui::ColorConvertFloat4ToU32(ImVec4(rawData[0], rawData[1], rawData[2], rawData[3])));
				}

			}
			break;

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

				int lightType = light->lightType;
				ImGui::Text("Intensity:");
				ImGui::SameLine();
				const char* data[] = { "Directional Light", "Point Light", "Spot Light" };

				if (ImGui::Combo("###LIGHT_TYPE", &lightType, data, IM_ARRAYSIZE(data)))
				{
					light->lightType = (rPBR::LightType)lightType;
				}

				float intensity = light->intensity;
				ImGui::Text("Radius:");
				ImGui::SameLine();
				if (ImGui::DragFloat("###LIGHT_INTENSITY", &intensity, 0.25f, 0.1f, float::MaxValue, "%.1f"))
				{
					light->intensity = intensity;
				}
				int shaderId = light->shaderId;
				ImGui::Text("Shader Id:");
				ImGui::SameLine();
				if (ImGui::InputInt("###SHADER_ID", &shaderId, 1, 1))
				{
					light->shaderId = shaderId;
				}


				float lightPower = light->lightPower;
				ImGui::Text("Light Intensity:");
				ImGui::SameLine();

				if (ImGui::DragFloat("###LIGHT_POWER", &lightPower, 100.0f, float::MinValue, float::MaxValue, "%.1f"))
				{
					light->lightPower = lightPower;
				}

				if ((rPBR::LightType)lightType == rPBR::LightType::LIGHT_DIRECTIONAL || (rPBR::LightType)lightType == rPBR::LightType::LIGHT_SPOT)
				{
					Engine::Components::Vector3^ target = light->target;

					float nativeVector[3] = { light->target->x, light->target->y, light->target->z };

					ImGui::Text("Target: ");
					ImGui::SameLine();
					if (ImGui::DragFloat2("###LIGHT_TARGET", nativeVector, 1.0f, float::MinValue, float::MaxValue, "%.3f"))
					{
						light->target = gcnew Engine::Components::Vector3(nativeVector[0], nativeVector[1], nativeVector[2]);
					}
				}

				if ((rPBR::LightType)lightType == rPBR::LightType::LIGHT_SPOT)
				{
					float cutoff = light->cutoff;
					ImGui::Text("Cutoff:");
					ImGui::SameLine();
					if (ImGui::DragFloat("###LIGHT_CUTOFF", &cutoff, 0.5f, 0.1f, float::MaxValue, "%.1f"))
					{
						light->cutoff = cutoff;
					}


					float outercutoff = light->outerCutoff;
					ImGui::Text("Outer Cutoff:");
					ImGui::SameLine();
					if (ImGui::DragFloat("###LIGHT_OUTERCUTOFF", &outercutoff, 0.5f, 0.1f, float::MaxValue, "%.1f"))
					{
						light->outerCutoff = outercutoff;
					}
				}

				auto float4 = ImGui::ColorConvertU32ToFloat4(ImU32(light->lightColor));

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
					light->lightColor = (ImGui::ColorConvertFloat4ToU32(ImVec4(rawData[0], rawData[1], rawData[2], rawData[3])));
				}

			}
			break;

			case ObjectType::Script:
			{
				Engine::EngineObjects::ScriptBehaviour^ script = (Engine::EngineObjects::ScriptBehaviour^)object;

				if (script->assemblyReference->Contains("LuaScript"))
				{
					auto scr = (Engine::EngineObjects::LuaScript^)script;

					ImGui::SeparatorText("Linked Source");

					ImGui::Text("Binary file path: ");
					ImGui::SameLine();

					std::string nativePath = CastStringToNative(scr->luaFilePath);

					char* data = nativePath.data();

					if (ImGui::InputText("###LUA_LINKEDSOURCE",
						data, scr->luaFilePath->Length + (8 * 32), ImGuiInputTextFlags_EnterReturnsTrue))
					{
						scr->luaFilePath = gcnew String(data);
					}

					if (ImGui::Button("Reload lua source"))
					{
						scr->Reset();
					}
				}

				ImGui::SeparatorText("Attributes");

				if (ImGui::BeginListBox("###ATTRIBUTE_LISTBOX", { ImGui::GetWindowWidth() - 20, ImGui::GetWindowHeight() - 240 }))
				{
					for each (Engine::Scripting::Attribute ^ attrib in script->attributes->attributes)
					{
						if (attrib != nullptr)
						{
							ImGui::Text(CastStringToNative(attrib->name + " (" + attrib->getValueType()->Name + ")").c_str());
							if (attrib->getValueType()->Equals(String::typeid))
							{
								String^ value = (String^)attrib->getValue();
								int valueLen = value->Length;
								char* data = new char[valueLen * 8];

								strcpy(data, CastStringToNative(value).c_str());

								ImGui::SetNextItemWidth(ImGui::GetWindowWidth() - 25);
								if (ImGui::InputText(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), data, valueLen * 8))
								{
									attrib->setValue(gcnew String(data));

								}
								delete[] data;
							}
							else if (attrib->getValueType()->Equals(Engine::Components::Vector3::typeid))
							{
								Engine::Components::Vector3^ vector = attrib->getValue<Engine::Components::Vector3^>();

								float data[3] = { vector->x, vector->y, vector->z };

								if (ImGui::DragFloat3(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), data, 1.0f, -INFINITY, INFINITY, "%.2f"))
								{
									attrib->setValue(Engine::Components::Vector3::create(data));
								}
							}
							else if (attrib->getValueType()->Equals(Engine::Components::Vector2::typeid))
							{
								Engine::Components::Vector2^ vector = (Engine::Components::Vector2^)attrib->getValue();

								float data[2] = { vector->x, vector->y };

								if (ImGui::DragFloat2(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), data, 1.0f, -INFINITY, INFINITY, "%.2f"))
								{
									attrib->setValue(Engine::Components::Vector2::create(data));
								}
							}
							else if (attrib->getValueType()->Equals(UInt32::typeid))
							{
								int value = (unsigned int)attrib->getValue();

								if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
								{
									attrib->setValue(gcnew UInt32(value), false);
									attrib->setType(UInt32::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(Int32::typeid))
							{
								int value = (int)attrib->getValue();

								if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
								{
									attrib->setValue(gcnew Int32(value), false);
									attrib->setType(Int32::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(Int64::typeid))
							{
								long long tmp = (Int64)attrib->getValue();

								int value = (int)tmp;

								if (ImGui::InputInt(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 1, 1))
								{
									attrib->setValue(gcnew Int64(value), false);
									attrib->setType(Int64::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(float::typeid))
							{
								float tmp = (float)attrib->getValue();

								float value = (float)tmp;

								if (ImGui::InputFloat(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 0.1f, 0.5f, "%.2f"))
								{
									attrib->setValue(value, false);
									attrib->setType(float::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(Single::typeid))
							{
								float tmp = (float)attrib->getValue();

								float value = (float)tmp;

								if (ImGui::InputFloat(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 0.1f, 0.5f, "%.2f"))
								{
									attrib->setValue(value, false);
									attrib->setType(float::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(Engine::Internal::Components::Object::typeid) || attrib->getValueType()->IsSubclassOf(Engine::Internal::Components::Object::typeid) || attrib->getValueType()->IsSubclassOf(Engine::EngineObjects::ScriptBehaviour::typeid) || attrib->getValueType()->IsSubclassOf(Engine::EngineObjects::Script::typeid))
							{
								std::string temp = std::string("");
								if (attrib->userData == nullptr)
								{
									temp = CastStringToNative("NOT ASSIGNED - (NULL)");
								}
								else
								{
									Engine::Internal::Components::Object^ value = attrib->getValueAs<Engine::Internal::Components::Object^>();

									temp = CastStringToNative(value->name + " - (" + GetAccessRoute(value) + ")");
								}

								if (ImGui::Button(temp.c_str()))
								{
									if (!selectionLock)
										selectionLock = true;
									else
									{
										if (selectionObject->GetType() == attrib->getValueType() || attrib->getValueType()->IsAssignableFrom(selectionObject->GetType()))
										{
											selectionLock = false;
											attrib->setValue(selectionObject, false);
										}
										else
										{
											selectionLock = false;
											attrib->setValue(nullptr, false);
										}
									}
								}
							}
							else if (attrib->getValueType()->Equals(Double::typeid))
							{
								float tmp = (double)attrib->getValue();

								float value = (double)tmp;

								if (ImGui::InputFloat(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value, 0.1f, 0.5f, "%.2f"))
								{
									attrib->setValue(value, false);
									attrib->setType(float::typeid);
								}
							}
							else if (attrib->getValueType()->Equals(bool::typeid))
							{
								bool tmp = (bool)attrib->getValue();

								bool value = (bool)tmp;

								if (ImGui::Checkbox(CastStringToNative("###PROPERTY_EDITOR_##" + attrib->name).c_str(), &value))
								{
									attrib->setValue(value, false);
									attrib->setType(bool::typeid);
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

	void OpenFileExplorer(std::string name, Engine::Editor::Gui::explorerMode mode, Engine::Editor::Gui::onFileSelected^ callback)
	{
		fileExplorer->SetWindowName(name);
		fileExplorer->setExplorerMode(mode);
		fileExplorer->Open();

		fileExplorer->OnCompleted(callback);
	}

	void CodeEditor()
	{
		if (ImGui::Begin("Embedded Code Editor", &codeEditorOpen, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar))
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
								OpenFileExplorer("Open File", Engine::Editor::Gui::explorerMode::Open, (gcnew Engine::Editor::Gui::onFileSelected(this, &EditorWindow::SetEditorCode)));
							}

							delete chunk;
							delete currentChunk;
						}
						else
						{
							OpenFileExplorer("Open File", Engine::Editor::Gui::explorerMode::Open, (gcnew Engine::Editor::Gui::onFileSelected(this, &EditorWindow::SetEditorCode)));
						}
					}
					if (ImGui::MenuItem("Save"))
					{
						codeEditorChunk = codeEditor->GetText();

						OpenFileExplorer("Save File", Engine::Editor::Gui::explorerMode::Save, (gcnew Engine::Editor::Gui::onFileSelected(this, &EditorWindow::SaveEditorContents)));
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit"))
					{
						codeEditorOpen = false;
					}
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Compiler"))
				{
					if (ImGui::BeginMenu("Lua Compiler"))
					{
						if (ImGui::MenuItem("Save Lua Bytecode"))
						{
							luaVM->LoadSource(gcnew String(codeEditor->GetText().c_str()));

							OpenFileExplorer("Save Lua Compiled File", Engine::Editor::Gui::explorerMode::Save, (gcnew Engine::Editor::Gui::onFileSelected(luaVM, &Engine::Lua::VM::LuaVM::WriteLuaCodeToFile)));
						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Language"))
				{
					if (ImGui::MenuItem("C++"))
					{
						language = TextEditor::LanguageDefinition::CPlusPlus();
					}
					if (ImGui::MenuItem("C#"))
					{
						language = TextEditor::LanguageDefinition::Cs();
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
					refName += (_reference->active == true) ? "" : "(INACTIVE)";

					if (_type == ObjectType::Daemon || _type == ObjectType::Datamodel || _type == ObjectType::LightManager || _reference->isProtected())
					{
						if (ImGui::Selectable(CastStringToNative(refName + " (ENGINE PROTECTED)" + "###" + _reference->GetTransform()->GetUID()).c_str()))
						{
							if (reparentLock)
								reparentObject = _reference;
							else if (selectionLock)
								selectionObject = _reference;
							else
							{
								readonlyLock = true;
								selectedObject = _reference;
								selectedObjectIndex = scene->GetRenderQueue()->IndexOf(selectedObject);
							}
						}
					}
					else
					{
						if (ImGui::Selectable(CastStringToNative(refName + "###" + _reference->GetTransform()->GetUID()).c_str()))
						{
							if (reparentLock)
								reparentObject = _reference;
							else if (selectionLock)
								selectionObject = _reference;
							else
							{
								readonlyLock = false;
								selectedObject = _reference;
								selectedObjectIndex = scene->GetRenderQueue()->IndexOf(selectedObject);
							}
						}
					}

					DrawHierarchyInherits(scene, _reference, depth + 1);
				}
			}
		}
	}

	void ExecAsIdentifiedObject(Engine::Internal::Components::ObjectType type, System::Object^ object)
	{
		if (scene->sceneLoaded())
		{
			Engine::Internal::Components::Object^ modelRenderer = (Engine::Internal::Components::Object^)object;
			modelRenderer->GameDraw();
			modelRenderer->GameDrawGizmos();
		}
	}

	void ThrowUIError(String^ eR)
	{
		std::string convErrRes = CastStringToNative(eR);

		errorReason = new char[convErrRes.size()];

		strcpy(errorReason, convErrRes.c_str());

		visualizeError = true;
	}

	void PackData(String^ convertedData)
	{
		if (!convertedData->Contains("."))
		{
			ImGui::CloseCurrentPopup();
			ThrowUIError("File does not have extension");
			ImGui::EndPopup();
			return;
		}
		else if (convertedData->Length <= 0)
		{
			ImGui::CloseCurrentPopup();
			ThrowUIError("File length is not valid");
			ImGui::EndPopup();
			return;
		}
		else if (convertedData->IsNullOrEmpty(convertedData))
		{
			ImGui::CloseCurrentPopup();
			ThrowUIError("FileName is not valid");
			ImGui::EndPopup();
			return;
		}

		print("[Asset Packer]: ", "------------------------");
		print("[Asset Packer]: ", "Packing data to -> " + convertedData);

		for each (auto asset in loadedAssets)
		{
			print("[Asset Packer]: ", "Packing asset -> " + asset);
		}

		print("[Asset Packer]: ", "------------------------");

		FileManager::WriteToCustomFile(convertedData, gcnew String(password), loadedAssets->ToArray());
	}

public:
	EditorWindow()
	{
#if HIDE_CONSOLE == true
		WinAPI::FreeCons();
#endif

		strcpy(password, ENCRYPTION_PASSWORD);
		strcpy(consoleBufferData, "");

		engine_bootstrap();

		luaVM = gcnew Engine::Lua::VM::LuaVM();
		assemblies = gcnew System::Collections::Generic::List<EngineAssembly^>();
		dataPack = DataPacks();

		assemblies->Add(gcnew EngineAssembly(System::Reflection::Assembly::GetExecutingAssembly()));

		for each (String ^ fileName in Directory::GetFiles("Bin\\Asm\\"))
		{
			if (fileName->Contains(".goldasm") || fileName->Contains(".dll"))
			{
				String^ path = Directory::GetCurrentDirectory() + "\\" + fileName;

				assemblies->Add(gcnew EngineAssembly(path));
			}
		}

		SceneManager::SetAssemblyManager(assemblies);

		for each (EngineAssembly ^ assembly in assemblies)
		{
			assembly->ListAssemblyTypes();
		}

		Start();
	}

	void Start()
	{
		SetTraceLogLevel(LOG_DEBUG);
		SetWindowFlags(FLAG_INTERLACED_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
		OpenWindow(1280, 720, (const char*)EDITOR_VERSION);

		gcnew Engine::Managers::SignalManager();

		if (File::Exists("./Data/Keys/secrets.dat"))
		{
			auto secrets = Engine::Config::EngineSecrets::ImportSecrets("./Data/Keys/secrets.dat");

			passwd = CypherLib::GetPasswordBytes(secrets->encryptionPassword);

			auto config = Engine::Config::EngineConfiguration::ImportConfig("./Data/appinfo.dat");

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
		}
		else
		{
			auto secrets = gcnew Engine::Config::EngineSecrets(ENCRYPTION_PASSWORD);

			secrets->ExportSecrets("./Data/Keys/secrets.dat");

			auto config = gcnew Engine::Config::EngineConfiguration();
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
		}

		LayerManager::RegisterDefaultLayers();

		Preload();

		Loop();
	}

#pragma region EditorGUI

	void ShowError()
	{
		if (ImGui::BeginPopupModal("Unexpected Error", (bool*)false, ImGuiWindowFlags_NoResize))
		{
			ImGui::Text(errorReason);

			if (ImGui::Button("Accept"))
			{
				visualizeError = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void DrawMainMenuBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Engine", true))
			{
				ImGui::SeparatorText("Encryption");

				if (ImGui::BeginMenu("Set encryption password"))
				{
					if (ImGui::InputText("###PASSWORD_SETTER", password, 512))
					{
						passwd = CypherLib::GetPasswordBytes(gcnew String(Engine::Config::EngineSecrets::singleton()->encryptionPassword));
					}

					ImGui::EndMenu();
				}

				if (ImGui::MenuItem("Export Secrets"))
				{
					Engine::Config::EngineSecrets::singleton()->ExportSecrets("./Data/Keys/secrets.dat");
				}

				ImGui::SeparatorText("Project Startup Settings");

				if (ImGui::BeginMenu("Engine Configuration"))
				{
					if (ImGui::MenuItem("Edit Engine Configuration"))
					{
						b9 = true;
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Export"))
					{
						Engine::Config::EngineConfiguration::singleton()->ExportConfig("./Data/appinfo.dat");
					}

					ImGui::EndMenu();
				}

				ImGui::SeparatorText("Project");

				if (ImGui::BeginMenu("Generate project"))
				{
					if (ImGui::MenuItem("C++ Template"))
					{

					}

					if (ImGui::MenuItem("C# Template"))
					{

					}

					ImGui::EndMenu();
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
				ImGui::SeparatorText("Render Pipelines");

				if (ImGui::BeginMenu("Pipelines"))
				{
					for each (EngineAssembly ^ assembly in assemblies)
					{
						auto types = assembly->GetTypesOf(Engine::Render::ScriptableRenderPipeline::typeid);

						for each (System::Type ^ pipeline in types)
						{
							if (ImGui::MenuItem(CastStringToNative(pipeline->Name).c_str()))
							{
								if (renderPipeline != nullptr)
									renderPipeline->OnUnloadPipeline();

								renderPipeline = (Engine::Render::ScriptableRenderPipeline^)assembly->CreateSimple(pipeline);
							}
						}
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenu();
			}

			ImGui::Separator();

			if (ImGui::BeginMenu("Object", true))
			{
				if (ImGui::MenuItem("Empty Object"))
				{
					Engine::EngineObjects::Script^ newObject = gcnew Engine::EngineObjects::Script("Empty Object",
						gcnew Engine::Internal::Components::Transform(
							Engine::Components::Vector3::create({ 0,0,0 }),
							Engine::Components::Vector3::create({ 0,0,0 }),
							Engine::Components::Vector3::create({ 1,1,1 }),
							scene->GetDatamodelMember("workspace")->GetTransform()
						)
					);

					scene->AddObjectToScene(newObject);
					scene->PushToRenderQueue(newObject);
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Primitives"))
				{
					if (ImGui::MenuItem("Cube Renderer"))
					{
						Engine::EngineObjects::CubeRenderer^ cubeRenderer = gcnew Engine::EngineObjects::CubeRenderer("CubeRenderer",
							gcnew Engine::Internal::Components::Transform(
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 1,1,1 }),
								scene->GetDatamodelMember("workspace")->GetTransform()
							), 0xFFFFFFFF);

						scene->AddObjectToScene(cubeRenderer);
						scene->PushToRenderQueue(cubeRenderer);
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Cameras"))
				{
					if (ImGui::MenuItem("Camera3D"))
					{
						Engine::EngineObjects::Camera^ newCamera = gcnew Engine::EngineObjects::Camera3D("Camera",
							gcnew Engine::Internal::Components::Transform(
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 1,1,1 }),
								nullptr
							));

						scene->AddObjectToScene(newCamera);
						scene->PushToRenderQueue(newCamera);
					}
					if (ImGui::MenuItem("Camera2D"))
					{
						Engine::EngineObjects::Camera^ newCamera = gcnew Engine::EngineObjects::Camera2D("Camera",
							gcnew Engine::Internal::Components::Transform(
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 1,1,1 }),
								nullptr
							));

						scene->AddObjectToScene(newCamera);
						scene->PushToRenderQueue(newCamera);
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("2D/3D"))
				{
					if (ImGui::BeginMenu("2D"))
					{
						if (ImGui::MenuItem("Sprite"))
						{
							auto sprite = gcnew Engine::EngineObjects::Sprite(
								"Sprite",
								gcnew Engine::Internal::Components::Transform(
									gcnew Engine::Components::Vector3(0, 0, 0),
									gcnew Engine::Components::Vector3(0, 0, 0),
									gcnew Engine::Components::Vector3(1, 1, 1),
									scene->GetDatamodelMember("workspace")->GetTransform()
								)
							);

							scene->AddObjectToScene(sprite);
							scene->PushToRenderQueue(sprite);
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("3D"))
					{
						if (ImGui::MenuItem("ModelRenderer"))
						{
							auto modelRenderer = gcnew Engine::EngineObjects::ModelRenderer(
								"ModelRenderer",
								gcnew Engine::Internal::Components::Transform(
									Engine::Components::Vector3::create({ 0,0,0 }),
									Engine::Components::Vector3::create({ 0,0,0 }),
									Engine::Components::Vector3::create({ 0,0,0 }),
									scene->GetDatamodelMember("workspace")->GetTransform()
								),
								0,
								0,
								0,
								0xFFFFFFFF
							);

							scene->AddObjectToScene(modelRenderer);
							scene->PushToRenderQueue(modelRenderer);
						}

						if (ImGui::MenuItem("MeshRenderer"))
						{

						}

						ImGui::EndMenu();
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("Lighting"))
				{
					if (ImGui::MenuItem("Point Light"))
					{
						auto meshRenderer = gcnew Engine::EngineObjects::LightSource(
							"Point Light",
							gcnew Engine::Internal::Components::Transform(
								gcnew Engine::Components::Vector3(0, 0, 0),
								gcnew Engine::Components::Vector3(0, 0, 0),
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

					if (ImGui::MenuItem("Dirrectional Light"))
					{
						auto meshRenderer = gcnew Engine::EngineObjects::LightSource(
							"Directional Light",
							gcnew Engine::Internal::Components::Transform(
								gcnew Engine::Components::Vector3(0, 0, 0),
								gcnew Engine::Components::Vector3(0, 0, 0),
								gcnew Engine::Components::Vector3(1, 1, 1),
								nullptr
							),
							0xFF0000FF,
							rPBR::LightType::LIGHT_DIRECTIONAL,
							gcnew Engine::Components::Vector3(1.0f, 1.0f, 1.0f),
							1.0f,
							1
						);
						meshRenderer->SetParent(lightManager);
						lightManager->AddLight(meshRenderer, 1);
						scene->AddObjectToScene(meshRenderer);
						scene->PushToRenderQueue(meshRenderer);
					}

					if (ImGui::MenuItem("Spot Light"))
					{
						auto meshRenderer = gcnew Engine::EngineObjects::LightSource(
							"Spot Light",
							gcnew Engine::Internal::Components::Transform(
								gcnew Engine::Components::Vector3(0, 0, 0),
								gcnew Engine::Components::Vector3(0, 0, 0),
								gcnew Engine::Components::Vector3(1, 1, 1),
								nullptr
							),
							0xFF0000FF,
							rPBR::LightType::LIGHT_SPOT,
							gcnew Engine::Components::Vector3(1.0f, 1.0f, 1.0f),
							1.0f,
							1
						);
						meshRenderer->SetParent(lightManager);
						lightManager->AddLight(meshRenderer, 1);
						scene->AddObjectToScene(meshRenderer);
						scene->PushToRenderQueue(meshRenderer);
					}

					ImGui::EndMenu();
				}

				ImGui::Separator();

				if (ImGui::BeginMenu("User Scripts"))
				{
					if (ImGui::MenuItem("Lua Script"))
					{
						Engine::EngineObjects::LuaScript^ luaScript = gcnew Engine::EngineObjects::LuaScript("LuaScript",
							gcnew Engine::Internal::Components::Transform(
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 0,0,0 }),
								Engine::Components::Vector3::create({ 1,1,1 }),
								scene->GetDatamodelMember("workspace")->GetTransform()
							));

						scene->AddObjectToScene(luaScript);
						scene->PushToRenderQueue(luaScript);
					}

					ImGui::Separator();

					for each (auto assembly in assemblies)
					{
						if (!assembly->getLoadedAssembly()->Equals(System::Reflection::Assembly::GetExecutingAssembly()))
						{
							for each (auto T in assembly->GetAssemblyTypes())
							{
								if (ImGui::MenuItem(CastToNative(T->Name)))
								{
									Engine::EngineObjects::ScriptBehaviour^ retn = assembly->Create<Engine::EngineObjects::ScriptBehaviour^>(T->FullName);
									scene->PushToRenderQueue(retn);
									scene->AddObjectToScene(retn);
								}
							}
						}
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
	}

	void DrawHierarchy()
	{
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
					if (reference->GetTransform()->parent != nullptr)
						continue;

					String^ refName = reference->name;
					refName += (reference->active == true) ? "" : " (INACTIVE)";

					if (type == ObjectType::Datamodel || type == ObjectType::LightManager || reference->isProtected())
					{
						if (ImGui::Selectable(CastToNative(refName + " (ENGINE PROTECTED)" + "###" + reference->GetTransform()->GetUID())))
						{
							if (reparentLock)
								reparentObject = reference;
							else if (selectionLock)
								selectionObject = reference;
							else
							{
								readonlyLock = true;
								selectedObject = reference;
								selectedObjectIndex = scene->GetRenderQueue()->IndexOf(selectedObject);
							}
						}
					}
					else if (reference->GetTransform()->parent == nullptr)
					{
						if (ImGui::Selectable(CastToNative(refName + " (UNPARENTED)" + "###" + reference->GetTransform()->GetUID())))
						{
							if (reparentLock)
								reparentObject = reference;
							else if (selectionLock)
								selectionObject = reference;
							else
							{
								readonlyLock = false;
								selectedObject = reference;
								selectedObjectIndex = scene->GetRenderQueue()->IndexOf(selectedObject);
							}
						}
					}

					DrawHierarchyInherits(scene, reference, 1);
				}
			}


			ImGui::End();
		}
	}

	void DrawProperties()
	{
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
					if (ImGui::BeginMenu("Instance Properties"))
					{
						if (ImGui::BeginMenu("Active"))
						{
							if (ImGui::MenuItem("Set Active"))
							{
								selectedObject->active = true;
							}

							if (ImGui::MenuItem("Set Inactive"))
							{
								selectedObject->active = false;
							}

							ImGui::EndMenu();
						}

						if (ImGui::BeginMenu("Rendering"))
						{
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
							}

							{
								std::string layer = "";

								if (selectedObject->layerMask != nullptr)
									layer = CastStringToNative(selectedObject->layerMask->layerName);
								else
									layer = "Select Layer";

								Layer^ l = nullptr;

								std::vector<std::string> layers = LayerManager::getLayerNames();

								if (ImGui::BeginCombo("###CURR_LAYER", layer.c_str(), ImGuiComboFlags_None))
								{
									for (std::string tmp : layers)
									{
										bool isSelected = false;

										String^ managedType = gcnew String(layer.data());
										String^ data = gcnew String(tmp.data());

										data = data->Substring(data->IndexOf("-") + 2);

										if (data->Equals(managedType))
										{
											isSelected = true;
										}

										if (ImGui::Selectable(tmp.c_str(), &isSelected))
										{
											data = gcnew String(tmp.c_str());

											String^ buffer = data->Substring(0, data->IndexOf(" -"));

											l = LayerManager::GetLayerFromId(int::Parse(buffer));

											if (l == nullptr)
											{
												printError("Failed getting layer (is null)");
												return;
											}

											printConsole("Swap layer from " + selectedObject->layerMask->layerName + " to " + l->layerName);

											if (l != nullptr)
												selectedObject->SetLayerMask(l);
										}

										if (isSelected)
											ImGui::SetItemDefaultFocus();
									}

									ImGui::EndCombo();
								}
							}

							ImGui::EndMenu();
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

					if (ImGui::BeginMenu("Instance Actions"))
					{
						if (ImGui::MenuItem("Change Parent"))
						{
							if (!readonlyLock)
								reparentLock = true;
						}

						ImGui::Separator();

						if (ImGui::MenuItem("Destroy Object"))
						{
							ObjectManager::singleton()->Destroy(selectedObject);
							selectedObject = nullptr;

							ImGui::EndMenu();

							ImGui::End();

							return;
						}

						ImGui::EndMenu();
					}

					if (ImGui::BeginMenu("Class"))
					{
						if (ImGui::MenuItem("Protect Member"))
						{
							selectedObject->protectMember();
						}

						if (ImGui::MenuItem("Unprotect Member"))
						{
							if (selectedObject->type == ObjectType::Datamodel || selectedObject->type == ObjectType::LightManager)
								return;

							selectedObject->unprotectMember();
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
					const char* opts[2] = { "World", "Local" };

					ImGui::Combo("###POSITION_SELECTOR", &positionSelector, opts, IM_ARRAYSIZE(opts));
					if (positionSelector == 0)
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
					}
					else
					{
						// local position
						float pos[3] = {
							selectedObject->GetTransform()->localPosition->x,
							selectedObject->GetTransform()->localPosition->y,
							selectedObject->GetTransform()->localPosition->z
						};

						if (ImGui::DragFloat3("Local Position", pos, 0.01f, float::MinValue, float::MaxValue, "%.3f", ImGuiInputTextFlags_CallbackCompletion) && !readonlyLock)
						{
							selectedObject->GetTransform()->UpdateLocalPosition(gcnew Engine::Components::Vector3(pos[0], pos[1], pos[2]));
						}
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
	}

	void DrawAssets()
	{
		if (ImGui::Begin("Assets", &isOpen, ImGuiWindowFlags_MenuBar))
		{
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Assets"))
				{
					if (ImGui::MenuItem("Pack Setup"))
					{
						packDataFileName = "";
						loadedAssets = gcnew System::Collections::Generic::List<String^>();
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

			const char* constData[] = { "ALL", "MODELS", "TEXTURES", "AUDIO" };
			ImGui::Text("Display assets: ");
			ImGui::SameLine();
			if (ImGui::Combo("###Display assets: ", &displayingAsset, constData, IM_ARRAYSIZE(constData)))
			{
				switch (displayingAsset)
				{
				case 0:
					displayingAssets = assetDisplay::ALL;
					break;
				case 1:
					displayingAssets = assetDisplay::MODELS;
					break;
				case 2:
					displayingAssets = assetDisplay::TEXTURES;
					break;
				case 3:
					displayingAssets = assetDisplay::AUDIO;
					break;
				}
			}

			ImGui::Separator();

			ImGui::BeginListBox("###ASSETS", { size.x - 20, size.y - 82 });

			createAssetEntries("./Data/");

			ImGui::EndListBox();


			ImGui::End();
		}
	}

	void DrawConsole()
	{
		if (ImGui::Begin("Console", &isOpen))
		{
			if (ImGui::Button("Clear"))
			{
				Engine::Scripting::Logging::clearLogs();
			}

			const ImVec2 windowSize = ImGui::GetWindowSize();

			if (ImGui::BeginListBox("###CONSOLE_OUTPUT", { windowSize.x - 20, windowSize.y - 80 }))
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

			ImGui::Text("Console Commands:");
			ImGui::SameLine();
			if (ImGui::InputText("###CONSOLE_COMMANDS", consoleBufferData, IM_ARRAYSIZE(consoleBufferData), ImGuiInputTextFlags_EnterReturnsTrue))
			{
				ThrowUIError("Console commands not implemented yet!");
			}

			ImGui::End();
		}
	}

#pragma endregion

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

		DrawMainMenuBar();

		DrawHierarchy();

		if (codeEditorOpen)
		{
			CodeEditor();
		}

		DrawAssets();

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
					if (!assembly->getLoadedAssembly()->Equals(System::Reflection::Assembly::GetExecutingAssembly()))
					{
						for each (auto T in assembly->GetAssemblyTypes())
						{
							if (ImGui::Button(CastToNative(T->Name)))
							{
								Engine::Internal::Components::Object^ retn = assembly->Create<Engine::Internal::Components::Object^>(T->FullName);
								scene->PushToRenderQueue(retn);
								scene->AddObjectToScene(retn);
							}
						}
					}
				}
			}

			ImGui::End();
		}

		DrawConsole();

		DrawProperties();

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
		else if (b5)
		{
			ImGui::OpenPopup("Pack Setup");
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
		else if (b9)
		{
			ImGui::OpenPopup("Engine Configuration");
		}

		if (visualizeError)
		{
			ImGui::OpenPopup("Unexpected Error");
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

		if (ImGui::BeginPopupModal("Pack Setup", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{
			char* buffer = new char[512];

			for (int x = 0; x < loadedAssets->Count; x++)
			{
				if (buffer != "")
					delete[] buffer;

				buffer = new char[512];
				strcpy(buffer, CastToNative(loadedAssets[x]));
				std::string name = "###" + std::string(CastToNative(loadedAssets[x])) + "###_index" + std::to_string(x);
				if (ImGui::InputText(name.c_str(), buffer, 512))
				{
					loadedAssets[x] = gcnew String(buffer);
				}
			}

			ImGui::Separator();
			{
				if (ImGui::Button("+"))
				{
					loadedAssets->Add("");
				}
				ImGui::SameLine();
				if (ImGui::Button("-"))
				{
					if (loadedAssets->Count >= 1)
					{
						loadedAssets->RemoveAt(loadedAssets->Count - 1);
					}
				}
			}
			ImGui::Separator();


			if (ImGui::Button("Pack all the files"))
			{
				OpenFileExplorer("Select output file", Engine::Editor::Gui::explorerMode::Save, gcnew Engine::Editor::Gui::onFileSelected(this, &EditorWindow::PackData));

				delete[] buffer;

				b5 = false;

				ImGui::CloseCurrentPopup();
			}
			ImGui::SameLine();
			if (ImGui::Button("Close"))
			{
				b5 = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		if (ImGui::BeginPopupModal("Engine Configuration", (bool*)false, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{
			char* buff = new char[8 * 512];

			auto config = Engine::Config::EngineConfiguration::singleton();

			strcpy(buff, CastStringToNative(config->windowName).c_str());

			ImGui::Text("Window Name: ");
			ImGui::SameLine();
			if (ImGui::InputText("##WINDOW_NAME", buff, config->windowName->Length + 512))
			{
				Engine::Config::EngineConfiguration::singleton()->windowName = gcnew String(buff);
			}




			if (ImGui::Button("Close"))
			{
				b9 = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

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

			delete[] tmp;

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
				//scene->LoadScene();
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
					delete[] data;

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
					if (scene->assetPacks->Count >= 1)
					{
						scene->assetPacks->RemoveAt(scene->assetPacks->Count - 1);
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


		ShowError();

	}

	void Exit() override
	{
		UnloadTexture(modelTexture);
		UnloadTexture(materialTexture);
		Engine::Utils::LogReporter::singleton->CloseThread();
		UnloadRenderTexture(viewportTexture);
		dataPack.FreeAll();
		exit(0);
	}

	void Draw() override
	{
		renderPipeline->ExecuteRenderWorkflow_Editor(this, scene, viewportTexture);
		/*
		BeginDrawing();
		{
			BeginTextureMode(viewportTexture);

			if (renderPipeline != nullptr)
				renderPipeline->OnRenderBegin();

			ClearBackground(GetColor(scene->skyColor));

			if (renderPipeline != nullptr)
				renderPipeline->PreRenderFrame();

			{
				Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

				if (camera == nullptr)
					return;

				bool is3DCamera = camera->is3DCamera();

				if (is3DCamera)
					BeginMode3D(((NativeCamera3D*)camera->get())->get());

				int currentLayer = 1;

				render(currentLayer, renderPipeline, scene);

				if (is3DCamera)
					EndMode3D();
			}

			if (renderPipeline != nullptr)
				renderPipeline->OnRenderEnd();

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
						obj->GetReference()->GameDrawImGUI();
					}
				}
			}

			DrawImGui();

			rlImGuiEnd();

		}
		EndDrawing();

		if (renderPipeline != nullptr)
			renderPipeline->PostRenderFrame();
			*/
	}

private:
	void create()
	{
		scene->GetDatamodelMember("workspace", true);
		scene->GetDatamodelMember("editor only", true);
		scene->GetDatamodelMember("gui", true);
		auto daemonParent = scene->GetDatamodelMember("daemons", true);


		if (!scene->ExistsMember("lighting"))
		{
			lightManager = gcnew LightManager("lighting",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					nullptr
				),
				"Data/Engine/Shaders/rPBR/pbr.vert",
				"Data/Engine/Shaders/rPBR/pbr.frag"
			);

			lightManager->protectMember();

			scene->PushToRenderQueue(lightManager);
			scene->AddObjectToScene(lightManager);
		}
		else
		{
			lightManager = scene->GetMember("lighting")->ToObjectType<LightManager^>();
			lightManager->protectMember();
		}

		if (ObjectManager::singleton()->GetChildrenOf(daemonParent)->Count <= 0)
		{
			auto lightdm = gcnew Engine::EngineObjects::Daemons::LightDaemon("lightdm",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					nullptr
				),
				lightManager
			);
			lightdm->SetParent(daemonParent);
			scene->PushToRenderQueue(lightdm);
			scene->AddObjectToScene(lightdm);
		}

		if (ObjectManager::singleton()->GetObjectsByName("EditorCamera")->Count <= 0)
		{
			auto camera3D = gcnew Engine::EngineObjects::Editor::EditorCamera("EditorCamera",
				gcnew Engine::Internal::Components::Transform(
					Engine::Components::Vector3::create({ 0,0,0 }),
					Engine::Components::Vector3::create({ 0,0,0 }),
					Engine::Components::Vector3::create({ 1,1,1 }),
					nullptr
				)
			);
			//camera3D->SetParent(Singleton<ObjectManager^>::Instance->GetDatamodel("workspace"));

			scene->PushToRenderQueue(camera3D);
			scene->PushToRenderQueue(camera3D);
		}
	}

public:
	void Init() override
	{
		modelTexture = LoadTexture("EditorAssets/Icons/Model.png");
		materialTexture = LoadTexture("EditorAssets/Icons/Material.png");

		cameraPosition = gcnew Engine::Components::Vector3(0, 0, 0);

		while (!scene->sceneLoaded())
			WaitTime(1.0);

		create();

		Logging::LogCustom("[GL Version]:", "Current OpenGL version is -> " + rlGetVersion() + ".");
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

		renderPipeline = gcnew Engine::Render::Pipelines::LitPBR_SRP();

		//scene = SceneManager::CreateScene("GoldEngineBoot");

		scene = SceneManager::LoadSceneFromFile("Level0", scene, passwd);

		while (!scene->sceneLoaded())
		{
			DataManager::HL_Wait(1.0f);
		}

		packedData = scene->getSceneDataPack();

		renderPipeline = Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance;

		// initialize editor assets

		// end of editor assets

		/*
		auto files = gcnew List<String^>();

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
		Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

		if (camera == nullptr)
			return;

		auto projectionMode = camera->cameraProjection;
		bool is3DCamera = (projectionMode == CameraProjection::CAMERA_PERSPECTIVE);

		void* cameraLocal = camera->get();

		if (showCursor)
			UpdateCamera(((NativeCamera3D*)cameraLocal)->getCameraPtr(), CAMERA_FREE);

		if (fpsCap)
		{
			SetFPS(60);
		}
		else
		{
			SetFPS(-1);
		}

		for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
		{
			if (obj->GetReference() != nullptr)
			{
				obj->GetReference()->GameUpdate();
			}
		}

		Engine::GC::EngineGC::Update();
	}
};



#pragma endregion

#else

#pragma region PLAYBACK ENGINE

public ref class GameWindow : public Engine::Window
{
private:
	bool initSettings = false;
	System::Collections::Generic::List<EngineAssembly^>^ assemblies;
	Scene^ scene;
	DataPack^ packedData;

public:
	GameWindow()
	{
#if HIDE_CONSOLE == true
		WinAPI::FreeCons();
#endif

		engine_bootstrap();

		assemblies = gcnew System::Collections::Generic::List<EngineAssembly^>();
		dataPack = DataPacks();

		assemblies->Add(gcnew EngineAssembly(System::Reflection::Assembly::GetExecutingAssembly()));
		assemblies->Add(gcnew EngineAssembly(System::Reflection::Assembly::GetCallingAssembly()));

		for each (String ^ fileName in Directory::GetFiles("Bin\\Asm\\"))
		{
			if (fileName->Contains(".goldasm") || fileName->Contains(".dll"))
			{
				String^ path = Directory::GetCurrentDirectory() + "\\" + fileName;

				assemblies->Add(gcnew EngineAssembly(path));
			}
		}

		SceneManager::SetAssemblyManager(assemblies);

		for each (EngineAssembly ^ assembly in assemblies)
		{
			assembly->ListAssemblyTypes();
		}

		Start();
	}

	virtual void Start() override
	{
		auto secrets = Engine::Config::EngineSecrets::ImportSecrets("./Data/Keys/secrets.dat");

		passwd = CypherLib::GetPasswordBytes(secrets->encryptionPassword);

		auto config = Engine::Config::EngineConfiguration::ImportConfig("./Data/appinfo.dat");

		SetWindowFlags(config->windowFlags);
		OpenWindow(config->resolution->w, config->resolution->h, config->getWindowName().c_str());

		gcnew Engine::Managers::SignalManager();

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

private:
	void create()
	{
		LightManager^ lightManager = nullptr;

		scene->GetDatamodelMember("workspace", true);

		if (!scene->ExistsMember("lighting"))
		{
			lightManager = gcnew LightManager("lighting",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					nullptr
				),
				"Data/Engine/Shaders/rPBR/pbr.vert",
				"Data/Engine/Shaders/rPBR/pbr.frag"
			);

			lightManager->protectMember();

			scene->PushToRenderQueue(lightManager);
			scene->AddObjectToScene(lightManager);
		}
		else
		{
			lightManager = scene->GetMember("lighting")->ToObjectType<LightManager^>();
			lightManager->protectMember();
		}


		scene->GetDatamodelMember("editor only");
		scene->GetDatamodelMember("gui");
		auto daemonParent = scene->GetDatamodelMember("daemons");

		if (ObjectManager::singleton()->GetChildrenOf(daemonParent)->Count <= 0)
		{
			auto lightdm = gcnew Engine::EngineObjects::Daemons::LightDaemon("lightdm",
				gcnew Engine::Internal::Components::Transform(
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					gcnew Engine::Components::Vector3(0, 0, 0),
					nullptr
				),
				lightManager
			);
			lightdm->SetParent(daemonParent);
			scene->PushToRenderQueue(lightdm);
			scene->AddObjectToScene(lightdm);
		}
	}

public:
	void Init() override
	{
		while (!scene->sceneLoaded())
			WaitTime(1.0);

		//Engine::Assets::Management::DataPack::SetSingletonReference(packedData);
		create();

		Logging::LogCustom("[GL Version]:", "Current OpenGL version is -> " + rlGetVersion() + ".");
	}

	void Preload() override
	{
		//scene = SceneManager::CreateScene("GoldEngineBoot");

		gcnew Engine::Render::Pipelines::LitPBR_SRP();

		scene = SceneManager::LoadSceneFromFile("Level0", scene, passwd);
		//scene->LoadScene();

		while (!scene->sceneLoaded())
		{
			DataManager::HL_Wait(1.0f);
		}

		packedData = scene->getSceneDataPack();

		Init();
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
		ImGui::DockSpaceOverViewport(viewPort, ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode);
	}

	virtual void Draw() override
	{

		Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance->ExecuteRenderWorkflow(this, scene);
		/*
		if (Singleton<Engine::Render::ScriptableRenderPipeline^>::Instantiated)
			Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance->PreFirstPassRender(scene);

		BeginDrawing();
		{
			if (Singleton<Engine::Render::ScriptableRenderPipeline^>::Instantiated)
				Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance->PreRenderFrame();

			ClearBackground(GetColor(scene->skyColor));

			Engine::EngineObjects::Camera^ camera = ObjectManager::singleton()->GetMainCamera();

			auto projectionMode = camera->cameraProjection;

			bool is3DCamera = (projectionMode == CameraProjection::CAMERA_PERSPECTIVE);

			if (camera == nullptr)
				return;

			if (is3DCamera)
				BeginMode3D(((Engine::EngineObjects::Native::NativeCamera3D*)camera->get())->get());
			else
				BeginMode3D(((Engine::EngineObjects::Native::NativeCamera3D*)camera->get())->get());

			int currentLayer = 1;

			render(currentLayer, Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance, scene);

			if (is3DCamera)
				EndMode3D();
			else
				EndMode3D();

			DrawFPS(0, 0);

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
						obj->GetReference()->GameDrawImGUI();
					}
				}
			}

			DrawImGui();

			rlImGuiEnd();

		}
		EndDrawing();

		if (Singleton<Engine::Render::ScriptableRenderPipeline^>::Instantiated)
			Singleton<Engine::Render::ScriptableRenderPipeline^>::Instance->PostRenderFrame();
		*/
	}

	virtual void Update() override
	{
		for each (Engine::Management::MiddleLevel::SceneObject ^ obj in scene->GetRenderQueue())
		{
			if (obj->GetReference() != nullptr)
			{
				obj->GetReference()->GameUpdate();
			}
		}

		Engine::GC::EngineGC::Update();
	}

	virtual void Exit() override
	{
		DataManager::HL_FreeAll();
		Engine::Utils::LogReporter::singleton->CloseThread();
	}
};

#pragma endregion

#endif

int main(array<System::String^>^ args)
{
	passwd = CypherLib::GetPasswordBytes(gcnew String(ENCRYPTION_PASSWORD));

#if PRODUCTION_BUILD
	gcnew GameWindow();
#else
	gcnew EditorWindow();
#endif
}
