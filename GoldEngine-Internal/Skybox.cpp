#include "Object.h"
#include "Includes.h"
#include "Transform.h"
#include "DataPacks.h"
#include "Skybox.h"

using namespace Engine::EngineObjects;

Skybox::Skybox(String^ s, Engine::Internal::Components::Transform^ t, int matId) : Object(s,t, Engine::Internal::Components::Skybox)
{
	materialId = matId;

	nativeSkybox = new Native::NativeSkybox(LoadModel("Data/tmp/Engine/Models/Skybox_cube.glb"));
}

void Skybox::Start() 
{

}

void Skybox::Draw()
{
	Engine::Internal::Components::Transform^ t = GetTransform();

	DrawModel(*nativeSkybox->model, { t->position->x,t->position->y, t->position->z }, t->scale, GetColor(0xFFFFFFFF));
}

void Skybox::SetupSkyboxImage(unsigned int shaderId, Texture2D texture)
{
	nativeSkybox->model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texture;
}

void Skybox::SwapMaterial(unsigned int materialId)
{
	nativeSkybox->model->materials[0] = DataPacks::singleton().GetMaterial(0);
}