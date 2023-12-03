#include "Scene.h"
#include "Object.h"

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

// SCENE UNLOADING
 
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