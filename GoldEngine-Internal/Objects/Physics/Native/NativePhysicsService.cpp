#include "../../../SDK.h"
#include "NativePhysicsService.h"


using namespace Engine::EngineObjects::Physics::Native;


// unmanaged code... 
// bs in general...
#pragma managed(push, off)

typedef enum NativeCollisionType
{
	Concave,
	Convex,
	BoundingBox
};

btCollisionShape* createCollisionMesh(RAYLIB::Mesh mesh, int collisionType)
{
	btCollisionShape* collisionShape = nullptr;

	if (collisionType == (int)NativeCollisionType::Convex)
	{
		collisionShape = new btConvexHullShape();

		for (int i = 0; i < mesh.vertexCount; i++)
		{
			float x = mesh.vertices[i * 3];
			float y = mesh.vertices[i * 3 + 1];
			float z = mesh.vertices[i * 3 + 2];

			((btConvexHullShape*)collisionShape)->addPoint(btVector3(x, y, z), false);
		}

		((btConvexHullShape*)collisionShape)->recalcLocalAabb();
	}


	return collisionShape;
}

#pragma managed(pop)


btCollisionShape* NativePhysicsService::getCollisionShapeFromID(unsigned int modelId, unsigned int meshId, int collisionType)
{
	RAYLIB::Mesh mesh = (Mesh)DataPacks::singleton().GetModel(modelId).meshes[meshId];

	btCollisionShape* collisionShape = createCollisionMesh(mesh, (int)collisionType);

	return collisionShape;
}

btCollisionShape* NativePhysicsService::getCollisionShapeFromMesh(RAYLIB::Mesh mesh, int collisionType)
{
	btCollisionShape* collisionShape = createCollisionMesh(mesh, (int)collisionType);
	return collisionShape;
}


NativePhysicsService::NativePhysicsService()
{
	//NativeSingleton<NativePhysicsService>::Create(this);
}