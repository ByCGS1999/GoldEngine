#pragma once

namespace Engine::Managers
{
	public ref class SceneManager
	{
	public:
		static Engine::Management::Scene^ LoadSceneFromFile(System::String^ fN);
		static Engine::Management::Scene^ CreateScene();
		static void SaveSceneToFile(Engine::Management::Scene^ scene, unsigned int password);
	};
}