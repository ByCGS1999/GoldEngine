#include "Scene.h"
#include "SceneManager.h"
#include "CypherLib.h"

using namespace Engine::Management;
using namespace System;
using namespace System::IO;

bool AssetExists(System::String^ fN)
{
	return File::Exists("Data/" + fN + ".scn");
}

Scene^ Engine::Managers::SceneManager::LoadSceneFromFile(String^ fN)
{
	if (!String::IsNullOrEmpty(fN))
	{
		// find file into data files.
		if (AssetExists(fN))
		{
			// decrypt scene and parse it
		}
	}

	return gcnew Scene();
}


Scene^ Engine::Managers::SceneManager::CreateScene()
{
	return gcnew Scene();
}

void Engine::Managers::SceneManager::SaveSceneToFile(Engine::Management::Scene^ scene, unsigned int password) 
{
	if (AssetExists(scene->sceneName))
	{
		String^ serializedData = Newtonsoft::Json::JsonConvert::SerializeObject(scene);
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