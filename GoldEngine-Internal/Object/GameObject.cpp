#include "../Includes.h"
#include "../GlIncludes.h"
#include "../CastToNative.h"
#include "ObjectType.h"
#include "ViewSpace.h"
#include "../Cast.h"
#include "Layer.h"
#include "LayerManager.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Quaternion.h"
#include "../ExecuteInEditModeAttribute.h"
#include "../LoggingAPI.h"
#include "../EngineState.h"
#include "../Event.h"
#include "Transform.h"
#include "GameObject.h"

using namespace Engine::Internal::Components;

// BUBBLING \\

void GameObject::descendantAdded(GameObject^ descendant)
{
	if (this->transform->parent != nullptr)
	{
		((GameObject^)transform->GetParent()->getGameObject())->onDescendantAdded->raiseExecution(gcnew cli::array<GameObject^> { descendant });
	}
}

// CTOR \\

GameObject::GameObject(System::String^ n, Engine::Internal::Components::Transform^ transform, Engine::Internal::Components::ObjectType t, String^ tag, Engine::Components::Layer^ layer)
{
	this->active = true;
	this->memberIsProtected = false;
	this->name = n;
	this->transform = transform;
	this->type = t;
	this->viewSpace = ViewSpace::V3D;

	this->lastTransform = nullptr;
	layerMask = layer;

	if (tag == nullptr)
		tag = "";

	if (this->transform != nullptr)
		this->transform->setReference(this);

	this->tag = tag;

	// EVENT CREATION \\

	this->onPropertyChanged = gcnew Engine::Scripting::Events::Event();
	this->onChildAdded = gcnew Engine::Scripting::Events::Event();
	this->onChildRemoved = gcnew Engine::Scripting::Events::Event();
	this->onDescendantAdded = gcnew Engine::Scripting::Events::Event();

	this->onDescendantAdded->connect(gcnew Action<GameObject^>(this, &GameObject::descendantAdded));
}

void GameObject::setParent(GameObject^ object)
{
	if (object == nullptr)
		return;

	if (this->transform->GetParent() != nullptr)
		((GameObject^)this->transform->GetParent()->getGameObject())->onChildRemoved->raiseExecution(gcnew cli::array<System::Object^> { this });

	transform->setParent(object->transform);

	object->onChildAdded->raiseExecution(gcnew cli::array<System::Object^> { this });
	object->onDescendantAdded->raiseExecution(gcnew cli::array<System::Object^> { this });
}

Engine::Internal::Components::Transform^ GameObject::getTransform()
{
	return transform;
}

String^ GameObject::getTag()
{
	return tag;
}

void GameObject::setTag(String^ tag)
{
	this->tag = tag;
}

void GameObject::GameUpdate()
{
	OnPropChanged();

	UpdateLocalPosition();
	UpdatePosition();

	if (!active)
		return;

	HookUpdate();

	if (EngineState::PlayMode == false)
	{
		auto method = GetType()->GetMethod("Update");

		if (!method->IsDefined(Engine::Scripting::ExecuteInEditModeAttribute::typeid, false))
		{
			return;
		}
	}

	Update();
}

void GameObject::GameDraw()
{
	if (!active)
		return;

	Draw();
}

void GameObject::GameDrawGizmos()
{
	if (!active)
		return;

	DrawGizmo();
}


void GameObject::GameDrawImGUI()
{
	if (!active)
		return;

	if (!EngineState::PlayMode)
	{
		auto method = GetType()->GetMethod("DrawImGUI");

		if (!method->IsDefined(Engine::Scripting::ExecuteInEditModeAttribute::typeid, false))
		{
			return;
		}
	}

	DrawImGUI();
}

generic <class T>
T GameObject::ToObjectType()
{
	try
	{
		return Cast::Dynamic<T>(this);
	}
	catch (Exception^ ex)
	{
		printError(ex->Message);
	}
}