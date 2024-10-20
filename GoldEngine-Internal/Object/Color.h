#pragma once

namespace Engine::Components
{
	[System::SerializableAttribute]
	[Newtonsoft::Json::JsonObjectAttribute]
	[MoonSharp::Interpreter::MoonSharpUserDataAttribute]
	public ref class Color
	{
	public:
		unsigned int hexColor;

	public:
		[Newtonsoft::Json::JsonConstructorAttribute]
		Color(unsigned int colorHex);
		unsigned int toHex();
		RAYLIB::Color toNativeAlt();
		RAYLIB::Color toNative();
		RAYLIB::Vector3 toNativeVector3();
		float* toFloat();

		static Color^ New(unsigned int hexColor)
		{
			return gcnew Color(hexColor);
		}
	};
}