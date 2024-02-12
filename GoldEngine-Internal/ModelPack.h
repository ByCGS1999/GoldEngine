#pragma once

namespace Engine::Assets::Storage::Types
{
	public class ModelPack
	{
	public:
		unsigned int ModelId;

	public:
		Model ModelReference;

	public:
		ModelPack(unsigned int id, Model ref)
		{
			ModelId = id;
			ModelReference = ref;
		}
	};
}