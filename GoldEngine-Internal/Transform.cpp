#include "Transform.h"

Engine::Internal::Components::Transform::Transform(Vector3^ position, Quaternion^ rotation, float scale)
{
	this->position = position;
	this->rotation = rotation;
	this->scale = scale;
}