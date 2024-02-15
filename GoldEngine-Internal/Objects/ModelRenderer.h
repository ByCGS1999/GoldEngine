#pragma once

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	namespace Native
	{
		class NativeModelRenderer
		{
		public:
			std::unique_ptr<Model> model;
			std::unique_ptr<Material> material;
			std::unique_ptr<Texture2D> texture;
			unsigned long color_hex;

			NativeModelRenderer(Model m, Material mat, Texture2D tex, unsigned long hex) : model(std::make_unique<Model>(m)),
				material(std::make_unique<Material>(mat)), texture(std::make_unique<Texture2D>(tex)),
				color_hex(hex)
			{

			}
		};
	}

	public ref class ModelRenderer : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeModelRenderer* nativeRenderer;

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

			nativeRenderer = new Native::NativeModelRenderer(DataPacks::singleton().GetModel(model), DataPacks::singleton().GetMaterial(material), DataPacks::singleton().GetTexture2D(texture), tint);
		}

		void SetNativeRenderer(Native::NativeModelRenderer* renderer)
		{
			if (nativeRenderer != nullptr)
				free(nativeRenderer);

			nativeRenderer = renderer;
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

			nativeRenderer = new Native::NativeModelRenderer(modelInst, materialInst, texInst, tint);
		}

		Native::NativeModelRenderer* GetNativeRenderer()
		{
			return nativeRenderer;
		}


		void Update() override {}
		void PhysicsUpdate() override {}
		void Start() override {}

		void Draw() override
		{
			auto t = GetTransform();
			DrawModel(
				*nativeRenderer->model, 
				{ t->position->x,t->position->y, t->position->z }, 
				t->scale->x, 
				GetColor(nativeRenderer->color_hex)
			);
		}

		void SetColorTint(unsigned int hexValue)
		{
			nativeRenderer->color_hex = hexValue;
		}
	};
}