#version 430

in vec2 fragTexCoord;
in vec4 fragColor;

layout(location = 0)
uniform sampler2D texture0;

layout(location = 1)
uniform float nearPlane;

layout(location = 2)
uniform float farPlane;

out vec4 finalColor;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0;
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    float depthValue = texture(texture0, fragTexCoord).r;
    float linearDepth = LinearizeDepth(depthValue) / farPlane;
    linearDepth = pow(linearDepth, 1.0 / 8.0);
    finalColor = vec4(vec3(depthValue), 1.0);
}