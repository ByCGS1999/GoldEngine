#pragma once

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	public ref class ModelRenderer : public Engine::Internal::Components::Object
	{
	public:
		// datapack refs
		unsigned int model;
		unsigned int material;
		unsigned int texture;
		unsigned int tint;
		// methods & constructor
		ModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, unsigned int material, unsigned int texture, unsigned int tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::ModelRenderer)
		{
			this->model = model;
			this->material = material;
			this->tint = tint;
			this->texture = texture;
		}

		void Init(unsigned int model, unsigned int material, unsigned int texture, unsigned int tint)
		{
			this->model = model;
			this->material = material;
			this->tint = tint;
			this->texture = texture;

			auto modelInst = Engine::Assets::Storage::DataPacks::singleton().GetModel(model);
			auto materialInst = DataPacks::singleton().GetMaterial(material);
			auto texInst = DataPacks::singleton().GetTexture2D(texture);
			
			materialInst.maps[MATERIAL_MAP_ALBEDO].texture = texInst;
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

			DrawModel(
				DataPacks::singleton().GetModel(model),
				{ t->position->x,t->position->y, t->position->z },
				t->scale->x,
				c
			);
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