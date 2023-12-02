#pragma once

using namespace System;
using namespace System::Collections;

namespace Engine
{
	public ref class Window
	{
	private:
		ArrayList drawList;

	public:
		void SetWindowFlags(unsigned int flags);
		void OpenWindow(int width, int height, System::String^ name);
		void OpenWindow(int width, int height, const char* name);
		void AddToDraw(System::Action^ invokable);
		void ResetPhysics();
		void SetFPS(int fps);
		int GetFPS();
		void Loop();
		virtual void Exit();
		virtual void Draw();
		virtual void Update();
	};
}