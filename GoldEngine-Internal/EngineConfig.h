
using namespace System;
using namespace Newtonsoft::Json;

namespace Engine::Config
{
	public ref class Resolution
	{
	public:
		int x, y, w, h;

	public:
		[[JsonConstructorAttribute]]
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

	public ref class EngineConfiguration
	{
	public:
		String^ windowName;
		String^ logPath;
		Resolution^ resolution;
		unsigned int windowFlags;

	private:
		static EngineConfiguration^ self;

	public:
		initonly static EngineConfiguration^ defaultConfiguration = gcnew EngineConfiguration("Gold Engine Window", gcnew Resolution(0, 0, 1280, 720), "GoldEngine/main.log", 0);

	public:
		[[JsonConstructorAttribute]]
		EngineConfiguration(String^ windowName, Resolution^ resolution, String^ logPath, unsigned int flags)
		{
			self = this;
			this->resolution = resolution;
			this->windowName = windowName;
			this->logPath = logPath;
			this->windowFlags = flags;
		}

		EngineConfiguration()
		{
			self = this;
			this->logPath = defaultConfiguration->logPath;
			this->windowName = defaultConfiguration->windowName;
			this->resolution = defaultConfiguration->resolution;
			this->windowFlags = defaultConfiguration->windowFlags;
		}

		std::string getWindowName()
		{
			return CastStringToNative(windowName);
		}

	public:
		void ExportConfig(System::String^ fN)
		{
			File::WriteAllText(fN, Convert::ToBase64String(
				Encoding::UTF8->GetBytes(
					CypherLib::EncryptFileContents(
						JsonConvert::SerializeObject(this), passwd
					)
				)
			));
		}

	public:
		static EngineConfiguration^ singleton()
		{
			return self;
		}

	private:
		static void setSingleton(EngineConfiguration^ inst)
		{
			self = inst;
		}

	public:
		static EngineConfiguration^ ImportConfig(System::String^ fN)
		{
			if (File::Exists(fN))
			{
				String^ encodedData = File::ReadAllText(fN);

				EngineConfiguration^ configuration = JsonConvert::DeserializeObject<EngineConfiguration^>(
					CypherLib::DecryptFileContents(
						Encoding::UTF8->GetString(
							Convert::FromBase64String(encodedData)
						),
						passwd)
				);

				setSingleton(configuration);

				return configuration;
			}

			return EngineConfiguration::defaultConfiguration;
		}
	};
}