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
		catch (System::InvalidCastException^ ex)
		{
			return T();
		}
	};


	template <typename T>
	static T toNativeType()
	{
		return nullptr;
	}


	static System::Object^ Deserialzable(System::Object^ object, System::Type^ type)
	{
		System::String^ serializedData = Serialize(object);

		return (System::Object^)Deserialize(serializedData, type);
	}
};