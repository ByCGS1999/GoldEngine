#include "Transform.h"
#include "Vector3.h"
#include "Vector2.h"

using namespace Engine::Internal;

Engine::Components::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

RAYLIB::Vector2 Engine::Components::Vector2::toNative()
{
	return { x, y };
}