#pragma once

/// <summary>
/// Architecturally is identical to the EngineObjects.
/// Just that the scripts that inherit from this class will be ran before the game completly loads (Before boot or on scene change)
/// </summary>
namespace Engine::Preload
{
	public ref class PreloadScript
	{
	public:
		virtual void Preload() {}
	};
}