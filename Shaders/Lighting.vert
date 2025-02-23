// lighting.vert
#version 330 core

layout(location = 0) in vec3 aPos;    // ���_�ʒu
layout(location = 1) in vec3 aNormal; // ���_�@��

// ���[���h�ϊ��s��Ǝˉe�ϊ��s��i�J�����p�j
uniform mat4 uWorldTransform;
uniform mat4 uViewProjTransform;

// �@���ϊ��p��3x3�s��i���[���h�ϊ��̏��3x3�̋t�]�u�s��j
uniform mat3 uNormalMatrix;

out vec3 FragPos; // �t���O�����g���֓n�����[���h��Ԃł̈ʒu
out vec3 Normal;  // �ϊ���̖@��

void main()
{
    // ���[���h���W�ɕϊ�
    vec4 worldPos = uWorldTransform * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // �@�����������ϊ��i���K�����Ă����j
    Normal = normalize(uNormalMatrix * aNormal);
    
    // �r���[�E�ˉe�s��������čŏI�N���b�v��Ԃ̈ʒu��
    gl_Position = uViewProjTransform * worldPos;
}
