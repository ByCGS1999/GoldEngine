#include "../../SDK.h"
#include "PhysicsService.h"

using namespace System;
using namespace Engine::EngineObjects::Physics;

// unmanaged code... 
// bs in general...
#pragma unmanaged
void setGravity(btDiscreteDynamicsWorld* world, float x, float y, float z)
{
	world->setGravity(btVector3(x,y,z));
}
#pragma managed



PhysicsService::PhysicsService(String^ name, Engine::Internal::Components::Transform^ transform) : Engine::EngineObjects::Script(name, transform)
{
	this->protectMember();

	if (this->Gravity == nullptr)
		this->Gravity = gcnew Engine::Components::Vector3();

	collisionConfig = new btDefaultCollisionConfiguration();
	collisionDispatcher = new btCollisionDispatcher(collisionConfig);
	bvhInterface = new btDbvtBroadphase();
	SQCsolver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(collisionDispatcher, bvhInterface, SQCsolver, collisionConfig);

	Singleton<PhysicsService^>::Create(this);
}

void PhysicsService::Start()
{
	if (!attributes->hasAttribute("Gravity"))
		attributes->addAttribute(Engine::Scripting::Attribute::New(Engine::Scripting::AccessLevel::Public, "Gravity", gcnew Engine::Components::Vector3(), Engine::Components::Vector3::typeid));
}

void PhysicsService::Update()
{
	setGravity(world, this->Gravity->x, this->Gravity->y, this->Gravity->z);

	world->stepSimulation((1.0f / 60.0f), 10);
}

void PhysicsService::AddPhysicsObject()
{

}