#pragma once

#pragma managed(push, off)
#include <cstdio>
#include <vector>

namespace rPBR
{
    using namespace rPBR;

    // Light type
    typedef enum {
        LIGHT_DIRECTIONAL = 0,
        LIGHT_POINT = 1,
        LIGHT_SPOT = 2
    } LightType;

    // Light data
    typedef struct {
        int type;
        int enabled;
        RAYLIB::Vector3 position;
        RAYLIB::Vector3 target;
        float color[4];
        float intensity;
        float cutOff;
        float outerCutOff;

        // Shader light parameters locations
        int typeLoc;
        int enabledLoc;
        int positionLoc;
        int targetLoc;
        int colorLoc;
        int intensityLoc;
        int cutoffLoc;
        int outercutoffLoc;

        int lightId;
    } Light;

    //----------------------------------------------------------------------------------
    // Global Variables Definition
    //----------------------------------------------------------------------------------
    static int lightCount = 0;     // Current number of dynamic lights that have been created

    //----------------------------------------------------------------------------------
    // Module specific Functions Declaration
    //----------------------------------------------------------------------------------
    // Create a light and get shader locations

    static void UpdateLight(Shader& shader, Light& light);
    static Light CreateLight(int type, RAYLIB::Vector3 position, RAYLIB::Vector3 target, RAYLIB::Color color, float intensity, float cutoff, float outerCutoff, Shader& shader);
    static Light ReInstantiateLight(int type, RAYLIB::Vector3 position, RAYLIB::Vector3 target, RAYLIB::Color color, float intensity, float cutoff, float outerCutoff, Shader& shader, unsigned int lightId);
    static void SetAmbientColor(Shader& shader, RAYLIB::Vector3* color, float* intensity);
    static void ReorganizeLights(std::vector<Light>);

    static Light CreateLight(int type, RAYLIB::Vector3 position, RAYLIB::Vector3 target, RAYLIB::Color color, float intensity , float cutoff, float outerCutoff, Shader& shader)
    {
        Light light = { 0 };

            light.enabled = 1;
            light.type = type;
            light.position = position;
            light.target = target;
            light.color[0] = (float)color.r / 255.0f;
            light.color[1] = (float)color.g / 255.0f;
            light.color[2] = (float)color.b / 255.0f;
            light.color[3] = (float)color.a / 255.0f;
            light.cutOff = cutoff;
            light.outerCutOff = outerCutoff;
            light.intensity = intensity;

            // NOTE: Shader parameters names for lights must match the requested ones
            light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightCount));
            light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightCount));
            light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightCount));
            light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightCount));
            light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightCount));
            light.intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", lightCount));
            light.cutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].cutOff", lightCount));
            light.outercutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].outerCutOff", lightCount));

            light.lightId = lightCount;

            printf("[Shader Debugger]: Light ID -> %d\n", light.lightId);
            printf("[Shader Debugger]: enabledLoc -> %d\n", light.enabledLoc);
            printf("[Shader Debugger]: typeLoc -> %d\n", light.typeLoc);
            printf("[Shader Debugger]: positionLoc -> %d\n", light.positionLoc);
            printf("[Shader Debugger]: targetLoc -> %d\n", light.targetLoc);
            printf("[Shader Debugger]: colorLoc -> %d\n", light.colorLoc);
            printf("[Shader Debugger]: intensityLoc -> %d\n", light.intensityLoc);
            printf("-------------------------------------------\n");

            UpdateLight(shader, light);

            lightCount++;

        return light;
    }

    static Light ReInstantiateLight(int type, RAYLIB::Vector3 position, RAYLIB::Vector3 target, RAYLIB::Color color, float intensity, float cutoff, float outerCutoff, Shader& shader, unsigned int lightId)
    {
        Light light = { 0 };

        light.enabled = 1;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color[0] = (float)color.r / 255.0f;
        light.color[1] = (float)color.g / 255.0f;
        light.color[2] = (float)color.b / 255.0f;
        light.color[3] = (float)color.a / 255.0f;
        light.intensity = intensity;
        light.cutOff = cutoff;
        light.outerCutOff = outerCutoff;

        // NOTE: Shader parameters names for lights must match the requested ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightId));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightId));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightId));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightId));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightId));
        light.intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", lightId));
        light.cutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].cutOff", lightCount));
        light.outercutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].outerCutOff", lightCount));

        light.lightId = lightId;

        printf("Reinstantiating Light\n");

        printf("[Shader Debugger]: Light ID -> %d\n", light.lightId);
        printf("[Shader Debugger]: enabledLoc -> %d\n", light.enabledLoc);
        printf("[Shader Debugger]: typeLoc -> %d\n", light.typeLoc);
        printf("[Shader Debugger]: positionLoc -> %d\n", light.positionLoc);
        printf("[Shader Debugger]: targetLoc -> %d\n", light.targetLoc);
        printf("[Shader Debugger]: colorLoc -> %d\n", light.colorLoc);
        printf("[Shader Debugger]: intensityLoc -> %d\n", light.intensityLoc);
        printf("-------------------------------------------\n");

        UpdateLight(shader, light);

        return light;
    }

    // Send light properties to shader
    // NOTE: Light shader locations should be available
    static void UpdateLight(Shader& shader, Light& light)
    {
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", light.lightId));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", light.lightId));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", light.lightId));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", light.lightId));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", light.lightId));
        light.intensityLoc = GetShaderLocation(shader, TextFormat("lights[%i].intensity", light.lightId));
        light.cutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].cutOff", lightCount));
        light.outercutoffLoc = GetShaderLocation(shader, TextFormat("lights[%i].outerCutOff", lightCount));
        
        SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
        SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

        // Send to shader light position values
        float position[3] = { light.position.x, light.position.y, light.position.z };
        SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

        // Send to shader light target position values
        float target[3] = { light.target.x, light.target.y, light.target.z };
        SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, light.colorLoc, light.color, SHADER_UNIFORM_VEC4);
        SetShaderValue(shader, light.intensityLoc, &light.intensity, SHADER_UNIFORM_FLOAT);

        SetShaderValue(shader, light.cutoffLoc, &light.cutOff, SHADER_UNIFORM_FLOAT);
        SetShaderValue(shader, light.outercutoffLoc, &light.outerCutOff, SHADER_UNIFORM_FLOAT);
    }

    static void SetAmbientColor(RAYLIB::Shader& shader,  RAYLIB::Vector3* col, float* intensity)
    {
        SetShaderValue(shader, GetShaderLocation(shader, "ambientColor"), col, SHADER_UNIFORM_VEC3);
        SetShaderValue(shader, GetShaderLocation(shader, "ambient"), intensity, SHADER_UNIFORM_FLOAT);
    }

    static void ReorganizeLights(std::vector<Light> light)
    {
        int lastIndex = 0;

        for (int x = 0; x < light.size(); x++)
        {
            light[x].lightId = x;
            lastIndex++;
        }

        lightCount = lastIndex;
    }
}

#pragma managed(pop)