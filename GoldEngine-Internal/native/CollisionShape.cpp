#include "../SDK.h"
#include "CollisionShape.h"
#include "../Objects/Physics/CollisionType.h"
#include "../Objects/Physics/Native/NativePhysicsService.h"
#include "../Objects/Physics/RigidBody.h"
#include "../Objects/Physics/PhysicsService.h"

using namespace Engine::Native;
using namespace System::Runtime::InteropServices;

CollisionShape::CollisionShape(Engine::Internal::Components::GameObject^ userPtr)
{
	this->handle = GCHandle::Alloc(userPtr);
	this->userHandler = static_cast<void*>(GCHandle::ToIntPtr(handle).ToPointer());
	this->collisionShape = nullptr;
}

CollisionShape::~CollisionShape()
{
	this->handle.Free();
}

void CollisionShape::createCollisionShape(btCollisionShape* shape)
{
	if (this->collisionShape == nullptr)
		this->collisionShape = shape;
	else
	{
		delete this->collisionShape;
		this->collisionShape = shape;
	}
}

void CollisionShape::createBulletObject()
{
	if(this->collisionObject == nullptr)
		this->collisionObject = new btCollisionObject();

	this->collisionObject->setCollisionShape(this->collisionShape);
	this->collisionObject->setUserPointer(this->userHandler);

	Singleton<Engine::EngineObjects::Physics::PhysicsService^>::Instance->AddCollisionObject(getCollisonObject());
}

btCollisionObject* CollisionShape::getCollisonObject()
{
	return this->collisionObject;
}

btCollisionShape* CollisionShape::getCollisionShape()
{
	return this->collisionShape;
}
