#include "Script.h"

System::TypeCode Engine::EngineObjects::Script::GetTypeCode()
{
    return System::TypeCode();
}

bool Engine::EngineObjects::Script::ToBoolean(System::IFormatProvider^ provider)
{
    return false;
}

wchar_t Engine::EngineObjects::Script::ToChar(System::IFormatProvider^ provider)
{
    return L'\0';
}

signed char Engine::EngineObjects::Script::ToSByte(System::IFormatProvider^ provider)
{
    return 0;
}

unsigned char Engine::EngineObjects::Script::ToByte(System::IFormatProvider^ provider)
{
    return 0;
}

short Engine::EngineObjects::Script::ToInt16(System::IFormatProvider^ provider)
{
    return 0;
}

unsigned short Engine::EngineObjects::Script::ToUInt16(System::IFormatProvider^ provider)
{
    return 0;
}

int Engine::EngineObjects::Script::ToInt32(System::IFormatProvider^ provider)
{
    return 0;
}

unsigned int Engine::EngineObjects::Script::ToUInt32(System::IFormatProvider^ provider)
{
    return 0;
}

long long Engine::EngineObjects::Script::ToInt64(System::IFormatProvider^ provider)
{
    return 0;
}

unsigned long long Engine::EngineObjects::Script::ToUInt64(System::IFormatProvider^ provider)
{
    return 0;
}

float Engine::EngineObjects::Script::ToSingle(System::IFormatProvider^ provider)
{
    return 0.0f;
}

double Engine::EngineObjects::Script::ToDouble(System::IFormatProvider^ provider)
{
    return 0.0;
}

System::Decimal Engine::EngineObjects::Script::ToDecimal(System::IFormatProvider^ provider)
{
    return System::Decimal();
}

System::DateTime Engine::EngineObjects::Script::ToDateTime(System::IFormatProvider^ provider)
{
    return System::DateTime();
}

System::String^ Engine::EngineObjects::Script::ToString(System::IFormatProvider^ provider)
{
    throw gcnew System::NotImplementedException();
    // TODO: Insertar una instrucción "return" aquí
}

System::Object^ Engine::EngineObjects::Script::ToType(System::Type^ conversionType, System::IFormatProvider^ provider)
{
    throw gcnew System::NotImplementedException();
    // TODO: Insertar una instrucción "return" aquí
}
