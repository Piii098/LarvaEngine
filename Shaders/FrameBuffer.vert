#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 TexCoords;

uniform float zoom = 1.f;

void main()
{
    gl_Position = vec4(inPosition, 1.0);

    vec2 centeredTexCoord = inTexCoord - vec2(0.5, 0.5);
    TexCoords = centeredTexCoord / zoom + vec2(0.5, 0.5);
}