#pragma once

#include <list>

namespace Engine::EngineObjects
{
	namespace Native
	{
		class NativeLightSource
		{
		private:
			rPBR::PBRLight light;
			Shader shader;

		public:
			NativeLightSource() {}

		public:
			rPBR::PBRLight getLight() { return light; }
			Shader getShader() { return shader; }
			void SetShader(Shader value) { shader = value; }
			void SetLight(rPBR::PBRLight value) { light = value; }

			void SetLightEnabled(bool value) { light.enabled = value; }

			void UpdateLighting()
			{
				rPBR::PBRLightUpdate(shader, light);
			}
		};
	}

	ref class LightSource : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeLightSource* nativeLightSource;

	public:
		unsigned long lightColor;
		float intensity;
		unsigned int shaderId;
		Engine::Internal::Components::Vector3^ target;
		rPBR::PBRLightType lightType;

		LightSource(String^ name, Engine::Internal::Components::Transform^ t, unsigned long lightColor, rPBR::PBRLightType lightType, Engine::Internal::Components::Vector3^ target, float intensity, unsigned int shader)  : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::ObjectType::LightSource)
		{
			nativeLightSource = new Native::NativeLightSource();
			this->lightType = lightType;
			this->lightColor = lightColor;
			this->target = target;
			this->shaderId = shader;
			this->intensity = intensity;
			nativeLightSource->SetShader(DataPacks::singleton().GetShader(shader));
			nativeLightSource->SetLight(rPBR::PBRLightCreate(lightType, GetTransform()->position->toNative(), target->toNative(), GetColor(lightColor), intensity, DataPacks::singleton().GetShader(shader)));
			nativeLightSource->SetLightEnabled(true);
		}

		void Init(unsigned long lightColor, float intensity, Engine::Internal::Components::Vector3^ target, rPBR::PBRLightType lightType, unsigned int shaderId) override
		{
			nativeLightSource = new Native::NativeLightSource();
			this->lightType = lightType;
			this->target = target;
			this->intensity = intensity;
			nativeLightSource->SetShader(DataPacks::singleton().GetShader(shaderId));
			nativeLightSource->SetLight(rPBR::PBRLightCreate(lightType, GetTransform()->position->toNative(), target->toNative(), GetColor(lightColor), intensity, DataPacks::singleton().GetShader(shaderId)));
			nativeLightSource->SetLightEnabled(true);
		}

		void Update() override
		{
			nativeLightSource->UpdateLighting();
		}

		void DrawGizmo() override
		{
			int lightType = this->lightType;

			auto lightTransform = this->GetTransform();

			if (lightType == rPBR::LIGHT_POINT)
			{
				if (this->GetLight().enabled)
				{
					DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, GetColor(this->lightColor));
				}
				else
				{
					DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, { GetColor(this->lightColor).r, GetColor(this->lightColor).g, GetColor(this->lightColor).b, 128 });
				}
			}
			else if (lightType == rPBR::LIGHT_DIRECTIONAL)
			{
				if (this->GetLight().enabled)
				{
					DrawLine3D(lightTransform->position->toNative(), this->target->toNative(), GetColor(this->lightColor));
				}
				else
				{
					Color col = GetColor(this->lightColor);
					DrawLine3D(lightTransform->position->toNative(), this->target->toNative(), { col.r, col.g, col.b, 128 });
				}
			}
			else if (lightType == rPBR::LIGHT_SPOT)
			{
				DrawCylinderWiresEx(lightTransform->position->toNative(), this->target->toNative(), lightTransform->scale->x, this->intensity, 6, GetColor(this->lightColor));
			}
			else
			{
				DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, GetColor(this->lightColor));
			}
		}

		void Draw() override
		{

		}

		rPBR::PBRLight GetLight() { return nativeLightSource->getLight(); }
		Shader GetShader() { return nativeLightSource->getShader(); }
	};

	public ref class LightManager : public Engine::Internal::Components::Object
	{
	public:
		System::Collections::Generic::List<LightSource^>^ lightSources;

	public:
		LightManager(String^ name, Engine::Internal::Components::Transform^ t) : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::ObjectType::LightManager)
		{
			lightSources = gcnew System::Collections::Generic::List<LightSource^>();
		}

	public:
		int AddLight(LightSource^ light, unsigned int shaderId)
		{
			lightSources->Add(light);
			
			Shader s = DataPacks::singleton().GetShader(shaderId);

			int numOfLightsLoc = GetShaderLocation(s, "numOfLights");

			std::vector<rPBR::PBRLight> _light;

			for (int x = 0; x < lightSources->Count; x++)
			{
				_light.push_back(lightSources[x]->GetLight());
			}

			TraceLog(LOG_INFO, "Lights %d", _light.size());

			SetShaderValue(s, numOfLightsLoc, &rPBR::lightsCount, SHADER_UNIFORM_INT);

			return lightSources->IndexOf(light);
		}

		void RemoveLight(LightSource^ light)
		{
			lightSources->Remove(light);
		}

		int* getLightCount() {
			int count = 0;
			count = lightSources->Count;
			return &count;
		}

		void DrawGizmo() override
		{
			auto t = GetTransform();

			DrawCubeWires(t->position->toNative(), 1, 1, 1, RED);
		}

		void Draw() override
		{

		}

		void Update() override
		{

		}
	};
}