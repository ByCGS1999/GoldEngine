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

	[System::ObsoleteAttribute("The class Skybox is obsolete, please, use the ModelRenderer for this. (This class may be deleted or used for internal testing purposes and might have unexpected behaviours)")]
	public ref class Skybox : Engine::Internal::Components::GameObject
	{
	private:
		Native::NativeSkybox* nativeSkybox;

	public:
		unsigned int model;
		unsigned int material;
		unsigned int texture;

		Skybox(System::String^ s, Engine::Internal::Components::Transform^ t, unsigned int modelId, unsigned int matId, unsigned int texturePath) : GameObject(s, t, Engine::Internal::Components::ObjectType::Skybox, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
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
			auto t = getTransform();

			nativeSkybox->model->transform = RAYMATH::MatrixRotateXYZ({
				DEG2RAD * this->transform->scale->x,
				DEG2RAD * this->transform->scale->y,
				DEG2RAD * this->transform->scale->z
			});

			DrawModelEx(*nativeSkybox->model, t->position->toNative(), {0,0,0}, 0.0f, t->scale->toNative(), GetColor(0xFFFFFFFF));
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
			
		}
	};
}
