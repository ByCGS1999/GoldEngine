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

Color Components::Vector3::toColor()
{
	return GetColor(x + y + z);
}

::Vector3 Components::Vector3::toNative()
{
	return { x,y,z };
}


void Components::Vector3::Add(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void Components::Vector3::Add(Vector3^ origin)
{
	this->x += origin->x;
	this->y += origin->y;
	this->z += origin->z;
}

void Components::Vector3::Multiply(Vector3^ origin)
{
	this->x *= origin->x;
	this->y *= origin->y;
	this->z *= origin->z;
}

void Components::Vector3::Multiply(float x, float y, float z)
{
	this->x *= x;
	this->y *= y;
	this->z *= z;
}

void Components::Vector3::Divide(Vector3^ origin)
{
	this->x /= origin->x;
	this->y /= origin->y;
	this->z /= origin->z;
}

void Components::Vector3::Divide(float x, float y, float z)
{
	this->x /= x;
	this->y /= y;
	this->z /= z;
}

void Components::Vector3::Sub(float x, float y, float z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
}

void Components::Vector3::Sub(Vector3^ origin)
{
	this->x -= origin->x;
	this->y -= origin->y;
	this->z -= origin->z;
}
