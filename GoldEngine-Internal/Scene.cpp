#include "Object.h"
#include "Transform.h"
#include "Scene.h"

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
 
void Scene::AddObjectToScene(System::Object^ object)
{
	sceneObjects.Add(object);
}

void Scene::LoadScene()
{
	OnLoad();
}

void Scene::UnloadScene()
{
	OnUnload();
	sceneObjects.Clear();
	System::GC::Collect();
}