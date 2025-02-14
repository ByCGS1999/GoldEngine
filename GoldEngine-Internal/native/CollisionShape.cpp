#include "../SDK.h"

#ifdef USE_BULLET_PHYS

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

void newThread(btCollisionObject* collisionObjectPointer)
{
	while (!Singleton<Engine::EngineObjects::Physics::PhysicsService^>::Instantiated)
	{
		using namespace std::chrono_literals;
		std::this_thread::sleep_for(1000ms);
	}

	Singleton<Engine::EngineObjects::Physics::PhysicsService^>::Instance->AddCollisionObject(collisionObjectPointer);

	std::this_thread::yield();
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
	if (this->collisionShape == nullptr)
		this->collisionShape = new Engine::Native::EnginePtr<btCollisionShape*>(shape, &onCollisionShapeDeleted, &onCollisionShapeDeleted);
	else
	{
		this->collisionShape->setInstance(shape);
	}
}

void CollisionShape::createBulletObject()
{
	try
	{
		this->collisionObject = new Engine::Native::EnginePtr<btCollisionObject*>(new btCollisionObject(), &onCollisionObjectDeleted);

		this->collisionObject->getInstance()->setCollisionShape(this->collisionShape->getInstance());
		this->collisionObject->getInstance()->setUserPointer(this->userHandler);

		Singleton<Engine::EngineObjects::Physics::PhysicsService^>::Instance->AddCollisionObject(getCollisonObject());
	}
	catch (NullReferenceException^ nullException)
	{
		// launch a separate thread and wait for the PhysicsService to be initialized
		std::thread t1(newThread, getCollisonObject());
	}
	catch (Exception^ ex)
	{
		printError(ex->Message);
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

#endif