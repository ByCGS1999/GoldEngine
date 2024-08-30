#pragma once

namespace Engine::Internal
{
	public ref class GraphicsWrapper abstract
	{
	public:
		enum class ShaderLocs
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

        enum class UniformType
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

	public:
		static void GL_SetShaderValue(void*, int, void*, UniformType);
        static void GL_SetShaderValue(void*, ShaderLocs, void*, UniformType);
        static int GL_GetShaderLoc(void*, const char*);
	};
}