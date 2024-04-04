#pragma once

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	public ref class ModelRenderer : public Engine::Internal::Components::Object
	{
	public:
		// datapack refs
		unsigned int model;
		unsigned int shader;
		unsigned int texture;
		unsigned int tint;
		// methods & constructor
		ModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::ModelRenderer, this->tag)
		{
			this->model = model;
			this->shader = shader;
			this->tint = tint;
			this->texture = texture;
		}

		void Init(unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint)
		{
			this->model = model;
			this->shader = shader;
			this->tint = tint;
			this->texture = texture;

			auto modelInst = Engine::Assets::Storage::DataPacks::singleton().GetModel(model);
			auto texInst = DataPacks::singleton().GetTexture2D(texture);

			modelInst.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texInst;

			/*
			modelInst.materials[0].shader = DataPacks::singleton().GetShader(shader);
			modelInst.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
			modelInst.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
			modelInst.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
			modelInst.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
			modelInst.materials[0].maps[MATERIAL_MAP_EMISSION].value = 0.0f;
			*/
		}

		void Update() override {}
		void PhysicsUpdate() override {}
		void Start() override {}

		void Draw() override
		{
			auto t = GetTransform();

			Color c =
			{
				tint >> 0,
				tint >> 8,
				tint >> 16,
				tint >> 24
			};

			auto m = DataPacks::singleton().GetModel(model);
			auto texInst = DataPacks::singleton().GetTexture2D(texture);

			//m.materials[0].shader = DataPacks::singleton().GetShader(shader);
			m.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texInst;

			/*
			m.materials[0].shader = DataPacks::singleton().GetShader(shader);
			m.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
			m.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
			m.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
			m.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
			m.materials[0].maps[MATERIAL_MAP_EMISSION].value = 0.0f;
			*/

			DrawModelEx(
				m,
				{ t->position->x,t->position->y, t->position->z },
				t->rotation->toNative(),
				t->rotationValue,
				t->scale->toNative(),
				c
			);
		}

		void SetModel(unsigned int m)
		{
			this->model = m;
		}

		void SetMaterial(unsigned int mat)
		{
			auto modelInst = Engine::Assets::Storage::DataPacks::singleton().GetModel(model);
			auto materialInst = DataPacks::singleton().GetShader(shader);
			auto texInst = DataPacks::singleton().GetTexture2D(texture);

			modelInst.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = texInst;
		}

		void SetColorTint(unsigned int hexValue)
		{
			tint = hexValue;
		}

		void Destroy() override
		{
			Object::Destroy();
		}
	};
}