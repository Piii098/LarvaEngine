#version 330 core

layout(location = 0) in vec3 inPosition; // ���_�̍��W
layout(location = 1) in vec3 inNormal; // ���_�̖@��
layout(location = 2) in vec2 inTexCoord;

uniform mat4 uWorldTransform;

out vec2 fragTexCoord;

void main()
{
    gl_Position = vec4(inPosition, 1.0) * uWorldTransform;
    fragTexCoord = inTexCoord;
}