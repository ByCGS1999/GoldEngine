#include "Transform.h"
#include "../ManagedIncludes.h"
#include "../Event.h"
#include "GameObject.h"
#include "../SDK.h"

using namespace System;
using namespace Engine::Internal::Components;

Engine::Internal::Components::Transform::Transform(Engine::Components::Vector3^ position, Engine::Components::Vector3^ rotation, Engine::Components::Vector3^ scale, Transform^ parent)
{
	this->uid = System::Guid::NewGuid().ToString();
	this->localPosition = position;
	this->localRotation = rotation;
	this->scale = scale;

	if (parent != nullptr)
	{
		this->parent = parent;
		this->position = Engine::Components::Vector3::add(parent->position, this->localPosition);
		this->rotation = Engine::Components::Vector3::add(parent->rotation, this->localRotation);

		this->localPosition = Engine::Components::Vector3::sub(parent->position, this->position);
	}
	else
	{
		this->position = this->localPosition;
		this->rotation = this->localRotation;
	}
}

String^ Engine::Internal::Components::Transform::GetUID()
{
	return uid;
}


void Engine::Internal::Components::Transform::SetUID(String^ uid)
{
	this->uid = uid;
}

void Engine::Internal::Components::Transform::setParent(Transform^ newTransform)
{
	this->parent = newTransform;
}

Engine::Internal::Components::Transform^ Engine::Internal::Components::Transform::getParent()
{
	return parent;
}

generic <class T>
T Engine::Internal::Components::Transform::GetObject()
{
	return (T)Singleton<Engine::Scripting::ObjectManager^>::Instance->GetObjectByUid(this->uid);
}

System::Object^ Engine::Internal::Components::Transform::GetObject()
{
	return Singleton<Engine::Scripting::ObjectManager^>::Instance->GetObjectByUid(this->uid);
}