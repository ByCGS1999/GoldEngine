#pragma once

#include <iostream>

// TODO: fix the entire datapack system due many memory leaks and non performant allocation. SAKE.
// TODO(pt2): Find out why datapacks are not accepting this new struct

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
			Shader* ptr = resource.release();
			delete ptr;
			resource = std::make_unique<Shader>(s);
		}

		void freealloc()
		{
			UnloadShader(*resource);
			Shader* resPtr = resource.release();
			delete resPtr;
		}
	};
}