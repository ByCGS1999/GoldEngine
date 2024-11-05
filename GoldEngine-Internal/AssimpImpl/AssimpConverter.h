#pragma managed(push, off)
#pragma once

#include <assimp/scene.h>

#define SETTER(T, ARG) void set##ARG(const T&);
#define SETTER_FUNC(T, ARG) void FBX2GLTF::set##ARG(const T& value) \
	{ \
		this->##ARG = value; \
	}

extern std::string utilTool;

int getAssimpExporters();
const char* getAssimpExporterDescription(int);
const char* getAssimpExporterId(int a1);

public class AssimpConverter
{
private:
	std::string FileName;
	RAYLIB::Mesh* temporalMesh;
	const aiScene* scene;

	aiScene objectScene;

public:
	AssimpConverter(std::string, std::string, std::string);
	AssimpConverter(std::string, unsigned int, std::string, std::string);

	void CreateMesh(unsigned int, std::string, std::string);

	RAYLIB::Mesh* GetMeshes();
	unsigned int GetMeshCount();

	void dealloc();
};

#pragma managed(pop)