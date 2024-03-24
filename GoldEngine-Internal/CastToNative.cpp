#define WIN32_LEAN_AND_MEAN 
#include "CastToNative.h"
#include <string>
#include <atlbase.h>
#include <atlconv.h>
#include <msclr/marshal.h>
#include <msclr/lock.h>


const char* CastToNative(System::String^ value)
{
	msclr::interop::marshal_context ctx;
	return ctx.marshal_as<const char*>(value);
}

std::string CastStringToNative(System::String^ value)
{
	System::Text::Encoding^ u8 = System::Text::Encoding::UTF8;
	array<unsigned char>^ bytes = u8->GetBytes(value);
	pin_ptr<unsigned char> pinnedPtr = &bytes[0];
	return std::string((char*)pinnedPtr);
}

template <class returnal, class input>
returnal CastToNativeType(input value)
{
	msclr::interop::marshal_context ctx;
	return ctx.marshal_as<returnal>(value);
}