#include "Includes.h"
#include "Transform.h"

using namespace System;

namespace Engine::Internal::Components
{
	enum ObjectType
	{
		Generic,
		ModelRenderer,
		Skybox
	};

	public ref class Object
	{
	public:
		String^ name;
		ObjectType type;
		Transform^ transform;

		Object(System::String^ n, Transform^ transform, ObjectType t)
		{
			this->name = n;
			this->transform = transform;
			this->type = t;
		}
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() {}
		virtual void Draw() {}
		Transform^ GetTransform() { return transform; }
	};
}