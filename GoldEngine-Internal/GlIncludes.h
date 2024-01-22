#pragma once


#include "Raylib/include/raylib.h"
#include "Raylib/include/raymath.h"
#include "Raylib/include/rlgl.h"
#include "Raylib/include/raygui.h"
#include "Raylib/include/rlImGui.h"

#ifndef GLIMPL
#define GLIMPL
#include "Raylib/include/rPBR.h"
#else
#define rPBR_IMPL true
#include "Raylib/include/rPBR.h"
#endif