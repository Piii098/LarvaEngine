#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec4 texColor = texture(screenTexture, TexCoords);
    
    // �f�o�b�O�p�F�e�N�X�`�����W��F�Ƃ��ĕ\��
    // FragColor = vec4(TexCoords.x, TexCoords.y, 0.0, 1.0);
    
    // �ʏ�̕`��
    FragColor = texColor;
}