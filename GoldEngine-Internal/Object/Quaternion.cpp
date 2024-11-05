#pragma once
#include "../Includes.h"
#include "../GlIncludes.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"

Engine::Components::Vector3^ Engine::Components::Quaternion::toEuler()
{
	return gcnew Engine::Components::Vector3(x, y, z);
}