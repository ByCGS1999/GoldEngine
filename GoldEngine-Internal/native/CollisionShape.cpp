#include "../SDK.h"
#include "CollisionShape.h"
#include "../Objects/Physics/CollisionType.h"
#include "../Objects/Physics/Native/NativePhysicsService.h"
#include "../Objects/Physics/RigidBody.h"
#include "../Objects/Physics/PhysicsService.h"

using namespace Engine::Native;
using namespace System::Runtime::InteropServices;

void onCollisionShapeDeleted(btCollisionShape* shape)
{
	delete shape;
}

void onCollisionObjectDeleted(btCollisionObject* object)
{
	delete object;
}

CollisionShape::CollisionShape(Engine::Internal::Components::GameObject^ userPtr)
{
	this->handle = GCHandle::Alloc(userPtr);
	this->userHandler = static_cast<void*>(GCHandle::ToIntPtr(handle).ToPointer());
	this->collisionShape = NULL;
	this->collisionObject = NULL;
}

CollisionShape::~CollisionShape()
{
	this->handle.Free();
}

void CollisionShape::createCollisionShape(btCollisionShape* shape)
{
	this->collisionShape = new Engine::Native::EnginePtr<btCollisionShape*>(shape, &onCollisionShapeDeleted);

	/*
	if (this->collisionShape == nullptr)
		this->collisionShape = shape;
	else
	{
		delete this->collisionShape;
		this->collisionShape = shape;
	}
	*/
}

void CollisionShape::createBulletObject()
{
	/*
	if(this->collisionObject == nullptr)
		this->collisionObject = new btCollisionObject();
	*/

	try
	{
		this->collisionObject = new Engine::Native::EnginePtr<btCollisionObject*>(new btCollisionObject(), &onCollisionObjectDeleted);

		this->collisionObject->getInstance()->setCollisionShape(this->collisionShape->getInstance());
		this->collisionObject->getInstance()->setUserPointer(this->userHandler);

		Singleton<Engine::EngineObjects::Physics::PhysicsService^>::Instance->AddCollisionObject(getCollisonObject());
	}
	catch (Exception^ ex)
	{

	}
}

btCollisionObject* CollisionShape::getCollisonObject()
{
	return this->collisionObject->getInstance();
}

btCollisionShape* CollisionShape::getCollisionShape()
{
	return this->collisionShape->getInstance();
}
