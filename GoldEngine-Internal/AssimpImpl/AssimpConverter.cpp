#include <string>
#include <filesystem>
#include "../Macros.h"
#include "../Includes.h"
#include "../GlIncludes.h"
#include "../CastToNative.h"
#include "../LoggingAPI.h"
#include <assimp/BaseImporter.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/importerdesc.h>
#include <assimp/Exporter.hpp>
#include <assimp/cexport.h>
#include "AssimpConverter.h"

#pragma managed(push, off)

AssimpConverter::AssimpConverter(std::string fileName, std::string outputFile, std::string format)
{
	this->FileName = fileName;
	this->scene = nullptr;

	this->CreateMesh(aiProcess_Triangulate, outputFile, format);
}

AssimpConverter::AssimpConverter(std::string fileName, unsigned int flags, std::string outputFile, std::string format)
{
	this->FileName = fileName;
	this->scene = nullptr;

	this->CreateMesh(flags, outputFile, format);
}

void SetMeshData(RAYLIB::Mesh& rlMesh, aiMesh* ai_mesh, std::string outputPath)
{
	rlMesh.vertexCount = ai_mesh->mNumVertices;
	
	rlMesh.vertices = (float*)malloc(rlMesh.vertexCount * 3 * sizeof(float));

	for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
	{
		rlMesh.vertices[i * 3 + 0] = ai_mesh->mVertices[i].x;
		rlMesh.vertices[i * 3 + 1] = ai_mesh->mVertices[i].y;
		rlMesh.vertices[i * 3 + 2] = ai_mesh->mVertices[i].z;
	}

	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
		aiFace face = ai_mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	rlMesh.indices = (unsigned short*)malloc(indices.size() * sizeof(unsigned short));
	for (size_t i = 0; i < indices.size(); i++) {
		rlMesh.indices[i] = indices[i];
	}
}

void AssimpConverter::CreateMesh(unsigned int flags, std::string outputPath, std::string format)
{
	Assimp::Importer impl;
	Assimp::Exporter exporter;
	this->scene = impl.ReadFile(this->FileName, flags);
	
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		printf("Error loading model: %s\n", impl.GetErrorString());

	if (!scene->HasMeshes())
		printf("The loaded scene has no meshes");

	for (int x = 0; x < scene->mNumMaterials; x++)
	{
		scene->mMaterials[x]->Clear();
	}

	if (exporter.Export(scene, format, outputPath) != AI_SUCCESS)
	{
		printf("Error exporting model: %s", exporter.GetErrorString());
	}
	else
	{
		printf("Model Exported!");
	}
}

RAYLIB::Mesh* AssimpConverter::GetMeshes()
{
	return this->temporalMesh;
}

unsigned int AssimpConverter::GetMeshCount()
{
	return (sizeof(this->temporalMesh) / sizeof(RAYLIB::Mesh));
}

void AssimpConverter::dealloc()
{
	delete this->temporalMesh;
	delete this;
}

int getAssimpExporters()
{
	return Assimp::Exporter().GetExportFormatCount();
}

const char* getAssimpExporterDescription(int a1)
{
	return Assimp::Exporter().GetExportFormatDescription(a1)->description;
}

const char* getAssimpExporterId(int a1)
{
	return Assimp::Exporter().GetExportFormatDescription(a1)->id;
}

#pragma managed(pop)