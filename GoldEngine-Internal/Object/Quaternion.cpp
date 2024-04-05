#pragma once
#include "Transform.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

Engine::Internal::Components::Vector3^ Engine::Internal::Components::Quaternion::toEuler()
{
	return gcnew Engine::Internal::Components::Vector3(x, y, z);
}