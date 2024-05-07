#pragma once
#include <list>
#include "../Object/Vector3.h"
#include "../Object/Transform.h"

namespace Engine::EngineObjects
{
	namespace Native
	{
		class NativeLightSource
		{
		private:
			rPBR::Light light;
			Shader shader;

		public:
			NativeLightSource(int lightType, ::Vector3 position, ::Vector3 target, ::Color color, float intensity, Shader s) 
			{
				light = rPBR::CreateLight(lightType, position, target, color, intensity, shader);
			}

		public:
			rPBR::Light getLight() { return light; }
			Shader getShader() { return shader; }
			void SetShader(Shader value) { shader = value; }
			void SetLight(rPBR::Light value) { light = value; }

			void SetLightEnabled(bool value) { light.enabled = value; }
			void SetIntensity(float value) { light.intensity = value; }
			void SetColor(float color[4]) { light.color[0] = color[0]; light.color[1] = color[1]; light.color[2] = color[2]; light.color[3] = color[3]; }
			void SetPosition(::Vector3 pos) { light.position = pos; }
			void SetTarget(::Vector3 pos) { light.target = pos; }

			void UpdateLighting()
			{
				rPBR::UpdateLight(shader, light);
			}
		};
	}

	public ref class LightSource : public Engine::Internal::Components::Object
	{
	private:
		Native::NativeLightSource* nativeLightSource;

	public:
		unsigned int lightColor;
		float intensity;
		unsigned int shaderId;
		Engine::Components::Vector3^ target;
		rPBR::LightType lightType;
		bool enabled;

		LightSource(String^ name, Engine::Internal::Components::Transform^ transform, unsigned int lightColor, int lightType, Engine::Components::Vector3^ target, float intensity, unsigned int shader) : Engine::Internal::Components::Object(name, transform, Engine::Internal::Components::ObjectType::LightSource, this->tag)
		{
			this->lightType = (rPBR::LightType)lightType;
			this->lightColor = lightColor;
			this->target = target;
			this->shaderId = shader;
			this->intensity = intensity;
			nativeLightSource = new Native::NativeLightSource(
				this->lightType,
				GetTransform()->position->toNative(),
				this->target->toNative(),
				GetColor(this->lightColor),
				this->intensity,
				DataPacks::singleton().GetShader(this->shaderId)
			);
			nativeLightSource->SetLightEnabled(true);
			enabled = true;
		}

		void Init(unsigned int lightColor, float intensity, Engine::Components::Vector3^ target, int lightType, unsigned int shaderId) override
		{
			this->lightColor = lightColor;
			this->lightType = (rPBR::LightType)lightType;
			this->target = target;
			this->intensity = intensity;
			::Color c =
			{
				lightColor >> 0,
				lightColor >> 8,
				lightColor >> 16,
				lightColor >> 24
			};

			nativeLightSource = new Native::NativeLightSource(
				this->lightType,
				GetTransform()->position->toNative(),
				this->target->toNative(),
				GetColor(this->lightColor),
				this->intensity,
				DataPacks::singleton().GetShader(this->shaderId)
			);
			nativeLightSource->SetLightEnabled(true);
			enabled = true;
		}

		rPBR::Light GetLight() { return nativeLightSource->getLight(); }
		Shader GetShader() { return nativeLightSource->getShader(); }

		void Update() override
		{
			nativeLightSource->SetLightEnabled(enabled);
			nativeLightSource->SetIntensity(intensity);
			nativeLightSource->SetPosition(GetTransform()->position->toNative());
			nativeLightSource->SetTarget(target->toNative());

			float v[4] = {
				lightColor >> 0,
				lightColor >> 8,
				lightColor >> 16,
				lightColor >> 24
			};

			nativeLightSource->SetColor(v);
		}

		void DrawGizmo() override
		{
			::Color c =
			{
				lightColor >> 0,
				lightColor >> 8,
				lightColor >> 16,
				lightColor >> 24
			};

			int lightType = this->lightType;

			auto lightTransform = this->GetTransform();

			if (lightType == rPBR::LIGHT_POINT)
			{
				if (this->GetLight().enabled)
				{
					DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, c);
				}
				else
				{
					DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, { c.r, c.g, c.b, 128 });
				}
			}
			else if (lightType == rPBR::LIGHT_DIRECTIONAL)
			{
				if (this->GetLight().enabled)
				{
					DrawLine3D(lightTransform->position->toNative(), this->target->toNative(), c);
				}
				else
				{
					DrawLine3D(lightTransform->position->toNative(), this->target->toNative(), { c.r, c.g, c.b, 128 });
				}
			}
			else if (lightType == rPBR::LIGHT_SPOT)
			{
				DrawCylinderWiresEx(lightTransform->position->toNative(), this->target->toNative(), lightTransform->scale->x, this->intensity, 6, c);
			}
			else
			{
				DrawSphereWires(lightTransform->position->toNative(), this->intensity, 8, 8, c);
			}
		}

		void Draw() override
		{

		}
	};

	public ref class LightManager : public Engine::Internal::Components::Object
	{
	private:
		System::Collections::Generic::List<Engine::EngineObjects::LightSource^>^ lightSources;
		static LightManager^ lightdm;
	public:
		String^ vs_path;
		String^ fs_path;

		unsigned int ambientColor;

	public:
		LightManager(String^ name, Engine::Internal::Components::Transform^ t, String^ vs, String^ fs) : Engine::Internal::Components::Object(name, t, Engine::Internal::Components::ObjectType::LightManager, this->tag)
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

	private:
		Shader CreateLocs(Shader shader, int maxLights)
		{
			shader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(shader, "albedoMap");
			shader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(shader, "mraMap");
			shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(shader, "normalMap");
			shader.locs[SHADER_LOC_MAP_EMISSION] = GetShaderLocation(shader, "emissiveMap");
			shader.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(shader, "albedoColor");
			shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
			shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");

			float ambientIntensity = 0.02f;
			::Color color = { 
				ambientColor >> 0,
				ambientColor >> 8, 
				ambientColor >> 16,
				ambientColor >> 24 
			};

			::Vector3 ambientColorNormalized = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f };

			SetShaderValue(shader, GetShaderLocation(shader, "ambientColor"), &ambientColorNormalized, SHADER_UNIFORM_VEC3);
			SetShaderValue(shader, GetShaderLocation(shader, "ambient"), &ambientIntensity, SHADER_UNIFORM_FLOAT);

			int emissiveIntensityLoc = GetShaderLocation(shader, "emissivePower");
			int emissiveColorLoc = GetShaderLocation(shader, "emissiveColor");
			int textureTilingLoc = GetShaderLocation(shader, "tiling");

			int usage = 1;
			SetShaderValue(shader, GetShaderLocation(shader, "useTexAlbedo"), &usage, SHADER_UNIFORM_INT);
			SetShaderValue(shader, GetShaderLocation(shader, "useTexNormal"), &usage, SHADER_UNIFORM_INT);
			SetShaderValue(shader, GetShaderLocation(shader, "useTexMRA"), &usage, SHADER_UNIFORM_INT);
			SetShaderValue(shader, GetShaderLocation(shader, "useTexEmissive"), &usage, SHADER_UNIFORM_INT);

			return shader;
		}

	public:
		void ShaderUpdate(unsigned int shaderId)
		{
			String^ vs_net = File::ReadAllText(vs_path);
			String^ fs_net = File::ReadAllText(fs_path);

			fs_net = fs_net->Replace("%numlights%", lightSources->Count.ToString());

			Shader s = LoadShaderFromMemory(CastStringToNative(vs_net).c_str(), CastStringToNative(fs_net).c_str());

			s = CreateLocs(s, lightSources->Count);

			std::vector<rPBR::Light> _light;

			for (int x = 0; x < lightSources->Count; x++)
			{
				_light.push_back(lightSources[x]->GetLight());
			}

			int lightCountLoc = GetShaderLocation(s, "numOfLights");
			int maxLightCount = _light.size();
			SetShaderValue(s, lightCountLoc, &maxLightCount, SHADER_UNIFORM_INT);

			TraceLog(LOG_INFO, "Lights %d", _light.size());

			if (&DataPacks::singleton().GetShader(shaderId) == nullptr)
			{
				DataPacks::singleton().AddShader(shaderId, s);
			}
			else
			{
				UnloadShader(DataPacks::singleton().GetShader(shaderId));

				DataPacks::singleton().AddShader(shaderId, s);
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
				
				s = CreateLocs(s, lightSources->Count);

				std::vector<rPBR::Light> _light;

				for (int x = 0; x < lightSources->Count; x++)
				{
					_light.push_back(lightSources[x]->GetLight());
				}

				int lightCountLoc = GetShaderLocation(s, "numOfLights");
				int maxLightCount = _light.size();
				SetShaderValue(s, lightCountLoc, &maxLightCount, SHADER_UNIFORM_INT);

				TraceLog(LOG_INFO, "Lights %d", _light.size());

				if (&DataPacks::singleton().GetShader(shaderId) == nullptr)
				{
					DataPacks::singleton().AddShader(shaderId, s);
				}
				else
				{
					DataPacks::singleton().AddShader(shaderId, s);
				}

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
				rPBR::UpdateLight(light->GetShader(), light->GetLight());
			}
		}
	};
}