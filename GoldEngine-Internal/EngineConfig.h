
using namespace System;
using namespace Newtonsoft::Json;

namespace Engine::Config
{
	ref class Resolution
	{
	public:
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

	ref class EngineSecrets
	{
	public:
		String^ encryptionPassword;

	private:
		static EngineSecrets^ self;

	public:
		initonly static EngineSecrets^ default = gcnew EngineSecrets(ENCRYPTION_PASSWORD);

	public:
		EngineSecrets(String^ password)
		{
			self = this;
			this->encryptionPassword = password;
		}

	public:
		std::string getPassword()
		{
			return CastStringToNative(encryptionPassword);
		}

		static EngineSecrets^ singleton()
		{
			return self;
		}

		void ExportSecrets(System::String^ fN)
		{
			File::WriteAllText(fN, Convert::ToBase64String(
				Encoding::UTF8->GetBytes
					(
						CypherLib::EncryptFileContents(encryptionPassword, int::Parse(File::ReadAllText("./Data/Keys/map.iv")))
					)
				)
			);
		}

		static EngineSecrets^ ImportSecrets(System::String^ fN)
		{
			if (File::Exists(fN))
			{
				String^ encodedData = File::ReadAllText(fN);
				String^ decodedData = Encoding::UTF8->GetString(Convert::FromBase64String(encodedData));

				String^ password = CypherLib::DecryptFileContents(decodedData, 
					int::Parse(File::ReadAllText("./Data/Keys/map.iv")
					)
				);

				return gcnew EngineSecrets(password);
			}

			return EngineSecrets::default;
		}

	};

	ref class EngineConfiguration
	{
	public:
		String^ windowName;
		String^ logPath;
		Resolution^ resolution;

	private:
		static EngineConfiguration^ self;

	public:
		initonly static EngineConfiguration^ defaultConfiguration = gcnew EngineConfiguration("Gold Engine Window", gcnew Resolution(0, 0, 1280, 720), "GoldEngine/main.log");

	public:
		EngineConfiguration(String^ windowName, Resolution^ resolution, String^ logPath)
		{
			self = this;
			this->resolution = resolution;
			this->windowName = windowName;
			this->logPath = logPath;
		}

		std::string getWindowName()
		{
			return CastStringToNative(windowName);
		}

	public:
		void ExportConfig(System::String^ fN)
		{
			File::WriteAllText(fN, Convert::ToBase64String(Encoding::UTF8->GetBytes(CypherLib::EncryptFileContents(JsonConvert::SerializeObject(this), passwd))));
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
				EngineConfiguration^ configuration = JsonConvert::DeserializeObject<EngineConfiguration^>(
					CypherLib::EncryptFileContents(
						Encoding::UTF8->GetString(
							Convert::FromBase64String(encodedData)
						)
						,
						passwd)
				);
				return configuration;
			}

			return EngineConfiguration::defaultConfiguration;
		}
	};
}