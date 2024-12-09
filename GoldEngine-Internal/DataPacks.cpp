#include "DataPacks.h"

using namespace Engine::Assets::Storage;

NativeDataPack::NativeDataPack() 
{
	shaders = std::vector<Engine::Assets::Storage::Types::ShaderPack>();
	models = std::vector<Engine::Assets::Storage::Types::ModelPack>();
	cameras = std::vector<Engine::Assets::Storage::Types::CameraPack>();
	materials = std::vector<Engine::Assets::Storage::Types::MaterialPack>();
	textures2d = std::vector<Engine::Assets::Storage::Types::Texture2DPack>();
	sounds = std::vector<Engine::Assets::Storage::Types::SoundPack>();
	musics = std::vector<Engine::Assets::Storage::Types::MusicPack>();
}

static DataPacks* instance;

DataPacks::DataPacks()
{
	nativePacks = new NativeDataPack();
	instance = this;
}

DataPacks DataPacks::singleton()
{
	return *instance;
}

NativeDataPack* DataPacks::GetNativeDataPack()
{
	return nativePacks;
}

void DataPacks::AddSound(unsigned int soundId, Sound sound)
{
	bool hasShader = false;

	for (int x = 0; x < nativePacks->sounds.size(); x++)
	{
		auto sP = &nativePacks->sounds[x];

		if (sP->getId() == soundId)
		{
			sP->setResource(sound); // overwrite shader reference
			hasShader = true;
			break;
		}
		else
		{
			hasShader = false;
		}
	}

	if (!hasShader)
	{
		nativePacks->sounds.push_back(Engine::Assets::Storage::Types::SoundPack(soundId, sound));
	}
}

Sound DataPacks::GetSound(unsigned int soundId)
{
	Sound sound;

	for (int x = 0; x < nativePacks->sounds.size(); x++)
	{
		auto sP = &nativePacks->sounds[x];

		if (sP->getId() == soundId)
		{
			sound = sP->getResource();
			break;
		}
	}

	return sound;
}

Sound* DataPacks::GetSoundPtr(unsigned int soundId)
{
	Sound* soundPtr;

	for (int x = 0; x < nativePacks->sounds.size(); x++)
	{
		auto sP = &nativePacks->sounds[x];

		if (sP->getId() == soundId)
		{
			soundPtr = sP->getResourcePtr();
			break;
		}
	}

	return soundPtr;
}


Music DataPacks::GetMusic(unsigned int musicId)
{
	Music sound;

	for (int x = 0; x < nativePacks->musics.size(); x++)
	{
		auto sP = &nativePacks->musics[x];

		if (sP->getId() == musicId)
		{
			sound = sP->getResource();
			break;
		}
	}

	return sound;
}

Music* DataPacks::GetMusicPtr(unsigned int musicId)
{
	Music* soundPtr;

	for (int x = 0; x < nativePacks->musics.size(); x++)
	{
		auto sP = &nativePacks->musics[x];

		if (sP->getId() == musicId)
		{
			soundPtr = sP->getResourcePtr();
			break;
		}
	}

	return soundPtr;
}

void DataPacks::AddMusic(unsigned int soundId, Music sound)
{
	bool hasShader = false;

	for (int x = 0; x < nativePacks->musics.size(); x++)
	{
		auto sP = &nativePacks->musics[x];

		if (sP->getId() == soundId)
		{
			sP->setResource(sound); // overwrite shader reference
			hasShader = true;
			break;
		}
		else
		{
			hasShader = false;
		}
	}

	if (!hasShader)
	{
		nativePacks->musics.push_back(Engine::Assets::Storage::Types::MusicPack(soundId, sound));
	}
}