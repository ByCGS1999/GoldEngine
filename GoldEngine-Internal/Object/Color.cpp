#include "../GlIncludes.h"
#include "Color.h"

using namespace System;
using namespace Engine::Components;

Engine::Components::Color::Color(unsigned int colorHex)
{
	this->hexColor = colorHex;
}

unsigned int Engine::Components::Color::toHex()
{
	return hexColor;
}

RAYLIB::Color Engine::Components::Color::toNative()
{
	return GetColor(hexColor);
}

float* Engine::Components::Color::toFloat()
{
	float *args = new float[3];
	RAYLIB::Color c = GetColor(hexColor);
	args[0] = c.r;
	args[1] = c.g;
	args[2] = c.b;

	return args;
}