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
		unsigned long tint;
		// methods & constructor
		ModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, unsigned int material, unsigned int texture, unsigned long tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::ModelRenderer)
		{
			this->model = model;
			this->material = material;
			this->tint = tint;
			this->texture = texture;
		}

		void Init(unsigned int model, unsigned int material, unsigned int texture, unsigned long tint)
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
			DrawModel(
				DataPacks::singleton().GetModel(model),
				{ t->position->x,t->position->y, t->position->z }, 
				t->scale->x, 
				GetColor(tint)
			);
		}

		void SetColorTint(unsigned int hexValue)
		{
			tint = hexValue;
		}
	};
}