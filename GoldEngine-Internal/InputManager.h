#pragma once

namespace Engine::Scripting
{
	public ref class InputManager
	{
	public:
		static bool IsKeyPressed(int key_id)
		{
			return ::IsKeyPressed(key_id);
		}

		static bool IsKeyDown(int key_id)
		{
			return ::IsKeyDown(key_id);
		}

		static bool IsKeyReleased(int key_id)
		{
			return ::IsKeyReleased(key_id);
		}

		static bool IsKeyPressedRepeat(int key_id)
		{
			return ::IsKeyPressedRepeat(key_id);
		}

		static Engine::Internal::Components::Vector2^ GetMousePosition()
		{
			return gcnew Engine::Internal::Components::Vector2(GetMousePosition()->x, GetMousePosition()->y);
		}

		static bool MouseButton1Up()
		{
			return IsMouseButtonUp(MOUSE_BUTTON_LEFT);
		}

		static bool MouseButton2Up()
		{
			return IsMouseButtonUp(MOUSE_BUTTON_RIGHT);
		}

		static bool MouseButton1Down()
		{
			return IsMouseButtonDown(MOUSE_BUTTON_LEFT);
		}

		static bool MouseButton2Down()
		{
			return IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
		}

		static bool MouseButton1Pressed()
		{
			return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
		}

		static bool MouseButton2Pressed()
		{
			return IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
		}
	};
}