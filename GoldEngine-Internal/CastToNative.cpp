#define WIN32_LEAN_AND_MEAN 
#include "CastToNative.h"
#include <string>
#include <atlbase.h>
#include <atlconv.h>
#include <msclr/marshal.h>


const char* CastToNative(System::String^ value)
{
	return msclr::interop::marshal_context().marshal_as<const char*>(value);
}

template <class returnal, class input>
returnal CastToNativeType(input value)
{
	return msclr::interop::marshal_context().marshal_as<returnal>(value);
}