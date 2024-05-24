#pragma once

#include <stdarg.h>

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	namespace Native
	{
		private class NativeModel
		{
		public:
			Model model;

		private:
			Shader shader;
			Texture texture;
			unsigned int tint;

		public:
			NativeModel(Model m, Texture tex, Shader s, unsigned int tint)
			{
				this->model = m;
				this->texture = tex;
				this->shader = s;
				this->tint = tint;

				setup();
			}

			void setup()
			{
				::Color c =
				{
					tint >> 0,
					tint >> 8,
					tint >> 16,
					tint >> 24
				};

				model.materials[0].shader = shader;
				model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = c;
				model.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = (Texture2D)texture;

				model.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
				model.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
				model.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
				model.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
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

			::Color c =
			{
				tint >> 0,
				tint >> 8,
				tint >> 16,
				tint >> 24
			};

			if (modelManager != nullptr)
				free(modelManager);

			modelManager = new Native::NativeModel(DataPacks::singleton().GetModel(model), DataPacks::singleton().GetTexture2D(texture), DataPacks::singleton().GetShader(shader), tint);


			/*
			m.materials[0].shader = DataPacks::singleton().GetShader(shader);
			*/

			if (modelManager == nullptr || &modelManager->model == nullptr)
				return;

			DrawModelEx(
				modelManager->model,
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