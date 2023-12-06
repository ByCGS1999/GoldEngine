#version 330

in vec3 lightDirection; // source direction
in float lightForce; // force of the light
in vec3 lightColor;

uniform sampler2D texture0; // model texture
in vec2 UV; // fragment uv

out vec4 finalColor; // output

void main()
{
    vec3 normalizedLightDir = normalize(lightDirection);
    vec4 texColor = texture(texture0, UV);

    // Calculate the dot product of the normalized light direction and the surface normal
    float diffuseFactor = max(dot(normalizedLightDir, vec3(0, 0, 1)), 0.0);

    // Apply lighting to the texture color
    vec3 litColor = texColor.rgb * ((lightColor * diffuseFactor) * lightForce);

    finalColor = vec4(litColor, 1.0);
}