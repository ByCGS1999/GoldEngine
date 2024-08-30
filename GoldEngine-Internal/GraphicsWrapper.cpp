#include "GlIncludes.h"
#include "GraphicsWrapper.h"

using namespace Engine::Internal;


void GraphicsWrapper::GL_SetShaderValue(void* shader, int loc, void* value, UniformType flags)
{
	RAYLIB::SetShaderValue((*(RAYLIB::Shader*)shader), loc, value, (int)flags);
}

void GraphicsWrapper::GL_SetShaderValue(void* shader, ShaderLocs loc, void* value, UniformType flags)
{
	RAYLIB::SetShaderValue((*(RAYLIB::Shader*)shader), (int)loc, value, (int)flags);
}


int GraphicsWrapper::GL_GetShaderLoc(void* shader, const char* loc_name)
{
	return RAYLIB::GetShaderLocation(*((RAYLIB::Shader*)shader), loc_name);
}