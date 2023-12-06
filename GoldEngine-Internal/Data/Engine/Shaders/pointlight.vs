#version 330

layout(location = 0) in vec3 inPosition;  // Vertex position attribute
layout(location = 1) in vec3 inNormal;    // Vertex normal attribute

out vec3 lightDirection;  // Output light direction to fragment shader
out float lightForce;     // Output light force to fragment shader
out vec2 UV;              // Output texture coordinates to fragment shader

uniform vec3 lightColor;      // Output light color to fragment shader

void main()
{
    // Pass the vertex position to the fragment shader
    gl_Position = vec4(inPosition, 1.0);

    // Assume a simple light direction in view space
    lightDirection = normalize(vec3(1.0, 1.0, 1.0));

    // Pass the transformed normal to the fragment shader
    // In a more complete setup, you might also pass other lighting-related information here
    UV = inPosition.xy;

    // Pass a constant light force for simplicity
    lightForce = 1.0;
}