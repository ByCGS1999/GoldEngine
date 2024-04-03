#pragma once

#include <iostream>

namespace Engine::Assets::Storage::Types
{
	public struct ShaderPack
	{
	private:
		unsigned int id;
		std::unique_ptr<Shader> resource;

	public:
		ShaderPack(unsigned int id, Shader res)
		{
			this->id = id;
			this->resource = std::make_unique<Shader>(res);
		}

	public:
		unsigned int getId() const { return id; }
		Shader getResource() const { return *resource; }

		void setResource(Shader s)
		{
			resource = std::make_unique<Shader>(s);
		}

		void freealloc()
		{
			UnloadShader(*resource);
			Shader* resPtr = resource.release();
			delete resPtr;
		}
	};
	
	public struct ModelPack
	{
	private:
		unsigned int id;
		std::unique_ptr<Model> resource;

	public:
		ModelPack(unsigned int id, Model res)
		{
			this->id = id;
			this->resource = std::make_unique<Model>(res);
		}

	public:
		bool hasValue() { return resource != nullptr; }
		unsigned int getId() const { return id; }
		Model getResource() const { return *resource; }

		void setResource(Model s)
		{
			resource = std::make_unique<Model>(s);
		}

		void freealloc()
		{
			try 
			{
				Model* resPtr = resource.release();
				delete resPtr;
			}
			catch (std::exception ex)
			{
				printf("Failed freeing resource.");
				printf(ex.what());
			}
		}
	};
}