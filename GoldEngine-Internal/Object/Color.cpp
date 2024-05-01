#include "Color.h"

using namespace System;
using namespace Engine::Components;

Color::Color(unsigned int colorHex)
{
	this->hexColor = colorHex;
}

unsigned int Color::toHex()
{
	return hexColor;
}