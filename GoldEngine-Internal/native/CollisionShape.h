#pragma once

#pragma managed(push, off)
#include <btBulletCollisionCommon.h>

namespace Engine::Native
{
	class CollisionShape
	{
	private:
		btCollisionShape* collisionShape;
		btCollisionObject* collisionObject;
		System::Runtime::InteropServices::GCHandle handle;
		void* userHandler;

	public:
		CollisionShape(Engine::Internal::Components::GameObject^);
		~CollisionShape();

		void createCollisionShape(btCollisionShape* shape);
		void createBulletObject();

		btCollisionObject* getCollisonObject();
		btCollisionShape* getCollisionShape();


	};
}

#pragma managed(pop)