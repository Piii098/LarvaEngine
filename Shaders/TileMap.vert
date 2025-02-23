#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;
uniform vec2 uTexOffset; // �ǉ��F�e�N�X�`���I�t�Z�b�g
uniform vec2 uTexScale;  // �ǉ��F�e�N�X�`���X�P�[��

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main() {
    vec4 pos = vec4(inPosition, 1.0);
    gl_Position = pos * uWorldTransform * uViewProj;
    
    // ���̓e�N�X�`�����W�ɑ΂��ăX�P�[���ƃI�t�Z�b�g��K�p
    fragTexCoord = inTexCoord * uTexScale + uTexOffset;
}