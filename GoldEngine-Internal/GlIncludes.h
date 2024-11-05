#pragma once

#pragma managed(push, off)

#include "Raylib/include/x64/raylib.h"

namespace RAYMATH
{
#include "Raylib/include/x64/raymath.h"
}

namespace RLGL
{
#include "Raylib/include/x64/rlgl.h"
}

#include "Raylib/include/raygui.h"
#include "Raylib/include/rlImGui.h"
#include "Raylib/include/rlights.h"
#include "Raylib/include/glad.h"

#ifndef GLIMPL
#define GLIMPL
#include "Raylib/include/rPBR.h"
#endif

#pragma managed(pop)