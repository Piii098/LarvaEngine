#version 330 core

layout(location = 0) in vec3 inPosition; // ���_���W
layout(location = 1) in vec2 inTexCoord; // �e�N�X�`�����W

out vec2 fragTexCoord;

uniform mat4 uViewProj;    // �r���[���e�s��

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}
