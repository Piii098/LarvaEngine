// Bloom.frag (�t���O�����g�V�F�[�_�[)
#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D sceneTexture;

void main()
{
    FragColor = texture(sceneTexture, TexCoords);
}