#version 330 core
out vec4 FragColor;

in vec2 fragTexCoord;

uniform sampler2D sceneTexture;   // �V���[�v�Ȍ��摜
uniform sampler2D blurTexture;    // �ڂ������摜
uniform sampler2D depthTexture;   // �[�x���
uniform float focusDistance;      // �t�H�[�J�X����
uniform float focusRange;         // �t�H�[�J�X�͈�
uniform float maxBlur;            // �ő�ڂ�����
uniform float near;               // �j�A�N���b�s���O�v���[��
uniform float far;                // �t�@�[�N���b�s���O�v���[��

void main() {
    float depth = texture(depthTexture, fragTexCoord).r;
    
    // ���`�����ꂽ�[�x�l���v�Z
    float linearDepth = (2.0 * near) / (far + near - depth * (far - near));
    
    // �t�H�[�J�X��������̋����Ɋ�Â��ڂ����W��
    float focus = abs(linearDepth - focusDistance) / focusRange;
    float blur = clamp(focus, 0.0, maxBlur);
    
    // �V���[�v�ȉ摜�Ƃڂ������摜������
    vec3 color = mix(
        texture(sceneTexture, fragTexCoord).rgb,
        texture(blurTexture, fragTexCoord).rgb,
        blur
    );
    
    //�I�v�V�����F�f�o�b�O�\���i�t�H�[�J�X�͈͂̉����j
    vec3 debugFocus = vec3(focus); // �t�H�[�J�X��������̋��������o��
    FragColor = vec4(color, 1.0);  // �ʏ�̏o��
    
}