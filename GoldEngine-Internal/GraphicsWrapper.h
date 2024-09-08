#pragma once

namespace Engine::Internal
{
    namespace rlWrapper
    {
        public value struct Texture
        {
        public:
            unsigned int id;
            int width;
            int height;
            int mipmaps;
            int format;


            static rlWrapper::Texture FromUnmanaged(const RAYLIB::Texture unmanagedTexture)
            {
                rlWrapper::Texture tex;

                tex.id = unmanagedTexture.id;
                tex.width = unmanagedTexture.width;
                tex.height = unmanagedTexture.height;
                tex.mipmaps = unmanagedTexture.mipmaps;
                tex.format = unmanagedTexture.format;

                return tex;
            }

            static void ApplyToUnmanaged(RAYLIB::Texture originalTexture, const rlWrapper::Texture managedTexture)
            {
                originalTexture.id = managedTexture.id;
                originalTexture.width = managedTexture.width;
                originalTexture.height = managedTexture.height;
                originalTexture.mipmaps = managedTexture.mipmaps;
                originalTexture.format = managedTexture.format;
            }
        };

        public value struct RenderTexture 
        {
        public:
            unsigned int id;
            rlWrapper::Texture texture;
            rlWrapper::Texture depth;


            static RAYLIB::Texture convertToNative(rlWrapper::Texture tex)
            {
                RAYLIB::Texture texture = { 0 };

                rlWrapper::Texture::ApplyToUnmanaged(texture, tex);

                return texture;
            }

            static rlWrapper::RenderTexture FromUnmanaged(const RAYLIB::RenderTexture unmanagedTexture)
            {
                rlWrapper::RenderTexture renderTexture;

                renderTexture.id = unmanagedTexture.id;
                renderTexture.texture = rlWrapper::Texture::FromUnmanaged(unmanagedTexture.texture);
                renderTexture.depth = rlWrapper::Texture::FromUnmanaged(unmanagedTexture.depth);

                return renderTexture;
            }

            static void ApplyToUnmanaged(RAYLIB::RenderTexture originalTexture, const rlWrapper::RenderTexture managedTexture)
            {
                originalTexture.id = managedTexture.id;
                rlWrapper::Texture::ApplyToUnmanaged(originalTexture.texture, managedTexture.texture);
                rlWrapper::Texture::ApplyToUnmanaged(originalTexture.depth, managedTexture.depth);
            }
        };

        public enum class ShaderLocs
        {
            SHADER_LOC_VERTEX_POSITION = 0, // Shader location: vertex attribute: position
            SHADER_LOC_VERTEX_TEXCOORD01,   // Shader location: vertex attribute: texcoord01
            SHADER_LOC_VERTEX_TEXCOORD02,   // Shader location: vertex attribute: texcoord02
            SHADER_LOC_VERTEX_NORMAL,       // Shader location: vertex attribute: normal
            SHADER_LOC_VERTEX_TANGENT,      // Shader location: vertex attribute: tangent
            SHADER_LOC_VERTEX_COLOR,        // Shader location: vertex attribute: color
            SHADER_LOC_MATRIX_MVP,          // Shader location: matrix uniform: model-view-projection
            SHADER_LOC_MATRIX_VIEW,         // Shader location: matrix uniform: view ( Camera transform)
            SHADER_LOC_MATRIX_PROJECTION,   // Shader location: matrix uniform: projection
            SHADER_LOC_MATRIX_MODEL,        // Shader location: matrix uniform: model (transform)
            SHADER_LOC_MATRIX_NORMAL,       // Shader location: matrix uniform: normal
            SHADER_LOC_VECTOR_VIEW,         // Shader location: vector uniform: view
            SHADER_LOC_COLOR_DIFFUSE,       // Shader location: vector uniform: diffuse color
            SHADER_LOC_COLOR_SPECULAR,      // Shader location: vector uniform: specular color
            SHADER_LOC_COLOR_AMBIENT,       // Shader location: vector uniform: ambient color
            SHADER_LOC_MAP_ALBEDO,          // Shader location: sampler2d texture: albedo (same as: SHADER_LOC_MAP_DIFFUSE)
            SHADER_LOC_MAP_METALNESS,       // Shader location: sampler2d texture: metalness (same as: SHADER_LOC_MAP_SPECULAR)
            SHADER_LOC_MAP_NORMAL,          // Shader location: sampler2d texture: normal
            SHADER_LOC_MAP_ROUGHNESS,       // Shader location: sampler2d texture: roughness
            SHADER_LOC_MAP_OCCLUSION,       // Shader location: sampler2d texture: occlusion
            SHADER_LOC_MAP_EMISSION,        // Shader location: sampler2d texture: emission
            SHADER_LOC_MAP_HEIGHT,          // Shader location: sampler2d texture: height
            SHADER_LOC_MAP_CUBEMAP,         // Shader location: samplerCube texture: cubemap
            SHADER_LOC_MAP_IRRADIANCE,      // Shader location: samplerCube texture: irradiance
            SHADER_LOC_MAP_PREFILTER,       // Shader location: samplerCube texture: prefilter
            SHADER_LOC_MAP_BRDF             // Shader location: sampler2d texture: brdf
        };

        public enum class UniformType
        {
            SHADER_UNIFORM_FLOAT = 0,       // Shader uniform type: float
            SHADER_UNIFORM_VEC2,            // Shader uniform type: vec2 (2 float)
            SHADER_UNIFORM_VEC3,            // Shader uniform type: vec3 (3 float)
            SHADER_UNIFORM_VEC4,            // Shader uniform type: vec4 (4 float)
            SHADER_UNIFORM_INT,             // Shader uniform type: int
            SHADER_UNIFORM_IVEC2,           // Shader uniform type: ivec2 (2 int)
            SHADER_UNIFORM_IVEC3,           // Shader uniform type: ivec3 (3 int)
            SHADER_UNIFORM_IVEC4,           // Shader uniform type: ivec4 (4 int)
            SHADER_UNIFORM_SAMPLER2D        // Shader uniform type: sampler2d
        };
    }


	/// <summary>
	/// This class its not memory safe.
    /// All the methods are raw and the user is expected to manage the memory by its own. Allocating and deallocating resources when needed.
	/// </summary>
	public ref class GraphicsWrapper abstract
	{
	public:
        
        /* -- SETTERS -- */

		static void GL_SetShaderValue(RAYLIB::Shader, int, void*, rlWrapper::UniformType);
        static void GL_SetShaderValue(RAYLIB::Shader, rlWrapper::ShaderLocs, void*, rlWrapper::UniformType);

        /*-- MODES --*/

        static void GL_BeginDrawing();
        static void GL_BeginTextureMode(RAYLIB::RenderTexture);
        static void GL_BeginShaderMode(RAYLIB::Shader);

        static void GL_EndTextureMode();
        static void GL_EndShaderMode();
        static void GL_EndDrawing();

        /* -- LOADERS -- */

        static RAYLIB::Model GL_LoadModel(String^);
        static RAYLIB::RenderTexture GL_LoadRenderTexture(int, int);
        static RAYLIB::Texture GL_LoadTexture(String^);
        static RAYLIB::Shader GL_LoadShader(String^, String^);
        static RAYLIB::Shader GL_LoadShaderFromMemory(String^, String^);
        static void GL_LoadDepthTexture(RAYLIB::RenderTexture, int, int, bool);

        /* -- GETTERS -- */

        static int  GL_GetShaderLoc(RAYLIB::Shader, const char*);
        static RAYLIB::RenderTexture GL_GetDepthTexture(int, int);

        /* -- GRAPHICS -- */

        static void GL_ClearBackground(Engine::Components::Color^);
        static void GL_DrawModel(RAYLIB::Model, Engine::Components::Vector3^, float, Engine::Components::Color^);
        static void GL_DrawTexture(RAYLIB::Texture, Engine::Components::Vector2^, Engine::Components::Color^);

        /* -- UNLOADERS -- */
        static void GL_UnloadTexture(RAYLIB::Texture);
        static void GL_UnloadModel(RAYLIB::Model);
        static void GL_UnloadRenderTexture(RAYLIB::RenderTexture);
	};
}