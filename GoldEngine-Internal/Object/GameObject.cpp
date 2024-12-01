#include "../Includes.h"
#include "../ManagedIncludes.h"
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
#include "../SDK.h"
#include "../Objects/Physics/CollisionType.h"
#include "../Objects/Physics/Native/NativePhysicsService.h"
#include "../Objects/Physics/RigidBody.h"
#include "../Objects/Physics/PhysicsService.h"
#include "../native/CollisionShape.h"
#include "../ObjectManager.h"

using namespace Engine::Internal::Components;

// UNMANAGED FUNCTIONS \\

#pragma managed(push, off)

btCollisionShape* createBoxShape(std::array<float, 3> bounds)
{
	return new btBoxShape({ bounds[0], bounds[1], bounds[2] });
}


#pragma managed(pop)

Engine::Native::CollisionShape* getCollider(GameObject^ inst)
{
	return (Engine::Native::CollisionShape*)inst->getCollisionShape();
}

// BUBBLING \\

void GameObject::descendantAdded(GameObject^ descendant)
{
	if (this->transform->parent != nullptr)
	{
		String^ TUID = transform->GetParent()->GetUID();

		Singleton<Engine::Scripting::ObjectManager^>::Instance->GetObjectByUid(TUID)->onDescendantAdded->raiseExecution(gcnew cli::array<GameObject^> { descendant });
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

	this->collisionShape = new Engine::Native::CollisionShape(this);

	getCollider(this)->createCollisionShape(createBoxShape({1,1,1}));

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
	{
		String^ TUID = this->transform->GetParent()->GetUID();
		Singleton<Engine::Scripting::ObjectManager^>::Instance->GetObjectByUid(TUID)->onChildRemoved->raiseExecution(gcnew cli::array<System::Object^> { this });
	}

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

void GameObject::OnPropChanged()
{
	if (lastTransform == nullptr)
	{
		lastTransform = gcnew Engine::Internal::Components::Transform(transform->position, transform->rotation, transform->scale, transform->parent);
		return;
	}

	if (!transform->position->Equals(lastTransform->position))
	{
		onPropertyChanged->raiseExecution(gcnew cli::array<System::Object^> { "position", transform->position, lastTransform->position });

		lastTransform = gcnew Engine::Internal::Components::Transform(transform->position, transform->rotation, transform->scale, transform->parent);
	}
	else if (!transform->rotation->Equals(lastTransform->rotation))
	{
		onPropertyChanged->raiseExecution(gcnew cli::array<System::Object^> { "rotation", transform->rotation, lastTransform->rotation });

		lastTransform = gcnew Engine::Internal::Components::Transform(transform->position, transform->rotation, transform->scale, transform->parent);
	}
	else if (!transform->scale->Equals(lastTransform->scale))
	{
		onPropertyChanged->raiseExecution(gcnew cli::array<System::Object^> { "scale", transform->scale, lastTransform->scale });

		lastTransform = gcnew Engine::Internal::Components::Transform(transform->position, transform->rotation, transform->scale, transform->parent);
	}
	else if ((lastTransform != nullptr && lastTransform->parent != nullptr) && (transform->parent != nullptr) && (!transform->parent->GetUID()->Equals(lastTransform->parent->GetUID())))
	{
		onPropertyChanged->raiseExecution(gcnew cli::array<System::Object^> { "parent", transform->parent, lastTransform->parent });

		lastTransform = gcnew Engine::Internal::Components::Transform(transform->position, transform->rotation, transform->scale, transform->parent);
	}
}

void GameObject::GameUpdate()
{
	OnPropChanged();

	UpdateLocalPosition();
	UpdatePosition();

	if (!active)
	{
		OnUnactive();
		return;
	}

	OnActive();

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

System::Collections::Generic::List<GameObject^>^ GameObject::GetChildren()
{
	return Singleton<Engine::Scripting::ObjectManager^>::Instance->GetChildrenOf(this);
}

GameObject^ GameObject::GetChild(int index)
{
	try
	{
		return Singleton<Engine::Scripting::ObjectManager^>::Instance->GetChildrenOf(this)[index];
	}
	catch(Exception^ ex)
	{
		printError(ex->Message);
		return nullptr;
	}
}

GameObject^ GameObject::GetChild(String^ childName)
{
	auto children = GetChildren();

	for each (GameObject^ child in children)
	{
		if (child->name == childName)
			return child;
	}

	return nullptr;
}