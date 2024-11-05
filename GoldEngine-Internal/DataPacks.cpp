#include "DataPacks.h"

using namespace Engine::Assets::Storage;

NativeDataPack::NativeDataPack() 
{
	shaders = std::vector<Engine::Assets::Storage::Types::ShaderPack>();
	models = std::vector<Engine::Assets::Storage::Types::ModelPack>();
	cameras = std::vector<Engine::Assets::Storage::Types::CameraPack>();
	materials = std::vector<Engine::Assets::Storage::Types::MaterialPack>();
	textures2d = std::vector<Engine::Assets::Storage::Types::Texture2DPack>();
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