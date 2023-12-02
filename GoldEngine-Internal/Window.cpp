#define RAYGUI_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#define SUPPORT_FILEFORMAT_HDR

#include "Includes.h"
#include "ScriptSandbox.h"
#include "CastToNative.h"
#include "DataPacks.h"
#include "Drawing.h"

using namespace System;

namespace Engine
{
	public ref class Window
	{
	private: 
		static int tickRate = 0;
		static int physicsTick = 60;
		static int targetFPS = 60;
		static ArrayList drawList;

	public:
		void SetWindowFlags(unsigned int flags)
		{
			SetConfigFlags(flags);
		}

	public:
		void OpenWindow(int width, int height, System::String^ name)
		{
			DataPacks::BeginDataPacks();
			InitWindow(width, height, CastToNative(name));
			rlImGuiSetup(true);
		}

	public:
		void AddToDraw(System::Action^ invokable)
		{
			drawList.Add(invokable);
		}

	public:
		void ResetPhysics()
		{
			tickRate = 0;
		}

	public:
		void SetFPS(int fps)
		{
			targetFPS = fps;
		}

	public:
		int GetFPS()
		{
			return targetFPS;
		}

	public:
		virtual void Exit()
		{

		}

	public:
		void Draw(System::Action<int,int>^ executionStack)
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
	};
}