#include "Windows.h"
#include <string>
#include "WinAPI.h"

void WinAPI::MBOX(void* winHandle, const char* contents, const char* owner, unsigned int flags)
{
	MessageBoxA((HWND)winHandle, contents, owner, flags);
}

void WinAPI::FreeCons()
{
	FreeConsole();
}