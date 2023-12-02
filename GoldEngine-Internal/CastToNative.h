#define WIN32_LEAN_AND_MEAN 

const char* CastToNative(System::String^ value);

template <class returnal, class input>
returnal CastToNativeType(input^ value);