#pragma once

namespace Engine::Internal::Components
{
	public enum class ObjectType
	{
		Generic = 0,
		Datamodel = 1,
		Skybox = 2,
		ModelRenderer = 3,
		GridRenderer = 4,
		CubeRenderer = 5,
		LightManager = 6,
		LightSource = 7,
		Script = 8,
		BoundingBoxRenderer = 9,
		Daemon = 10
	};
}