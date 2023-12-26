#include "Includes.h"
#include "Transform.h"

using namespace System;

namespace Engine::Internal::Components
{
	enum ObjectType
	{
		Datamodel,
		Generic,
		ModelRenderer,
		Skybox,
		LightManager,
		LightSource,
		PBR_ModelRenderer
	};

	public ref class Object
	{
	public:
		String^ name;
		ObjectType type;
		Transform^ transform;
		Object^ parent;

		[[JsonConstructorAttribute]]
		Object(System::String^ n, Transform^ transform, ObjectType t, Object^ parent)
		{
			this->name = n;
			this->transform = transform;
			this->type = t;
			this->parent = parent;
		}
		virtual void Start() {}
		virtual void PhysicsUpdate() {}
		virtual void Update() {}
		virtual void Draw() {}
		virtual void DrawGizmo() {}
		Transform^ GetTransform() { return transform; }
		void SetParent(Object^ object) { parent = object; }
	};
}