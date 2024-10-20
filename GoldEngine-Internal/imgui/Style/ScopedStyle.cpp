#include "../imgui.h"
#include "ScopedStyle.h"

ScopedStyle::ScopedStyle()
{
	this->originalStyle = ImGui::GetStyle();
}

ScopedStyle::~ScopedStyle()
{
	ImGui::GetStyle() = originalStyle;
}

void ScopedStyle::Set(const ImGuiStyle& style)
{
	ImGuiStyle& styleRef = ImGui::GetStyle();
	styleRef = style;
}

void ScopedStyle::Reset()
{
	ImGuiStyle& styleRef = ImGui::GetStyle();
	styleRef = this->originalStyle;
}