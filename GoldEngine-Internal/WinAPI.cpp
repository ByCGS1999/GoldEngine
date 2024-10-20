#include "Windows.h"
#include <string>
#include "WinAPI.h"

void WinAPI::MBOXA(void* winHandle, const char* contents, const char* owner, unsigned int flags)
{
	MessageBoxA((HWND)winHandle, contents, owner, flags);
}

void WinAPI::FreeCons()
{
	FreeConsole();
}

void WinAPI::AllocCons()
{
	AllocConsole();
}

void WinAPI::SetWindowStatus(void* winHandle, int windowId)
{
	ShowWindow((HWND)winHandle, windowId);
}

void WinAPI::SetAttribute(const char* path, int ptr)
{
#if _DEBUG
	SetFileAttributes((LPCWSTR)path, (DWORD)ptr);
#else
	SetFileAttributesA((LPCSTR)path, (DWORD)ptr);
#endif
}