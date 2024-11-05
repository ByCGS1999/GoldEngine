#pragma once

using namespace Engine::Assets::Storage;

namespace Engine::EngineObjects
{
	public ref class MeshRenderer : public Engine::EngineObjects::ScriptBehaviour
	{
	public:
		MeshRenderer(String^ name, Engine::Internal::Components::Transform^ trans, unsigned int model, List<unsigned int>^ mats, unsigned int tint) : ScriptBehaviour(name, trans)
		{
			attributes->addAttribute(Engine::Scripting::Attribute::New(Engine::Scripting::AccessLevel::Public, "model", model, UInt32::typeid));
			attributes->addAttribute(Engine::Scripting::Attribute::New(Engine::Scripting::AccessLevel::Public, "materials", mats, List<UInt32>::typeid));
			attributes->addAttribute(Engine::Scripting::Attribute::New(Engine::Scripting::AccessLevel::Public, "tint", gcnew Engine::Components::Color(tint), Engine::Components::Color::typeid));
		}

		void Init(unsigned int model, List<unsigned int>^ mats, unsigned int tint)
		{
			attributes->getAttribute("model")->setValue(model, false);
			attributes->getAttribute("materials")->setValue(mats, false);
			attributes->getAttribute("tint")->setValue(gcnew Engine::Components::Color(tint), false);

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
			auto t = getTransform();

			if (!attributes->getAttribute("tint") || !attributes->getAttribute("model") || !attributes->getAttribute("material"))
				return;


			Model m = DataPacks::singleton().GetModel(attributes->getAttribute("model")->getValue<UInt32>());
			auto jarray = attributes->getAttribute("materials")->getValue<Newtonsoft::Json::Linq::JArray^>();
			auto materials = jarray->ToObject<List<unsigned int>^>();
			
			RAYLIB::Color c =
			{
				255,
				0,
				0,
				255
			};

			m.transform = RAYMATH::MatrixRotateXYZ({
				DEG2RAD * this->transform->rotation->x,
				DEG2RAD * this->transform->rotation->y,
				DEG2RAD * this->transform->rotation->z
			});

			DrawModelEx(
				m,
				{ t->position->x,t->position->y, t->position->z },
				{ 0,0,0 },
				0.0f,
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
			GameObject::Destroy();
		}
	};
}