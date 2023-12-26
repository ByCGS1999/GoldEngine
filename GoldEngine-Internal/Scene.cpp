#include "Object.h"
#include "DataPacks.h"
#include "Transform.h"
#include "Scene.h"
#include "SceneObject.h"

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
		object));
}

void Scene::LoadScene()
{
	OnLoad();
}

void Scene::UnloadScene()
{
	OnUnload();
	sceneObjects->Clear();
	DataPacks::singleton().FreeAll();
	System::GC::Collect();
}