#pragma once

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	public ref class MeshRenderer : public Engine::EngineObjects::Script
	{
	public:

		MeshRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, List<unsigned int>^ mats, unsigned int tint) : Script(name, trans)
		{
			attributes->addAttribute(Engine::Scripting::Attribute::New("model", model, __int64::typeid));
			attributes->addAttribute(Engine::Scripting::Attribute::New("materials", mats, List<__int64>::typeid));
			attributes->addAttribute(Engine::Scripting::Attribute::New("tint", tint, __int64::typeid));
		}

		void Init(unsigned int model, List<unsigned int>^ mats, unsigned int tint)
		{
			attributes->getAttribute("model")->setValue(model);
			attributes->getAttribute("materials")->setValue(model);
			attributes->getAttribute("tint")->setValue(model);

			auto modelInst = Engine::Assets::Storage::DataPacks::singleton().GetModel(model);

			auto materials = attributes->getAttribute("materials")->getValue<List<unsigned int>^>();

			for (int x = 0; x < materials->Count; x++)
			{
				modelInst.materials[x] = DataPacks::singleton().GetMaterial(materials[x]);

				/*
				modelInst.materials[0].shader = DataPacks::singleton().GetShader(shader);
				modelInst.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
				modelInst.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
				modelInst.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
				modelInst.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
				modelInst.materials[0].maps[MATERIAL_MAP_EMISSION].value = 0.0f;
				*/
			}
		}

		void Update() override {}
		void PhysicsUpdate() override {}
		void Start() override {}

		void Draw() override
		{
			auto t = GetTransform();

			__int64 tint = attributes->getAttribute("tint")->getValue<__int64>();

			Color c =
			{
				tint >> 0,
				tint >> 8,
				tint >> 16,
				tint >> 24
			};

			auto m = DataPacks::singleton().GetModel(attributes->getAttribute("model")->getValue<__int64>());
			auto jarray = attributes->getAttribute("materials")->getValue<Newtonsoft::Json::Linq::JArray^>();
			auto materials = jarray->ToObject<List<unsigned int>^>();
			

			for (int x = 0; x < materials->Count; x++)
			{
				m.materials[x] = DataPacks::singleton().GetMaterial(materials[x]);

				/*
				m.materials[0].shader = DataPacks::singleton().GetShader(shader);
				m.materials[0].maps[MATERIAL_MAP_METALNESS].value = 0.0f;
				m.materials[0].maps[MATERIAL_MAP_ROUGHNESS].value = 0.0f;
				m.materials[0].maps[MATERIAL_MAP_OCCLUSION].value = 1.0f;
				m.materials[0].maps[MATERIAL_MAP_EMISSION].color = WHITE;
				m.materials[0].maps[MATERIAL_MAP_EMISSION].value = 0.0f;
				*/
			}

			DrawModelEx(
				m,
				{ t->position->x,t->position->y, t->position->z },
				t->rotation->toNative(),
				t->rotationValue,
				t->scale->toNative(),
				c
			);
		}

		void updateMaterial(int materialIndex, unsigned int newMaterial)
		{
			auto materials = attributes->getAttribute("materials")->getValue<List<unsigned int>^>();

			if (materialIndex >= materials->Count)
				materials->Add(newMaterial);
			else
				materials[materialIndex] = newMaterial;

			attributes->getAttribute("materials")->setValue(materials);
		}
		
		void Destroy() override
		{
			Object::Destroy();
		}
	};
}