#pragma once

#pragma managed(push, off)
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#pragma managed(pop)

namespace Engine::EngineObjects::Physics
{
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class PhysicsService : public Engine::EngineObjects::Script
	{
	private:
		btDefaultCollisionConfiguration* collisionConfig;
		btCollisionDispatcher* collisionDispatcher;
		btBroadphaseInterface* bvhInterface;
		btSequentialImpulseConstraintSolver* SQCsolver;
		btDiscreteDynamicsWorld* world;

	public:
		Engine::Components::Vector3^ Gravity;

	public:
		PhysicsService(String^ name, Engine::Internal::Components::Transform^ transform);

	public:
		void Start() override;
		void Update() override;


		void AddPhysicsObject();
	};
}