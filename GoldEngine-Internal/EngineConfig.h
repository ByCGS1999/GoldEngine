
using namespace System;
using namespace Newtonsoft::Json;

namespace Engine::Config
{
	ref class Resolution
	{
		int x, y, w, h;

	public:
		Resolution(int x, int y, int w, int h)
		{
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	ref class EngineConfiguration
	{
	public:
		String^ windowName;
		String^ encryptionPassword;
		String^ logPath;
		Resolution^ resolution;

	private:
		static EngineConfiguration^ self;

	public:
		initonly static EngineConfiguration^ defaultConfiguration = gcnew EngineConfiguration(ENCRYPTION_PASSWORD, "Gold Engine Window", gcnew Resolution(0, 0, 1280, 720), "GoldEngine/main.log");

	public:
		EngineConfiguration(String^ password, String^ windowName, Resolution^ resolution, String^ logPath)
		{
			self = this;
			this->encryptionPassword = password;
			this->resolution = resolution;
			this->windowName = windowName;
			this->logPath = logPath;
		}

	public:
		std::string getPassword()
		{
			return CastStringToNative(encryptionPassword);
		}

	public:
		void ExportConfig(System::String^ fN)
		{
			File::WriteAllText(fN, Convert::ToBase64String(Encoding::UTF8->GetBytes(CypherLib::Encrypt(JsonConvert::SerializeObject(this)))));
		}

	public:
		static EngineConfiguration^ singleton()
		{
			return self;
		}

	public:
		static EngineConfiguration^ ImportConfig(System::String^ fN)
		{
			if (File::Exists(fN))
			{
				String^ encodedData = File::ReadAllText(fN);
				EngineConfiguration^ configuration = JsonConvert::DeserializeObject<EngineConfiguration^>(Encoding::UTF8->GetString(Convert::FromBase64String(encodedData)));
				return configuration;
			}

			return EngineConfiguration::defaultConfiguration;
		}
	};
}