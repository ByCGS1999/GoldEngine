#pragma once

public ref class Cast
{
public:
	generic <class T>
	static T Dynamic(System::Object^ value)
	{
		try
		{
			return safe_cast<T>(value);
		}
		catch (System::InvalidCastException^)
		{
			return T();
		}
	};
};