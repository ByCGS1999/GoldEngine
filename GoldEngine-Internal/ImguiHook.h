#pragma once

#include "GlIncludes.h"

namespace Engine::Scripting
{
	public ref class ImGuiHook
	{
	public:
		static void ImGui_PopStyleVar(int count)
		{
			return ImGui::PopStyleVar(count);
		}

		static void ImGui_PopStyleColor(int count)
		{
			return ImGui::PopStyleColor(count);
		}

		static void ImGui_PushStyleVar(ImGuiStyleVar_ styleVar, float val)
		{
			return ImGui::PushStyleVar((int)styleVar, val);
		}

		static void ImGui_PushStyleVar(ImGuiStyleVar_ styleVar, Engine::Components::Vector2^ val)
		{
			return ImGui::PushStyleVar((int)styleVar, {val->x, val->y});
		}

		static void ImGui_PushStyleVar(int idx, unsigned int val)
		{
			return ImGui::PushStyleColor(idx, val);
		}

		static void ImGui_PushStyleVar(int idx, Engine::Components::Quaternion^ val)
		{
			return ImGui::PushStyleColor(idx, { val->x, val->y, val->z, val->w });
		}

		static bool ImGui_Begin(String^ name, bool* is_open)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open);
		}

		static bool ImGui_Begin(String^ name, bool is_open)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), &is_open);
		}

		static bool ImGui_Begin(String^ name, bool* is_open, ImGuiWindowFlags_ flags)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), is_open, flags);
		}

		static bool ImGui_Begin(String^ name, bool is_open, ImGuiWindowFlags_ flags)
		{
			return ImGui::Begin(CastStringToNative(name).c_str(), &is_open, flags);
		}

		static void ImGui_End()
		{
			ImGui::End();
		}

		static void ImGui_Text(String^ name)
		{
			ImGui::Text(CastStringToNative(name).c_str());
		}

		static bool ImGui_BeginListbox(String^ listId) 
		{ 
			return ImGui::BeginListBox(CastStringToNative(listId).c_str());
		}

		static bool ImGui_BeginListbox(String^ listId, Engine::Components::Vector2^ vector2)
		{
			return ImGui::BeginListBox(CastStringToNative(listId).c_str(), {vector2->x, vector2->y});
		}

		static void ImGui_EndListbox() { return ImGui::EndListBox(); }

		static bool ImGui_Checkbox(String^ name, bool* checked)
		{
			return ImGui::Checkbox(CastStringToNative(name).c_str(), checked);
		}

		static bool ImGui_Checkbox(String^ name, bool checked)
		{
			return ImGui::Checkbox(CastStringToNative(name).c_str(), &checked);
		}

		static bool ImGui_Button(String^ name)
		{
			return ImGui::Button(CastStringToNative(name).c_str());
		}

		static bool ImGui_Button(String^ name, Engine::Components::Vector2^ size)
		{
			return ImGui::Button(CastStringToNative(name).c_str(), {size->x, size->y});
		}

		static Engine::Components::Vector2^ ImGui_GetWindowSize()
		{
			ImVec2& vec = ImGui::GetWindowSize();

			return gcnew Engine::Components::Vector2(vec.x, vec.y);
		}

		static Engine::Components::Vector2^ ImGui_GetWindowPos()
		{
			ImVec2& vec = ImGui::GetWindowPos();

			return gcnew Engine::Components::Vector2(vec.x, vec.y);
		}
	};
}