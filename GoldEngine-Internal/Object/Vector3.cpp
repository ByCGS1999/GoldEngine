#include "Transform.h"
#include "Vector3.h"

using namespace Engine::Internal;
using namespace Newtonsoft::Json;

[[JsonConstructorAttribute]]
Engine::Components::Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Engine::Components::Vector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Engine::Components::Vector3::Set(float* v)
{
	this->x = v[0];
	this->y = v[1];
	this->z = v[2];
}

Color Engine::Components::Vector3::toColor()
{
	return GetColor(x + y + z);
}

RAYLIB::Vector3 Engine::Components::Vector3::toNative()
{
	return { x,y,z };
}


void Engine::Components::Vector3::add(float x, float y, float z)
{
	this->x += x;
	this->y += y;
	this->z += z;
}

void Engine::Components::Vector3::add(Vector3^ origin)
{
	this->x += origin->x;
	this->y += origin->y;
	this->z += origin->z;
}

void Engine::Components::Vector3::multiply(Vector3^ origin)
{
	this->x *= origin->x;
	this->y *= origin->y;
	this->z *= origin->z;
}

void Engine::Components::Vector3::multiply(float x, float y, float z)
{
	this->x *= x;
	this->y *= y;
	this->z *= z;
}

void Engine::Components::Vector3::divide(Vector3^ origin)
{
	this->x /= origin->x;
	this->y /= origin->y;
	this->z /= origin->z;
}

void Engine::Components::Vector3::divide(float x, float y, float z)
{
	this->x /= x;
	this->y /= y;
	this->z /= z;
}

void Engine::Components::Vector3::sub(float x, float y, float z)
{
	this->x -= x;
	this->y -= y;
	this->z -= z;
}

void Engine::Components::Vector3::sub(Vector3^ origin)
{
	this->x -= origin->x;
	this->y -= origin->y;
	this->z -= origin->z;
}

void Engine::Components::Vector3::copy(const Vector3^ origin)
{
	this->x = origin->x;
	this->y = origin->y;
	this->z = origin->z;
}