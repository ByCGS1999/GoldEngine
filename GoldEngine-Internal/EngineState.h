#pragma once

private ref class EngineState
{
public:
#if PRODUCTION_BUILD
	static bool PlayMode = true;
#else
	static bool PlayMode = false;
#endif

	static bool glInitialized = false;
};