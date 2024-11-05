#pragma once

using namespace System::Collections::Generic;

public ref class SceneFormat
{
public:
	String^ sceneName;

	List<Engine::Internal::Components::GameObject^>^ sceneObjects;
};

