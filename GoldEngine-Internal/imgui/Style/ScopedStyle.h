#pragma once

class ScopedStyle
{
private:
	ImGuiStyle originalStyle;

public:
	ScopedStyle();
	~ScopedStyle();

	void Set(const ImGuiStyle&);
	void Reset();
};