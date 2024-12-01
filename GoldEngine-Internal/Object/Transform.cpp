#include "Transform.h"
#include "../Event.h"
#include "GameObject.h"

using namespace System;
using namespace Engine::Internal::Components;

Engine::Internal::Components::Transform::Transform(Engine::Components::Vector3^ position, Engine::Components::Vector3^ rotation, Engine::Components::Vector3^ scale, Transform^ parent)
{
	this->uid = System::Guid::NewGuid().ToString();
	this->localPosition = position;
	this->localRotation = rotation;
	this->scale = scale;
	this->gameObject = gameObject;

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

void Engine::Internal::Components::Transform::setParent(Transform^ newTransform)
{
	this->parent = newTransform;
}

void Engine::Internal::Components::Transform::setReference(System::Object^ gameObject)
{
	//this->gameObject = (Engine::Internal::Components::GameObject^)gameObject;
	this->gameObject = gameObject;
}

System::Object^ Engine::Internal::Components::Transform::getGameObject()
{
	//return (Engine::Internal::Components::GameObject^)this->gameObject;
	return this->gameObject;
}

Engine::Internal::Components::Transform^ Engine::Internal::Components::Transform::getParent()
{
	return parent;
}