#include "Includes.h"
#include "GlIncludes.h"
#include "Time.h"

using namespace Engine::Scripting;

void Time::Wait(double sleepTime)
{
	return RAYLIB::WaitTime(sleepTime);
}
