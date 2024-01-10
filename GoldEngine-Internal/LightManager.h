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
		unsigned int lightColor;
		float intensity;
		Engine::Internal::Components::Vector3^ target;
		rPBR::PBRLightType lightType;

		LightSource(String^ name, Engine::Internal::Components::Transform^ t, rPBR::PBRLightType lightType, Engine::Internal::Components::Vector3^ target, float intensity, Shader shader)  : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::LightSource)
		{
			nativeLightSource = new Native::NativeLightSource();
			this->lightType = lightType;
			this->target = target;
			this->intensity = intensity;
			nativeLightSource->SetShader(shader);
			nativeLightSource->SetLight(rPBR::PBRLightCreate(lightType, GetTransform()->position->toNative(), target->toNative(), GetColor(lightColor), intensity,shader));
			nativeLightSource->SetLightEnabled(true);
		}

		void Update() override
		{
			nativeLightSource->UpdateLighting();
		}

		rPBR::PBRLight GetLight() { return nativeLightSource->getLight(); }
		Shader GetShader() { return nativeLightSource->getShader(); }
	};

	public ref class LightManager : public Engine::Internal::Components::Object
	{
	public:
		System::Collections::Generic::List<LightSource^>^ lightSources;

	public:
		LightManager(String^ name, Engine::Internal::Components::Transform^ t) : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::LightManager)
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

			for each(LightSource^ light in lightSources)
			{
				int lightType = light->lightType;
				
				auto lightTransform = light->GetTransform();

				if (lightType == rPBR::LIGHT_POINT)
				{
					if (light->GetLight().enabled)
					{
						DrawSphereWires(lightTransform->position->toNative(), light->intensity, 8, 8, GetColor(light->lightColor));
					}
					else
					{
						DrawSphereWires(lightTransform->position->toNative(), light->intensity, 8, 8, {GetColor(light->lightColor).r, GetColor(light->lightColor).g, GetColor(light->lightColor).b, 128});
					}
				}
				else if (lightType == rPBR::LIGHT_DIRECTIONAL)
				{
					if (light->GetLight().enabled)
					{
						DrawLine3D(lightTransform->position->toNative(), light->target->toNative(), GetColor(light->lightColor));
					}
					else
					{
						Color col = GetColor(light->lightColor);
						DrawLine3D(lightTransform->position->toNative(), light->target->toNative(), {col.r, col.g, col.b, 128});
					}
				}
				else if (lightType == rPBR::LIGHT_SPOT)
				{
					DrawCylinderWiresEx(lightTransform->position->toNative(), light->target->toNative(), lightTransform->scale->x, light->intensity, 6, GetColor(light->lightColor));
				}
				else
				{
					DrawSphereWires(lightTransform->position->toNative(), light->intensity, 8, 8, GetColor(light->lightColor));
				}
			}
		}

		void Draw() override
		{

		}

		void Update() override
		{

		}
	};
}