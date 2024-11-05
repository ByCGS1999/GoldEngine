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
	ImGui::GetStyle() = style;
}

void ScopedStyle::Reset()
{
	ImGui::GetStyle() = this->originalStyle;
}