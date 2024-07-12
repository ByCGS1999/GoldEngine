#pragma once

/// <summary>
/// Interface for scripts that will be ran on the 'preload' of a scene.
/// Usually these are invoked after scene assets loaded, but before the scene completly loaded
/// </summary>
namespace Engine::Preload
{
	public interface class IPreloadScript
	{
	public:
		static void Preload() {}
		static String^ GetTarget() { return ""; }
	};
}