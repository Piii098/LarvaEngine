#version 330 core

in vec2 fragTexCoord;
in vec2 fragPosition;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;

uniform vec3 ambientLightColor;       // �����̐F
uniform float ambientLightIntensity;  // �����̋��x

uniform vec2 pointLightPos;    
uniform vec3 pointLightColor;  
uniform float pointLightIntensity;    // ���̋��x
uniform float pointLightRadius;       // ���̓͂��ő勗��

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    // �����x�[�X�̃��C�e�B���O�v�Z
    float dist = distance(fragPosition, pointLightPos);
    float attenuation = clamp(1.0 - dist / pointLightRadius, 0.0, 1.0); // �����ɉ���������
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // �����Ɠ_����������
    vec3 finalColor = ambientLight + pointLight;

    FragColor = vec4(finalColor, texColor.a); // �A���t�@�͌��̃e�N�X�`������擾

    // �P�x�̌v�Z�Əo��
    float brightnessValue = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightnessValue > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor.w);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, FragColor.w);
}