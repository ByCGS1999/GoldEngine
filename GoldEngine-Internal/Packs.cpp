#define GLIMPL

#include "GlIncludes.h"
#include "Packs.h"

using namespace Engine::Assets::Storage::Types;

// SHADER PACK

ShaderPack::ShaderPack(unsigned int id, Shader& res)
{
	this->id = id;
	this->resource = res;
}

unsigned int ShaderPack::getId() { return id; }
Shader& ShaderPack::getResource() { return resource; }
Shader* ShaderPack::getResourcePtr() { return &resource; }

void ShaderPack::setResource(Shader& s)
{
	resource = s;
}

void ShaderPack::freealloc()
{
	UnloadShader(resource);
}

// MODEL PACK

ModelPack::ModelPack(unsigned int id, Model& res)
{
	this->id = id;
	this->resource = std::make_unique<Model>(res);
}

bool ModelPack::hasValue() { return resource != nullptr; }
unsigned int ModelPack::getId() const { return id; }
Model& ModelPack::getResource() const { return *resource; }

void ModelPack::setResource(Model& s)
{
	resource.reset(new Model);
	resource = std::make_unique<Model>(s);
}

void ModelPack::freealloc()
{
	UnloadModel(*resource);
	Model* resPtr = resource.release();
	delete resPtr;
}

// TEXTURE2DPACK

Texture2DPack::Texture2DPack(unsigned int id, Texture2D& ref)
{
	this->id = id;
	this->resource = std::make_unique<Texture>(ref);
}

bool Texture2DPack::hasValue() { return resource != nullptr; }
unsigned int Texture2DPack::getId() const { return id; }

Texture& Texture2DPack::getResource() const 
{
	return *resource;
}

void Texture2DPack::setResource(Texture& s)
{
	resource = std::make_unique<Texture>(s);
}

void Texture2DPack::freealloc()
{
	UnloadTexture(*resource);
	Texture* resPtr = resource.release();
	delete resPtr;
}