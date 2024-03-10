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

	public ref class LightSource : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeLightSource* nativeLightSource;

	public:
		unsigned long lightColor;
		float intensity;
		unsigned int shaderId;
		Engine::Internal::Components::Vector3^ target;
		rPBR::PBRLightType lightType;

		LightSource(String^ name, Engine::Internal::Components::Transform^ transform, unsigned long lightColor, int lightType, Engine::Internal::Components::Vector3^ target, float intensity, unsigned int shader) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::LightSource)
		{
			nativeLightSource = new Native::NativeLightSource();
			this->lightType = (rPBR::PBRLightType)lightType;
			this->lightColor = lightColor;
			this->target = target;
			this->shaderId = shader;
			this->intensity = intensity;
			nativeLightSource->SetShader(DataPacks::singleton().GetShader(shader));
			auto light = rPBR::PBRLightCreate(
				this->lightType,
				GetTransform()->position->toNative(),
				this->target->toNative(),
				GetColor(this->lightColor),
				this->intensity,
				DataPacks::singleton().GetShader(this->shaderId)
			);
			nativeLightSource->SetLight(light);
			nativeLightSource->SetLightEnabled(true);

		}

		void Init(unsigned long lightColor, float intensity, Engine::Internal::Components::Vector3^ target, int lightType, unsigned int shaderId) override
		{
			nativeLightSource = new Native::NativeLightSource();
			this->lightColor = lightColor;
			this->lightType = (rPBR::PBRLightType)lightType;
			this->target = target;
			this->intensity = intensity;
			nativeLightSource->SetShader(DataPacks::singleton().GetShader(shaderId));
			nativeLightSource->SetLight(rPBR::PBRLightCreate(lightType, GetTransform()->position->toNative(), target->toNative(), GetColor(lightColor), intensity, DataPacks::singleton().GetShader(shaderId)));
			nativeLightSource->SetLightEnabled(true);
		}

		void Update() override
		{

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
	private:
		System::Collections::Generic::List<Engine::EngineObjects::LightSource^>^ lightSources;
		static LightManager^ lightdm;
	public:
		String^ vs_path;
		String^ fs_path;

	public:
		LightManager(String^ name, Engine::Internal::Components::Transform^ t, String^ vs, String^ fs) : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::ObjectType::LightManager)
		{
			lightdm = this;
			lightSources = gcnew System::Collections::Generic::List<Engine::EngineObjects::LightSource^>();
			vs_path = vs;
			fs_path = fs;
		}

	public:
		static LightManager^ singleton()
		{
			return lightdm;
		}

	public:
		Engine::EngineObjects::LightSource^ getLight(int index)
		{
			if (lightSources->Count > index)
			{
				return lightSources[index];
			}
			else
			{
				return nullptr;
			}
		}

	public:
		int AddLight(Engine::EngineObjects::LightSource^ light, unsigned int shaderId)
		{
			if (!lightSources->Contains(light))
			{
				lightSources->Add(light);

				String^ vs_net = File::ReadAllText(vs_path);
				String^ fs_net = File::ReadAllText(fs_path);

				fs_net = fs_net->Replace("%numlights%", lightSources->Count.ToString());

				Shader s = LoadShaderFromMemory(CastStringToNative(vs_net).c_str(), CastStringToNative(fs_net).c_str());
				
				int numOfLightsLoc = GetShaderLocation(s, "numOfLights");
				
				if (&DataPacks::singleton().GetShader(shaderId) == nullptr)
				{
					DataPacks::singleton().AddShader(shaderId, s);
				}
				else
				{
					DataPacks::singleton().SetShader(shaderId, s);
				}

				std::vector<rPBR::PBRLight> _light;

				for (int x = 0; x < lightSources->Count; x++)
				{
					_light.push_back(lightSources[x]->GetLight());
				}

				TraceLog(LOG_INFO, "Lights %d", _light.size());

				SetShaderValue(s, numOfLightsLoc, &rPBR::lightsCount, SHADER_UNIFORM_INT);

				return lightSources->IndexOf(light);
			}
			else
			{
				return -1;
			}
		}

		void RemoveLight(Engine::EngineObjects::LightSource^ light)
		{
			lightSources->Remove(light);
		}

		int* getLightCount() {
			int count = 0;
			count = lightSources->Count;
			return &count;
		}

		bool hasLight(LightSource^ source)
		{
			return lightSources->Contains(source);
		}

		void DrawGizmo() override
		{

		}

		void Draw() override
		{

		}

		void Update() override
		{
			for each (auto light in lightSources)
			{
				rPBR::PBRLightUpdate(light->GetShader(), light->GetLight());
			}
		}
	};
}