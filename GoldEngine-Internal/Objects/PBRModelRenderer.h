#pragma once
namespace Engine::EngineObjects
{
	/*
	namespace Native
	{
		class NativePBRModelRenderer
		{
		public:
			std::unique_ptr<rPBR::PBRModel> model;
			rPBR::PBRMaterial material;

			NativePBRModelRenderer(rPBR::PBRModel m)
			{
				model = std::make_unique<rPBR::PBRModel>(m);
				material = {0};
			}
		};
	}
	*/

	public ref class PBRModelRenderer : public Engine::Internal::Components::GameObject
	{
	public:
		PBRModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans) : Engine::Internal::Components::GameObject(name, trans, Engine::Internal::Components::ObjectType::PBR_ModelRenderer, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{

		}

		/*
	private:
		Native::NativePBRModelRenderer* nativeRenderer;
		int emissiveCnt = 0;

	public:
		unsigned int model_id;
		unsigned int shader_id;
		unsigned int texture_id;
		unsigned int color_hex;

		PBRModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint) : Engine::Internal::Components::GameObject(name, trans, Engine::Internal::Components::ObjectType::PBR_ModelRenderer, this->tag)
		{
			rPBR::PBRModel pbrModel = rPBR::PBRModelLoad(&DataPacks::singleton().GetModel(model));
			model_id = model;
			color_hex = tint;
			shader_id = shader;
			texture_id = texture;
			nativeRenderer = new Native::NativePBRModelRenderer(pbrModel);
			rPBR::PBRMaterialSetup(&nativeRenderer->material, DataPacks::singleton().GetShader(shader), nullptr);
			rPBR::PBRLoadTextures(&nativeRenderer->material, rPBR::PBRTexType::PBR_TEXTURE_ALBEDO, DataPacks::singleton().GetTexture2D(texture));
		}

		void Init(unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint)
		{
			rPBR::PBRModel pbrModel = rPBR::PBRModelLoad(&DataPacks::singleton().GetModel(model));
			color_hex = tint;
			shader_id = shader;
			texture_id = texture;
			nativeRenderer = new Native::NativePBRModelRenderer(pbrModel);
			SetupMaterial(shader);
			SetTexture(texture, rPBR::PBR_TEXTURE_ALBEDO);
			SetColor(rPBR::PBR_COLOR_ALBEDO, GetColor(tint));
		}

		void SetTexture(unsigned int textureId, rPBR::PBRTexType texType)
		{
			rPBR::PBRLoadTextures(&nativeRenderer->material, texType, DataPacks::singleton().GetTexture2D(textureId));
		}

		void SetVector(Engine::Internal::Components::Vector2^ value, rPBR::PBRVec2Type vec2Type)
		{
			rPBR::PBRSetVec2(&nativeRenderer->material, vec2Type, value->toNative());
		}

		void SetColor(rPBR::PBRColorType colorType, Color color)
		{
			rPBR::PBRSetColor(&nativeRenderer->material, colorType, color);
		}

		void SetMaterial(int materialIndex)
		{
			rPBR::PBRSetMaterial(nativeRenderer->model.get(), &nativeRenderer->material, materialIndex);
		}

		void SetupMaterial(unsigned int shaderId) 
		{
			rPBR::PBRMaterialSetup(&nativeRenderer->material, DataPacks::singleton().GetShader(shaderId), nullptr);
		}

		void Draw() override
		{
			auto t = getTransform();
			Color c =
			{
				color_hex >> 0,
				color_hex >> 8,
				color_hex >> 16,
				color_hex >> 24
			};
			rPBR::PBRDrawModelEx(*nativeRenderer->model, t->position->toNative(), t->rotation->toNative(), t->rotationValue, t->scale->toNative(), c);
		}

		void Update() override
		{
			if (emissiveCnt <= 0)
			{
				emissiveCnt = GetRandomValue(0, 20);
				rPBR::PBRSetFloat(&nativeRenderer->material, rPBR::PBR_PARAM_EMISSIVE, (float)GetRandomValue(0, 100) / 100);
			}
		}*/
	};
}