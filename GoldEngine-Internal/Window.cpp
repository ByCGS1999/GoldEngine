#include "Window.h"

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