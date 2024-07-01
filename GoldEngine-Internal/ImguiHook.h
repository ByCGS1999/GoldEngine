#pragma once

#include "GlIncludes.h"

namespace Engine::Scripting
{
	public ref class ImguiHook
	{
	public:
		static bool ImGui_begin_window(String^ name, bool* is_open)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open);
		}

		static bool ImGui_begin_window(String^ name, bool is_open)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), &is_open);
		}

		static bool ImGui_begin_window(String^ name, bool* is_open, ImGuiWindowFlags_ flags)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open, flags);
		}

		static bool ImGui_begin_window(String^ name, bool is_open, ImGuiWindowFlags_ flags)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), &is_open, flags);
		}

		static void ImGui_end_window()
		{
			ImGui::End();
		}

		static void ImGui_text(String^ name)
		{
			ImGui::Text(CastStringToNative(name).c_str());
		}

		static bool ImGui_begin_listbox(String^ listId) 
		{ 
			return ImGui::BeginListBox(CastStringToNative(listId).c_str());
		}

		static bool ImGui_begin_listbox(String^ listId, Engine::Components::Vector2^ vector2)
		{
			return ImGui::BeginListBox(CastStringToNative(listId).c_str(), {vector2->x, vector2->y});
		}

		static void ImGui_end_listbox() { return ImGui::EndListBox(); }

		static bool ImGui_checkbox(String^ name, bool* checked)
		{
			return ImGui::Checkbox(CastStringToNative(name).c_str(), checked);
		}

		static bool ImGui_checkbox(String^ name, bool checked)
		{
			return ImGui::Checkbox(CastStringToNative(name).c_str(), &checked);
		}

		static bool ImGui_button(String^ name)
		{
			return ImGui::Button(CastStringToNative(name).c_str());
		}

		static bool ImGui_button(String^ name, Engine::Components::Vector2^ size)
		{
			return ImGui::Button(CastStringToNative(name).c_str(), {size->x, size->y});
		}

		static Engine::Components::Vector2^ ImGui_get_windowsize()
		{
			ImVec2& vec = ImGui::GetWindowSize();

			return gcnew Engine::Components::Vector2(vec.x, vec.y);
		}
	};
}