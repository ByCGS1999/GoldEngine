#include "Object.h"
#include "Includes.h"
#include "Transform.h"
#include <msclr/auto_gcroot.h>
#include "Skybox.h"

using namespace Engine::EngineObjects;

Skybox::Skybox(String^ s, Engine::Internal::Components::Transform^ t) : Object(s,t, Engine::Internal::Components::Skybox)
{
	nativeSkybox = new Native::NativeSkybox(LoadModel("Data/Engine/Models/Skybox.glb"));

	if(nativeSkybox->model->materialCount > 0)
	{
		Shader s = LoadShader("data/Engine/Shaders/base.vs", "data/Engine/Shaders/base.fs");
		Material newMaterial;

		newMaterial.shader = s;

		nativeSkybox->model->materials[0] = newMaterial;
	}
}

void Skybox::Start()
{

}

void Skybox::Draw()
{
	Engine::Internal::Components::Transform^ t = GetTransform();

	DrawModel(*nativeSkybox->model, { t->position->x,t->position->y, t->position->z }, t->scale, GetColor(0x2FFF2F));
}

void Skybox::SetupSkyboxImage(int index, Texture2D texture)
{

}