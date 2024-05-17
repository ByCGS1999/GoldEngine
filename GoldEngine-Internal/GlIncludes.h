#pragma once

#ifdef _WIN64
#include "Raylib/include/x64/raylib.h"
#include "Raylib/include/x64/raymath.h"
#include "Raylib/include/x64/rlgl.h"
#else
#include "Raylib/include/x86/raylib.h"
#include "Raylib/include/x86/raymath.h"
#include "Raylib/include/x86/rlgl.h"
#endif

#include "Raylib/include/raygui.h"
#include "Raylib/include/rlImGui.h"
#include "Raylib/include/rlights.h"

#ifndef GLIMPL
#define GLIMPL
#include "Raylib/include/rPBR.h"
#endif