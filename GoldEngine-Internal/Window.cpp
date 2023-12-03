#include "Window.h"

#include "Includes.h"
#include "ScriptSandbox.h"
#include "CastToNative.h"
#include "DataPacks.h"
#include "Drawing.h"
#include "CypherLib.h"


int tickRate = 0;
int physicsTick = 60;
int targetFPS = 60;

void Engine::Window::SetWindowFlags(unsigned int flags)
{
	SetConfigFlags(flags);
}

void Engine::Window::OpenWindow(int width, int height, System::String^ name)
{
	InitWindow(width, height, CastToNative(name));
	rlImGuiSetup(true);
}

void Engine::Window::OpenWindow(int width, int height, const char* name)
{
	InitWindow(width, height, name);
	rlImGuiSetup(true);
}

void Engine::Window::AddToDraw(System::Action^ invokable)
{
	drawList.Add(invokable);
}

void Engine::Window::ResetPhysics()
{
	tickRate = 0;
}

void Engine::Window::SetFPS(int fps)
{
	targetFPS = fps;
}

int Engine::Window::GetFPS()
{
	return targetFPS;
}

void Engine::Window::Exit()
{

}

void Engine::Window::Loop()
{
	SetTargetFPS(targetFPS);

	unsigned int passwd = CypherLib::GetPasswordBytes(ENCRYPTION_PASSWORD);
	String^ encryptedContents = CypherLib::EncryptFileContents("Mel kai shi, sai ei chmu", passwd);
	String^ decryptedContents = CypherLib::DecryptFileContents(encryptedContents, passwd);
	Console::WriteLine(encryptedContents);
	Console::WriteLine(decryptedContents);

	while (!WindowShouldClose())
	{
		Update();
		Draw();
	}

	rlImGuiShutdown();
	Exit();
	CloseWindow();
}

void Engine::Window::Draw()
{

}

void Engine::Window::Update()
{

}

void Engine::Window::Preload()
{

}