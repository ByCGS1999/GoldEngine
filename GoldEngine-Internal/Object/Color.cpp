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
	__int8 rByte;
	__int8 gByte;
	__int8 bByte;
	__int8 aByte;

	rByte = (hexColor >> 0) & 0xFF;
	gByte = (hexColor >> 8) & 0xFF;
	bByte = (hexColor >> 16) & 0xFF;
	aByte = (hexColor >> 24) & 0xFF;

	RAYLIB::Color color = {};

	color.r = rByte;
	color.g = gByte;
	color.b = bByte;
	color.a = aByte;

	return color;
}

RAYLIB::Vector3 Engine::Components::Color::toNativeVector3()
{
	RAYLIB::Color c = toNative();
	RAYLIB::Vector3 vector = { 0 };

	vector.x = (c.r / 255.0f);
	vector.y = (c.g / 255.0f);
	vector.z = (c.b / 255.0f);

	return vector;
}

float* Engine::Components::Color::toFloat()
{
	float *args = new float[4];
	RAYLIB::Color c = GetColor(hexColor);
	args[0] = c.r;
	args[1] = c.g;
	args[2] = c.b;
	args[3] = c.a;

	return args;
}