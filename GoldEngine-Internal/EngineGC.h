#pragma once

// MAXIMUM SIZE OF GC COLLECTOR (AFTER THIS THRESHOLD OF MB IS EXCEEDED GC WILL BE CALLED TO ATTEMPT FREEING MEMORY)
#define MAX_GC_SIZE 256 // IN MB
#define GC_MEMORY_INCREMENT 128

namespace Engine::GC
{
	private ref class EngineGC
	{
	private:
		static bool GC_TRIGGERED_LASTFRAME;

	public:
		static int GC_SIZE = MAX_GC_SIZE;


	public:
		static void Update();
	};
}