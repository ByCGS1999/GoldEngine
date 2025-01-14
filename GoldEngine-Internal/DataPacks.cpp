#include <utility>
#include "DataPacks.h"

using namespace Engine::Assets::Storage;

void onUnloadSound(RAYLIB::Sound s)
{
	UnloadSound(s);
}

void onUnloadMusicStream(RAYLIB::Music m)
{
	UnloadMusicStream(m);
}

void onUnloadModel(RAYLIB::Model m)
{
	UnloadModel(m);
}

void onUnloadShader(RAYLIB::Shader m) 
{
	UnloadShader(m);
}

void onUnloadTexture(RAYLIB::Texture2D tex)
{
	UnloadTexture(tex);
}

NativeDataPack::NativeDataPack() 
{
	shaders = std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Shader>>();
	models = std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Model>>();
	textures2d = std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Texture2D>>();
	sounds = std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Sound>>();
	musics = std::map<unsigned int, Engine::Native::EnginePtr<RAYLIB::Music>>();
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
	auto sP = &nativePacks->sounds[soundId];

	if (sP == nullptr)
	{
		nativePacks->sounds[soundId] = Engine::Native::EnginePtr<RAYLIB::Sound>(sound, &onUnloadSound, &onUnloadSound);
	}
	else
	{
		nativePacks->sounds[soundId].setInstance(sound);
	}
}

Sound DataPacks::GetSound(unsigned int soundId)
{
	Sound soundPtr;

	auto sP = &nativePacks->sounds[soundId];
	if (sP != nullptr)
	{
		soundPtr = sP->getInstance();
	}

	return soundPtr;
}

Sound* DataPacks::GetSoundPtr(unsigned int soundId)
{
	Sound* soundPtr = nullptr;

	auto sP = &nativePacks->sounds[soundId];
	if (sP != nullptr)
	{
		soundPtr = &sP->getInstance();
	}

	return soundPtr;
}


Music DataPacks::GetMusic(unsigned int musicId)
{
	Music soundPtr;

	auto sP = &nativePacks->musics[musicId];
	if (sP != nullptr)
	{
		soundPtr = sP->getInstance();
	}

	return soundPtr;
}

Music* DataPacks::GetMusicPtr(unsigned int musicId)
{
	Music* soundPtr = nullptr;

	auto sP = &nativePacks->musics[musicId];
	if (sP != nullptr)
	{
		soundPtr = &sP->getInstance();
	}

	return soundPtr;
}

void DataPacks::AddMusic(unsigned int soundId, Music sound)
{
	auto sP = &nativePacks->musics[soundId];

	if (sP == nullptr)
	{
		nativePacks->musics[soundId] = Engine::Native::EnginePtr<RAYLIB::Music>(sound, &onUnloadMusicStream, &onUnloadMusicStream);
	}
	else
	{
		nativePacks->musics[soundId].setInstance(sound);
	}
}


void DataPacks::AddModel(unsigned int modelId, Model modelRef)
{
	auto sP = &nativePacks->models[modelId];

	if (sP == nullptr)
	{
		nativePacks->models[modelId] = Engine::Native::EnginePtr<RAYLIB::Model>(modelRef, &onUnloadModel, &onUnloadModel);
	}
	else
	{
		nativePacks->models[modelId].setInstance(modelRef);
	}
}

void DataPacks::AddShader(unsigned int shaderId, Shader& shader)
{
	auto sP = &nativePacks->shaders[shaderId];

	if (sP == nullptr)
	{
		nativePacks->shaders[shaderId] = Engine::Native::EnginePtr<RAYLIB::Shader>(shader, &onUnloadShader, &onUnloadShader);
	}
	else
	{
		nativePacks->shaders[shaderId].setInstance(shader);
	}
}

void DataPacks::AddTexture2D(unsigned int textureId, Texture2D texture)
{
	auto sP = &nativePacks->textures2d[textureId];

	if (sP == nullptr)
	{
		nativePacks->textures2d[textureId] = Engine::Native::EnginePtr<RAYLIB::Texture2D>(texture, &onUnloadTexture, &onUnloadTexture);
	}
	else
	{
		sP->setInstance(texture);
	}
}