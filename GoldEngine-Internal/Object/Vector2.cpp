#include "../Includes.h"
#include "../ManagedIncludes.h"
#include "../GlIncludes.h"
#include "Vector2.h"

Engine::Components::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Engine::Components::Vector2::Vector2()
{
	this->x = 0.0f;
	this->y = 0.0f;
}

RAYLIB::Vector2 Engine::Components::Vector2::toNative()
{
	return { x, y };
}
