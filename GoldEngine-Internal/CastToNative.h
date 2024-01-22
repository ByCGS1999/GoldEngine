#define WIN32_LEAN_AND_MEAN 
#include <string>

const char* CastToNative(System::String^ value);

std::string CastStringToNative(System::String^ value);

template <class returnal, class input>
returnal CastToNativeType(input value);