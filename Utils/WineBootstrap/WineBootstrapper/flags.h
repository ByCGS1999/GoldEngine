#pragma once

#define NOT_FOUND_EXIT 32512
#define OK_EXIT 0
#define FORCE_EXIT 2
#define NORMAL_EXIT 256

bool prefixExists = false;
bool prefixInstalled = false;
bool dependenciesInstalled = false;

bool hasMonoInstalled = true;

std::string prefixPath = "";
std::string basePrefix = "";

// installer flags

bool useUnattendedInstall = false;
bool useGameLauncher = true;