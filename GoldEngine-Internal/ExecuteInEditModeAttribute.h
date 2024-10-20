#pragma once

namespace Engine::Scripting
{
	[System::AttributeUsageAttribute(System::AttributeTargets::Method)]
	public ref class ExecuteInEditModeAttribute : System::Attribute
	{

	};
}