#pragma once
public class WinAPI
{
public:
	static void MBOX(void* winHandle, const char* contents, const char* owner, unsigned int flags);
	static void FreeCons();
	static void AllocCons();
	static void SetWindowStatus(void* winHandle, int windowId);
	static void SetAttribute(const char* path, int ptr);
};

