#pragma once

#include <stdarg.h>

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	namespace Native
	{
		private class NativeModel
		{
		private:
			Model model;
			Shader shader;
			Texture texture;
			unsigned int tint;

		public:
			NativeModel(Model& m, Texture& tex, Shader& s, unsigned int tint)
			{
				this->model = m;
				this->texture = tex;
				this->shader = s;
				this->tint = tint;

				setup();
			}
			
			Model& getModel()
			{
				return model;
			}

			Shader& getShader()
			{
				return shader;
			}

			void setup()
			{
				RAYLIB::Color c =
				{
					(tint >> 0) & 0xFF,
					(tint >> 8) & 0xFF,
					(tint >> 16) & 0xFF,
					(tint >> 24) & 0xFF
				};

				model.materials[0].shader = shader;
				
				model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = c;
				model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = (Texture2D)texture;

				model.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
				model.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
				model.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
				model.materials[0].maps[MATERIAL_MAP_EMISSION].color = RAYLIB::WHITE;
				model.materials[0].maps[MATERIAL_MAP_EMISSION].value = 0.0f;
			}
		};
	}

	public ref class ModelRenderer : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeModel* modelManager;

	public:
		// datapack refs
		unsigned int model;
		unsigned int shader;
		unsigned int texture;
		unsigned int tint;
		// methods & constructor
		ModelRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint) : Engine::Internal::Components::Object(name, trans, Engine::Internal::Components::ObjectType::ModelRenderer, this->tag, Engine::Scripting::LayerManager::GetLayerFromId(1))
		{
			this->model = model;
			this->shader = shader;
			this->tint = tint;
			this->texture = texture;

			modelManager = new Native::NativeModel(
				DataPacks::singleton().GetModel(model), 
				DataPacks::singleton().GetTexture2D(texture), 
				DataPacks::singleton().GetShader(shader), 
				tint
			);
		}

		void Init(unsigned int model, unsigned int shader, unsigned int texture, unsigned int tint)
		{
			this->model = model;
			this->shader = shader;
			this->tint = tint;
			this->texture = texture;

			modelManager = new Native::NativeModel(DataPacks::singleton().GetModel(model), DataPacks::singleton().GetTexture2D(texture), DataPacks::singleton().GetShader(shader), tint);

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

			RAYLIB::Color c =
			{
					(tint >> 0) & 0xFF,
					(tint >> 8) & 0xFF,
					(tint >> 16) & 0xFF,
					(tint >> 24) & 0xFF
			};

			if (modelManager != nullptr)
				free(modelManager);

			modelManager = new Native::NativeModel(DataPacks::singleton().GetModel(model), DataPacks::singleton().GetTexture2D(texture), DataPacks::singleton().GetShader(shader), tint);


			/*
			m.materials[0].shader = DataPacks::singleton().GetShader(shader);
			*/

			if (modelManager == nullptr || &modelManager->getModel() == nullptr)
				return;

			RAYLIB::Vector2 baseVctr = { 0.5f, 0.5f };
			RAYLIB::Vector4 EmissiveColor = ColorNormalize(modelManager->getModel().materials[0].maps[MATERIAL_MAP_EMISSION].color);

			int emmisiveIntensityLocation = GetShaderLocation(modelManager->getShader(), "emissivePower");
			int emmisiveColorLocation = GetShaderLocation(modelManager->getShader(), "emissiveColor");
			int textureTilingLocation = GetShaderLocation(modelManager->getShader(), "tiling");
			float emissiveIntensity = 0.01f;

			RAYLIB::SetShaderValue(modelManager->getShader(), textureTilingLocation, &baseVctr, SHADER_UNIFORM_VEC2);
			RAYLIB::SetShaderValue(modelManager->getShader(), emmisiveColorLocation, &EmissiveColor, SHADER_UNIFORM_VEC4);
			RAYLIB::SetShaderValue(modelManager->getShader(), emmisiveIntensityLocation, &emissiveIntensity, SHADER_UNIFORM_FLOAT);

			rlCheckErrors();

			DrawModelEx(
				modelManager->getModel(),
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