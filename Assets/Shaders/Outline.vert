#version 330

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform mat4 uScaleMatrix; // ���f�����g�傷�邽�߂̍s��

void main() {
    // ���_�ʒu�������g��
    vec4 scaledPos = uScaleMatrix * vec4(inPosition, 1.0);
    gl_Position = uViewProj * uWorldTransform * scaledPos;
}