#include "Window.h"

ref class EntryPoint : Engine::Window
{
public:
	EntryPoint()
	{
		Start();
	}


	void Start()
	{
		SetWindowFlags(4096 | 4);
		OpenWindow(640, 480, "Raylib Window");
		SetFPS(60);

		Loop();
	}

	void Draw() override
	{

	}

	void Update() override
	{

	}
};


int main()
{
	gcnew EntryPoint();
}