#include "Object.h"
#include "DataPacks.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneObject.h"
#include "WinAPI.h"
#include "FileManager.h"
#include "DataPack.h"

using namespace Engine::Management;

void Scene::Draw()
{

}

void Scene::Preload()
{

}

void Scene::PhysicsUpdate()
{

}

void Scene::Update()
{

}

void Scene::OnUnload()
{

}

void Scene::OnLoad()
{

}

// METHODS
 
void Scene::AddObjectToScene(Engine::Internal::Components::Object^ object)
{
	sceneObjects->Add(gcnew ::MiddleLevel::SceneObject(
		object->type,
		object,
		""));
}

void Scene::LoadScene()
{
	DataPack^ pack = gcnew DataPack();
	DataPacks::singleton().FreeAll(); // free all the assets

	for each (auto packRoute in assetPacks)
	{
		FileManager::ReadCustomFileFormat(packRoute, "ThreadcallNull");
	}

	pack->ReadFromFile(sceneRequirements, 1234);

	Directory::Delete("Data/tmp/", true);

	OnLoad();
}

void Scene::UnloadScene()
{
	//OnUnload();
	sceneObjects->Clear();
	drawQueue->Clear();
	DataPacks::singleton().FreeAll();
	System::GC::Collect();
}