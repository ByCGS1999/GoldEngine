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
	SetFileAttributes((LPCWSTR)path, (DWORD)ptr);
}