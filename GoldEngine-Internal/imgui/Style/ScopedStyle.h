#pragma once

private struct ScopedStyle
{
private:
	ImGuiStyle originalStyle;

public:
	ScopedStyle();
	~ScopedStyle();

	void Set(const ImGuiStyle&);
	void Reset();
};