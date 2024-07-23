#include "cfg.h"
#include <cstdio>
#include <iostream>
#include <string>
#include <regex>
#include <filesystem>
#include <fstream>
#include "flags.h"
#include "system2/sys2.h"
#include <sys/types.h>
#include <sstream>
#include <vector>

using namespace std::filesystem;

std::string convertWineArch()
{
	if (WINE_ARCH == wineArch::win32)
		return "win32";
	else if (WINE_ARCH == wineArch::win64)
		return "win64";

	return "win64";
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos) pos = str.length();
		std::string token = str.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());
	return tokens;
}

bool findAndRemoveMono()
{
	if (!hasMonoInstalled)
		return true;

	System2CommandInfo* commandInfo = new System2CommandInfo();
	char* outputBuffer = new char[1024];

	printf("Created Buffers\n");

	std::string command = (basePrefix + "wine uninstaller --list");

	SYSTEM2_RESULT result = System2Run(command.c_str(), commandInfo);

	printf("Got Result\n");

	if (result == SYSTEM2_RESULT_SUCCESS)
	{
		uint32_t readedBytes = 0;
		printf("Configuring readed bytes\n");
		if (System2ReadFromOutput(commandInfo, outputBuffer, 1023, &readedBytes) == SYSTEM2_RESULT_SUCCESS)
		{
			printf("Reading output\n");

			std::string output = std::string(outputBuffer);
		
			if (output.find("{") == std::string::npos)
				goto wineMonoUninstalled;

			std::string installed_packages = (output + "\n").substr(output.find_first_of("{")).c_str();

			printf(installed_packages.c_str());

			if (installed_packages.empty() || installed_packages == "")
				goto wineMonoUninstalled;

			std::vector<std::string> args = split(installed_packages, "|||");

			std::vector<std::string> uids_to_remove = { };

			for (int x = 1; x < args.size(); x++)
			{
				if (strstr(args[x].c_str(), "Mono"))
				{
					if (args[x - 1].find("{") != 0)
					{
						std::string uid = args[x - 1].substr(args[x - 1].find_first_of("{"));
						uids_to_remove.push_back(uid);
					}
					else
					{
						uids_to_remove.push_back(args[x - 1]);
					}
				}
			}

			printf("[Bootstrapper]: Wine may through errors due one of the wine mono packages depends from the other, so if you delete the root it will delete the dependency too.\nBut we got no way of ensuring the first package deleted will be the mono support.\n");

			for (std::string uid : uids_to_remove)
			{
				printf((uid + "\n").c_str());
				std::string command = basePrefix + "wine uninstaller --remove " + uid;

				system(command.c_str());
			}

			printf("[Bootstrapper]: Wine Mono has been uninstalled from the system, installing dependencies.");

			system(std::string("touch ./bootstrapper_data/monodel.stg").c_str());
		}
		else
		{
			printf("[ERROR]: Failed reading output hook.\n");
			return false;
		}
	}
	else
	{
		printf("[ERROR]: Failed to capture mono guid for uninstallation.\n");
		return false;
	}

wineMonoUninstalled:

	printf("Exiting mono uninstaller routine\n");

	delete commandInfo;
	delete outputBuffer;

	return true;
}

bool createWinePrefix()
{
	std::string command = basePrefix + "wineboot";
	
	int result = system(command.c_str());

	return (result == 0);
}

bool downloadDependencies()
{
	std::string unsolvedDependencies = "";

	if (dependenciesInstalled)
		goto onDependenciesFinished;
	
	for (const char* dependency : dependencies)
	{
		unsolvedDependencies += std::string(dependency) + " ";
	}

	if (!unsolvedDependencies.empty())
	{
		std::string command = basePrefix + " winetricks --force " + unsolvedDependencies;

		if (useUnattendedInstall)
			command += " --unattended";

		system(command.c_str());
	}

	dependenciesInstalled = true;

	system("touch ./bootstrapper_data/dependencies.stg");

onDependenciesFinished:
	return true;
}

void launchGame()
{
	system((basePrefix + "wine " + EXECUTABLE_NAME).c_str());
}

int main(int argc, char* argv[])
{
	for (int x = 0; x < argc; x++)
	{
		printf("%s\n", argv[x]);
		if (argv[x] == "-h" || argv[x] == "--help")
		{
			printf("Options List\n");
			printf("-h, --help | Shows this help\n");
			printf("-u, --unattended | Allows for winetricks to handle the dependency installation by itself\n");
			printf("-i, --install | Only installs the WINEPREFIX and dependencies, doesn't launch the game\n");
			exit(0);
			return 0;
		}
		else if(argv[x] == "-u" || argv[x] == "--unattended")
		{
			useUnattendedInstall = true;
		}
		else if (argv[x] == "-i" || argv[x] == "--install")
		{
			useGameLauncher = false;
		}
	}

	if (system("wine --version") == NOT_FOUND_EXIT)
	{
		printf("[Bootstrapper]: Wine has not been found, please install wine and try again"); 
		exit(0);
		return 0;
	}

	if (system("winetricks --version") == NOT_FOUND_EXIT)
	{
		printf("[Bootstrapper]: Downloading Winetricks.");
		system("wget  https://raw.githubusercontent.com/Winetricks/winetricks/master/src/winetricks");
		system("chmod +x winetricks");
		system("sudo cp winetricks /usr/local/bin/");
	}

	for (const char* dep : dependencies)
	{
		if (dep == "none")
		{
			dependenciesInstalled = true;
			break;
		}
	}

	prefixPath = std::string(WINE_PREFIX_PATH) + "/" + WINE_PREFIX;

	basePrefix = "env WINEPREFIX=" + prefixPath + " WINEARCH=" + convertWineArch() + " ";

	if (exists(prefixPath))
	{
		prefixExists = true;
	}

	if (!exists("./bootstrapper_data/"))
		system("mkdir ./bootstrapper_data/");

	if (exists("./bootstrapper_data/monodel.stg"))
		hasMonoInstalled = false;

	if (exists("./bootstrapper_data/dependencies.stg"))
		dependenciesInstalled = true;

	if (!prefixExists)
	{
		if (!createWinePrefix())
		{
			printf("[FATAL ERROR]: Could not create WINEPREFIX, aborting!");
			exit(1);
			return 1;
		}

		printf("[Bootstrapper]: WinePrefix setted up.\n");
	}

	printf("[Bootstrapper]: WinePrefix ready.\n");

	if (findAndRemoveMono())
	{
		if (downloadDependencies())
		{
			if(useGameLauncher)
				launchGame();
		}
		else
		{
			printf("[Error]: A problem during the installation has been found, could not install the dependencies");
		}
	}
	else
	{
		printf("[Error]: A problem during the installation has been found, could not uninstall mono");
	}

	return 0;
}