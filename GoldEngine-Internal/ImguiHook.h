#pragma once

#include "GlIncludes.h"

namespace Engine::Scripting
{
	public ref class ImguiHook
	{
	public:
		static bool ImGui_begin(String^ name, bool* is_open)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open);
		}

		static bool ImGui_begin(String^ name, bool* is_open, ImGuiWindowFlags_ flags)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open, flags);
		}

		static void ImGui_end()
		{
			return ImGui::End();
		}

		static void ImGui_text(String^ name)
		{
			return ImGui::Text(CastStringToNative(name).c_str());
		}

		static bool ImGui_checkbox(String^ name, bool* checked)
		{
			return ImGui::Checkbox(CastStringToNative(name).c_str(), checked);
		}
	};
}