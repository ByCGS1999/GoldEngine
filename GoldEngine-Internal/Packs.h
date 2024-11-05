#pragma once

#include <iostream>

namespace Engine::Assets::Storage::Types
{
	struct ShaderPack
	{
	private:
		unsigned int id;
		Shader resource;

	public:
		ShaderPack(unsigned int id, Shader& res);

	public:
		unsigned int getId();
		Shader& getResource();
		Shader* getResourcePtr();

		void setResource(Shader& s);

		void freealloc();
	};
	
	struct ModelPack
	{
	private:
		unsigned int id;
		std::unique_ptr<Model> resource;

	public:
		ModelPack(unsigned int id, Model& res);

	public:
		bool hasValue();
		unsigned int getId() const;
		Model& getResource() const;

		void setResource(Model& s);

		void freealloc();
	};


	struct Texture2DPack
	{
	private:
		unsigned int id;
		std::unique_ptr<Texture> resource;

	public:
		Texture2DPack(unsigned int id, Texture2D& ref);

	public:
		bool hasValue();
		unsigned int getId() const;
		Texture& getResource() const;

		void setResource(Texture& s);

		void freealloc();
	};
}