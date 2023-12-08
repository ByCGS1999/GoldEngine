#pragma once

namespace Engine::EngineObjects
{
	namespace Native
	{
		class NativeSkybox
		{
		public:
			std::unique_ptr<Model> model;
			NativeSkybox(Model newMod)
			{
				model = std::make_unique<Model>(newMod);
			}
		};
	}

	public ref class Skybox : Engine::Internal::Components::Object
	{
	private:
		Native::NativeSkybox* nativeSkybox;

	public:
		int materialId;
		Skybox(String^ s, Engine::Internal::Components::Transform^ t, int matId);
		void Draw() override;
		void Update() override {}
		void PhysicsUpdate() override {}
		void Start() override;
		void SetupSkyboxImage(unsigned int shaderId, Texture2D texture);
		void SwapMaterial(unsigned int materialId);
	};
}
