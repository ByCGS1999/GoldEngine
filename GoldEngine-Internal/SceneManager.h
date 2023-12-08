#pragma once

namespace Engine::Managers
{
	public ref class SceneManager
	{
	private:
		static bool AssetExists(System::String^ fN);
	public:
		static Engine::Management::Scene^ LoadSceneFromFile(System::String^ fN);
		static Engine::Management::Scene^ CreateScene();
		static void SaveSceneToFile(Engine::Management::Scene^ scene, unsigned int password);
	};
}