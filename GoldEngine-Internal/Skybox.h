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
		unsigned int model;
		unsigned int material;
		unsigned int texture;

		Skybox(String^ s, Engine::Internal::Components::Transform^ t, unsigned int modelId, unsigned int matId, unsigned int texturePath) : Object(s, t, Engine::Internal::Components::Skybox)
		{
			material = matId;
			texture = texturePath;
			model = modelId;

			nativeSkybox = new Native::NativeSkybox(DataPacks::singleton().GetModel(model));
		}

		void Init(unsigned int modelId, unsigned int matId, unsigned int texturePath) override
		{
			material = matId;
			texture = texturePath;
			model = modelId;

			nativeSkybox = new Native::NativeSkybox(DataPacks::singleton().GetModel(model));
		}

		void Draw() override
		{
			auto t = GetTransform();
			DrawModelEx(*nativeSkybox->model, t->position->toNative(), t->rotation->toNative(), t->rotationValue , t->scale->toNative(), GetColor(0xFFFFFFFF));
		}

		void DrawGizmo() override
		{
			DrawSphereWires({ 0,0,0 }, 5.0f, 6, 6, GetColor(0x0000FFFF));
		}

		void Update() override
		{

		}

		void PhysicsUpdate() override
		{

		}

		void Start() override
		{

		}

		void SetupSkyboxImage()
		{
			nativeSkybox->model->materials[0].maps[MATERIAL_MAP_ALBEDO].texture = DataPacks::singleton().GetTexture2D(material);
		}

		void SwapMaterial(unsigned int materialId)
		{
			nativeSkybox->model->materials[0] = DataPacks::singleton().GetMaterial(0);
		}
	};
}
