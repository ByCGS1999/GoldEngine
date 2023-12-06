#include "Object.h"
#include "Transform.h"
#include "ModelRenderer.h"

using namespace System;
using namespace Engine::EngineObjects;

Engine::EngineObjects::ModelRenderer::ModelRenderer(String^ n, Engine::Internal::Components::Transform^ t) : Engine::Internal::Components::Object(n, t, Engine::Internal::Components::ModelRenderer)
{
	Material mat;
	mat.shader = LoadShader("data/Engine/Shaders/base.vs", "data/Engine/Shaders/base.fs");
	nativeRenderer = new Native::NativeModelRenderer(LoadModel("data/Engine/Models/Error.obj"), mat, 0xFF0000);
}

void ModelRenderer::SetNativeRenderer(Native::NativeModelRenderer* renderer)
{
	if(nativeRenderer != nullptr)
		free(nativeRenderer);
	
	nativeRenderer = renderer;
}

void ModelRenderer::Update()
{

}

void ModelRenderer::Start()
{

}

void ModelRenderer::PhysicsUpdate()
{

}

void ModelRenderer::Draw()
{
	Engine::Internal::Components::Vector3^ internal_v3 = transform->position;

	DrawModel(*nativeRenderer->model, {internal_v3->x, internal_v3->y, internal_v3->z}, transform->scale, GetColor(nativeRenderer->color_hex));
}

void ModelRenderer::SetColorTint(unsigned int hexValue)
{
	nativeRenderer->color_hex = hexValue;
}