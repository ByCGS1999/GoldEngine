#include "Scene.h"
#include "SceneManager.h"
#include "CypherLib.h"

using namespace Engine::Management;
using namespace System;
using namespace System::IO;

bool Engine::Managers::SceneManager::AssetExists(System::String^ fN)
{
	return File::Exists("Data/" + fN + ".scn");
}

Scene^ Engine::Managers::SceneManager::LoadSceneFromFile(String^ fN)
{
	if (AssetExists(fN))
	{
		return Newtonsoft::Json::JsonConvert::DeserializeObject<Scene^>(File::ReadAllText("Data/" + fN + ".scn"));
	}

	return nullptr;
}

Scene^ Engine::Managers::SceneManager::CreateScene()
{
	return gcnew Scene();
}

void Engine::Managers::SceneManager::SaveSceneToFile(Engine::Management::Scene^ scene, unsigned int password) 
{
	if (AssetExists(scene->sceneName))
	{
		String^ serializedData = Newtonsoft::Json::JsonConvert::SerializeObject(scene, Newtonsoft::Json::Formatting::Indented);
		String^ cipheredContents = CypherLib::EncryptFileContents(serializedData, password);

		//System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", System::Convert::ToBase64String(Encoding::UTF8->GetBytes(cipheredContents)));
		System::IO::File::WriteAllText("Data/" + scene->sceneName + ".scn", serializedData);
	}
	else
	{ 
		// create file and rerun save
		System::IO::File::Create("Data/" + scene->sceneName + ".scn")->Close();
		SaveSceneToFile(scene, password);
	}
}

void Engine::Managers::SceneManager::UnloadScene(Scene^ loadedScene)
{
	loadedScene->UnloadScene();
}