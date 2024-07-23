#pragma once

// DO NOT TOUCH THESE
typedef enum wineArch { win64, win32 } wineArch;
#define _CRT_SECURE_NO_WARNINGS


// EDIT THESE

const char* dependencies[] = { // dependencies of the game, these are the default dependencies used by the engine, tbh idk if glut and gdiplus are even required, i guess not...
	"dotnet48", // will also install dotnet40 aka .NET Framework 4.0
	"vcrun2022"
};

#define PRODUCT_NAME "Gold Engine Game" // Name of the product (game)
#define WINE_PREFIX ".GoldEngine" // WINEPREFIX (Takes space into the user disk, so not changing it won't make the user cry about the games eating its disk.)
#define WINE_PREFIX_PATH getenv("HOME") // PREFIX PATH (Where the WINEPREFIX ^) will be located.
#define WINE_ARCH wineArch::win64 // Architecture of the WinePrefix, in case users are running 32 bit devices.
#define EXECUTABLE_NAME "GoldEngine-Internal.exe" // Name of your executable, can put a route in there and will work anyway.
