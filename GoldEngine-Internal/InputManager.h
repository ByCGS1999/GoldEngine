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
	};
}