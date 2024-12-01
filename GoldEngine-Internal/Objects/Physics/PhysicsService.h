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
		Engine::EngineObjects::Physics::Native::NativePhysicsService* nativePhysicsService;
		btDiscreteDynamicsWorld* world;

	public:
		Engine::Components::Vector3^ Gravity;
		[Engine::Scripting::PropertyAttribute(Engine::Scripting::AccessLevel::Public)]
		float frameRate;
		[Engine::Scripting::PropertyAttribute(Engine::Scripting::AccessLevel::Public)]
		int maxSubSteps;

	public:
		PhysicsService(String^ name, Engine::Internal::Components::Transform^ transform);

	public:
		void Start() override;

		[Engine::Scripting::ExecuteInEditModeAttribute]
		void Update() override;

		void AddPhysicsObject(Engine::EngineObjects::Physics::RigidBody^);
		void RemovePhysicsObject(Engine::EngineObjects::Physics::RigidBody^);

		void AddCollisionObject(btCollisionObject*);
		void RemoveCollisionObject(btCollisionObject*);

		Engine::EngineObjects::Physics::Native::NativePhysicsService* getNativePhysicsService();
	};
}