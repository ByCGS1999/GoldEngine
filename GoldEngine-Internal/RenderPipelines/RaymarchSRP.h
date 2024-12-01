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