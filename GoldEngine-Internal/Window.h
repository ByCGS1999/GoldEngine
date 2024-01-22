#pragma once

#include "Includes.h"
#include "CastToNative.h"
#include "DataPacks.h"
#include "Drawing.h"
#include "CypherLib.h"

using namespace System;
using namespace System::Collections;

namespace Engine
{
	public ref class Window
	{
	private:

		int tickRate = 0;
		int physicsTick = 60;
		int targetFPS = 60;

		ArrayList drawList;

	public:
		bool FirstTimeBoot()
		{
			return !System::IO::File::Exists("./Data/UserData/firstInit.asset");
		}

		void Boot()
		{
			System::IO::File::Create("./Data/UserData/firstInit.asset")->Close();
		}

		void SetWindowFlags(unsigned int flags)
		{
			SetConfigFlags(flags);
		}

		void OpenWindow(int width, int height, System::String^ name)
		{
			InitWindow(width, height, CastToNative(name));
			rlImGuiSetup(true);
		}

		void OpenWindow(int width, int height, const char* name)
		{
			InitWindow(width, height, name);
			rlImGuiSetup(true);
		}

		void AddToDraw(System::Action^ invokable)
		{
			drawList.Add(invokable);
		}

		void ResetPhysics()
		{
			tickRate = 0;
		}

		void SetFPS(int fps)
		{
			targetFPS = fps;
		}

		int GetFPS()
		{
			return targetFPS;
		}

		void Loop()
		{
			SetTargetFPS(targetFPS);

			while (!WindowShouldClose())
			{
				Update();
				Draw();
			}

			rlImGuiShutdown();
			Exit();
			CloseWindow();
		}

		// vmethods
		virtual void Preload() { } // loading assets
		virtual void Init() {} // Initialization (spawning objects, scene loading, whatever.)
		virtual void Exit() {} // exit
		virtual void Draw() {} // draw
		virtual void Update() {} // update 
		virtual void DrawImGui() {}
	};
}