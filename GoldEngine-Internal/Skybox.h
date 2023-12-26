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
		unsigned int material;
		unsigned int texture;

		Skybox(String^ s, Engine::Internal::Components::Transform^ t, unsigned int matId, unsigned int texturePath) : Object(s, t, Engine::Internal::Components::Skybox, nullptr)
		{
			material = matId;
			texture = texturePath;

			nativeSkybox = new Native::NativeSkybox(LoadModel("Data/tmp/Engine/Models/Skybox_cube.glb"));
		}

		Skybox(Engine::Internal::Components::Object ^object) : Object(object->name, object->transform, object->type, object->parent) 
		{

		}

		void Init(unsigned int matId, unsigned int texturePath)
		{
			material = matId;
			texture = texturePath;
			nativeSkybox = new Native::NativeSkybox(LoadModel("Data/tmp/Engine/Models/Skybox_cube.glb"));
		}

		void Draw() override
		{
			Engine::Internal::Components::Transform^ t = GetTransform();
			DrawModel(*nativeSkybox->model, { t->position->x,t->position->y, t->position->z }, t->scale, GetColor(0xFFFFFFFF));
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
