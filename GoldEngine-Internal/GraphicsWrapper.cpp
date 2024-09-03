#include "Includes.h"
#include "CastToNative.h"
#include "GlIncludes.h"
#include "LoggingAPI.h"
#include "GraphicsWrapper.h"

using namespace Engine::Internal;

void* GraphicsWrapper::GL_GetDepthTexture(int width, int height)
{
	RenderTexture2D* target = &LoadRenderTexture(width, height);;

	target->id = rlLoadFramebuffer();

	if (target->id > 0)
	{
		rlEnableFramebuffer(target->id);


		target->texture.id = rlLoadTexture(NULL, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
		target->texture.width = width;
		target->texture.height = height;
		target->texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
		target->texture.mipmaps = 1;

		target->depth.id = rlLoadTextureDepth(width, height, false);
		target->depth.width = width;
		target->depth.height = height;
		target->depth.format = 19;       //DEPTH_COMPONENT_24BIT?
		target->depth.mipmaps = 1;

		rlFramebufferAttach(target->id, target->texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
		rlFramebufferAttach(target->id, target->depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_TEXTURE2D, 0);

		if (rlFramebufferComplete(target->id)) 
			printWarning(gcnew String(TextFormat("FBO: [ID %i] Framebuffer object created successfully", target->id)));

		rlDisableFramebuffer();
	}
	else 
		printWarning(gcnew String("FBO: Framebuffer object can not be created"));

	return target;
}

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

Model GraphicsWrapper::GL_LoadModel(String^ fileName)
{
	return RAYLIB::LoadModel(CastStringToNative(fileName).c_str());
}