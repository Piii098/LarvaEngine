#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D normalBuffer;
uniform sampler2D brightBuffer;

void main()
{
    FragColor = texture(normalBuffer, TexCoords);
    BrightColor = texture(brightBuffer, TexCoords);

}