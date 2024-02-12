#pragma once

namespace Engine::Assets::Storage::Types
{
	public class Texture2DPack
	{
	public:
		unsigned int textureId;

	public:
		Texture2D textureReference;

	public:
		Texture2DPack(unsigned int id, Texture2D ref)
		{
			textureId = id;
			textureReference = ref;
		}
	};
}