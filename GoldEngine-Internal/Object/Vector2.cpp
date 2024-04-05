#include "Transform.h"
#include "Vector3.h"
#include "Vector2.h"

using namespace Engine::Internal;

Components::Vector2::Vector2(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2 Components::Vector2::toNative()
{
	return { x, y };
}