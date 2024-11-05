/// <summary>
/// Default Gold Engine RenderPipeline
/// LitPBR, only contains Lit (Direct Illumination) & PBR Rendering
/// </summary>
#include <memory>

namespace Engine::Render::Pipelines
{
    Shader raymarchShader;
    Shader rasterShader;

    RenderTexture2D depthTexture;

    int screenWidth, screenHeight;

    typedef struct {
        unsigned int camPos, camDir, screenCenter;
    } RayLocs;

    RayLocs marchLocs = {0};

    RenderTexture2D LoadRenderTextureDepthTex(int width, int height)
    {
        RenderTexture2D target = { 0 };

        target.id = RLGL::rlLoadFramebuffer(); // Load an empty framebuffer

        if (target.id > 0)
        {
            RLGL::rlEnableFramebuffer(target.id);

            // Create color texture (default to RGBA)
            target.texture.id = RLGL::rlLoadTexture(0, width, height, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
            target.texture.width = width;
            target.texture.height = height;
            target.texture.format = PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
            target.texture.mipmaps = 1;

            // Create depth texture buffer (instead of raylib default renderbuffer)
            target.depth.id = RLGL::rlLoadTextureDepth(width, height, false);
            target.depth.width = width;
            target.depth.height = height;
            target.depth.format = 19;       //DEPTH_COMPONENT_24BIT?
            target.depth.mipmaps = 1;

            // Attach color texture and depth texture to FBO
            RLGL::rlFramebufferAttach(target.id, target.texture.id, RLGL::RL_ATTACHMENT_COLOR_CHANNEL0, RLGL::RL_ATTACHMENT_TEXTURE2D, 0);
            RLGL::rlFramebufferAttach(target.id, target.depth.id, RLGL::RL_ATTACHMENT_DEPTH, RLGL::RL_ATTACHMENT_TEXTURE2D, 0);

            // Check if fbo is complete with attachments (valid)
            if (RLGL::rlFramebufferComplete(target.id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

            RLGL::rlDisableFramebuffer();
        }
        else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

        return target;
    }

    // Unload render texture from GPU memory (VRAM)
    void UnloadRenderTextureDepthTex(RenderTexture2D target)
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

    public ref class RaymarchSRP : Engine::Render::ScriptableRenderPipeline
    {
    public:
        RaymarchSRP() : Engine::Render::ScriptableRenderPipeline()
        {
            raymarchShader = LoadShader(0, "Data/Engine/Shaders/Raymarcher/raymarcher.fs");
            rasterShader = LoadShader(0, "Data/Engine/Shaders/Raymarcher/raster.fs");

            marchLocs.camPos = GetShaderLocation(raymarchShader, "camPos");
            marchLocs.camDir = GetShaderLocation(raymarchShader, "camDir");
            marchLocs.screenCenter = GetShaderLocation(raymarchShader, "screenCenter");

            screenWidth = GetScreenWidth();
            screenHeight = GetScreenHeight();

            RAYLIB::Vector2 screenCenter = { screenWidth / 2.0f, screenHeight / 2.0f };
            SetShaderValue(raymarchShader, marchLocs.screenCenter, &screenCenter, SHADER_UNIFORM_VEC2);

            depthTexture = LoadRenderTextureDepthTex(screenWidth, screenHeight);
        }

    public:
        void OnUnloadPipeline() override
        {
            UnloadShader(raymarchShader);
            UnloadShader(rasterShader);

            UnloadRenderTextureDepthTex(depthTexture);
        }

        void PreRenderFrame() override
        {
            // raymarch the scene
            BeginTextureMode(depthTexture);

            RLGL::rlEnableDepthTest();

            BeginShaderMode(raymarchShader);
                DrawRectangleRec({ 0,0, (float)screenWidth, (float)screenHeight }, RAYLIB::WHITE);
            EndShaderMode();
        }

        void PostRenderFrame() override
        {
            EndTextureMode();
        }

        void PreRenderObjects() override
        {
            BeginShaderMode(rasterShader);
        }

        void PostRenderObjects() override
        {
            EndShaderMode();
            ClearBackground(RAYLIB::WHITE);
        }

        void OnRenderBegin() override
        {
            // hook camera position and direction.
            RAYLIB::Vector3 cameraPosition, cameraDirection;

            Engine::EngineObjects::Camera^ camera = Singleton<ObjectManager^>::Instance->GetFirstObjectOfType<Engine::EngineObjects::Camera^>();

            if (camera->is3DCamera())
            {
                auto newCamera = (Engine::EngineObjects::Camera3D^)camera;

                float camDist = (1.0f / (tanf(
                    ((Engine::EngineObjects::Native::NativeCamera3D*)newCamera->get())->get().fovy * 0.5f * DEG2RAD)));

                cameraPosition =
                    ((Engine::EngineObjects::Native::NativeCamera3D*)newCamera->get())
                    ->get().position;
                cameraDirection = RAYMATH::Vector3Scale(RAYMATH::Vector3Normalize(RAYMATH::Vector3Subtract(
                    ((Engine::EngineObjects::Native::NativeCamera3D*)newCamera->get())
                    ->get().target,
                    ((Engine::EngineObjects::Native::NativeCamera3D*)newCamera->get())
                    ->get().position)), camDist);

                SetShaderValue(raymarchShader, marchLocs.camPos, &cameraPosition, RLGL::RL_SHADER_UNIFORM_VEC3);
                SetShaderValue(raymarchShader, marchLocs.camDir, &cameraDirection, RLGL::RL_SHADER_UNIFORM_VEC3);
            }
        }

        void OnRenderEnd() override
        {
            DrawTextureRec(depthTexture.texture, { 0, 0, (float)screenWidth, (float)-screenHeight }, { 0,0 }, RAYLIB::WHITE);
        }

        void PreRenderObject(Engine::Internal::Components::GameObject^ engineObject) override
        {

        }

        void PostRenderObject() override
        {

        }

        void PreFirstPassRender(Engine::Management::Scene^ handle) override
        {
            
        }

        void PostFirstPassRender() override
        {

        }
    };
}