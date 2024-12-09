#include "../../SDK.h"
#include "../../Screen.h"
#include "ScriptableRenderPipeline.hpp"

using namespace Engine::Scripting;
using namespace Engine::Components;

void Engine::Render::UnloadRenderTextureDepthTex(RenderTexture2D target)
{
	if (target.id > 0)
	{
		// Color texture attached to FBO is deleted
		RLGL::rlUnloadTexture(target.texture.id);
		RLGL::rlUnloadTexture(target.depth.id);

		// NOTE: Depth texture is automatically
		// queried and deleted before deleting framebuffer
		RLGL::rlUnloadFramebuffer(target.id);
	}
}

void Engine::Render::unloadCameraData(cameraData* data)
{
	delete data;
}