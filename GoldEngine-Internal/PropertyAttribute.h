#pragma once

namespace Engine::Scripting
{
	public ref class PropertyAttribute : System::Attribute
	{
	public:
		Engine::Scripting::AccessLevel accessLevel;
		String^ attributeName;

	public:
		PropertyAttribute(Engine::Scripting::AccessLevel level, String^ name)
		{
			this->accessLevel = level;
			this->attributeName = name;
		}

		PropertyAttribute(Engine::Scripting::AccessLevel level)
		{
			this->accessLevel = level;
			this->attributeName = "";
		}
	};
}