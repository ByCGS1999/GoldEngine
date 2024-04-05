#include "Transform.h"
#include "Vector3.h"

using namespace Engine::Internal;

Components::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Components::Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Components::Vector3::Set(float* v)
{
	this->x = v[0];
	this->y = v[1];
	this->z = v[2];
}