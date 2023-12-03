#include "SceneManager.h"

using namespace System;
using namespace System::IO;

const char* password = "GoldEngine_DefaultPassword";
int xorPasswd;

bool AssetExists(System::String^ fN)
{
	return File::Exists("Data/" + fN + ".scn");
}

void SceneManager::LoadSceneFromFile(String^ fN)
{
	if (!String::IsNullOrEmpty(fN))
	{
		// find file into data files.
		if (AssetExists(fN))
		{
			// decrypt scene and parse it
		}
	}
}