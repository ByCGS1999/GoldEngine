#include "Window.h"

#include "Includes.h"
#include "ScriptSandbox.h"
#include "CastToNative.h"
#include "DataPacks.h"
#include "Drawing.h"


int tickRate = 0;
int physicsTick = 60;
int targetFPS = 60;

void Engine::Window::SetWindowFlags(unsigned int flags)
{
	SetConfigFlags(flags);
}

void Engine::Window::OpenWindow(int width, int height, System::String^ name)
{
	DataPacks::BeginDataPacks();
	InitWindow(width, height, CastToNative(name));
	rlImGuiSetup(true);
}

void Engine::Window::OpenWindow(int width, int height, const char* name)
{
	DataPacks::BeginDataPacks();
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

void Engine::Window::Draw()
{
	SetTargetFPS(targetFPS);

	while (!WindowShouldClose())
	{
		BeginDrawing();

		for each (System::Object ^ object in drawList)
		{
			System::Action^ invokable = (System::Action^)object;
			invokable->Invoke();
		}

		EndDrawing();

		rlImGuiBegin();
		DrawFPS(0, 0);
		ImGui::ShowDemoWindow((bool*)true);
		rlImGuiEnd();

		tickRate++;
	}

	rlImGuiShutdown();
	Exit();
	CloseWindow();
}

void Engine::Window::Draw(System::Action<int, int>^ executionStack)
{
	SetTargetFPS(targetFPS);

	while (!WindowShouldClose())
	{
		// -- update
		executionStack->Invoke(tickRate, physicsTick);

		BeginDrawing();

		for each (System::Object ^ object in drawList)
		{
			System::Action^ invokable = (System::Action^)object;
			invokable->Invoke();
		}

		EndDrawing();

		rlImGuiBegin();
		DrawFPS(0, 0);
		ImGui::ShowDemoWindow((bool*)true);
		rlImGuiEnd();

		tickRate++;
	}

	rlImGuiShutdown();
	Exit();
	CloseWindow();
}