#pragma once

#ifdef USE_BULLET_PHYS

#pragma managed(push, off)
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#pragma managed(pop)

namespace Engine::EngineObjects::Physics::Native
{
	private class NativePhysicsService
	{
	public:
		NativePhysicsService();

		btCollisionShape* getCollisionShapeFromID(unsigned int modelId, unsigned int meshId, int collisionType);
		btCollisionShape* getCollisionShapeFromMesh(RAYLIB::Mesh mesh, int collisionType);

		btCollisionShape* getCollisionShapeForBox(float x, float y, float z);
		btCollisionShape* getCollisionShapeForSphere(float radius);
	};
}

#endif