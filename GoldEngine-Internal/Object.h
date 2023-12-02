#include "Includes.h"
#include "Transform.h"

using namespace System;

namespace Engine::Internal::Components
{
	public ref class Object
	{
	public:
		String^ name;
	public:
		Transform^ transform;

	public:
		Object(System::String^ n, Transform^ transform)
		{
			this->name = n;
			this->transform = transform;
		}
	};
}