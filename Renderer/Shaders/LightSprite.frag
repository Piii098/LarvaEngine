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

uniform vec3 selfLightColor;          // ���̎��̂̔����F
uniform float selfLightIntensity;     // ���̎��̂̔������x

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);

    // �����̌v�Z
    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    // �����x�[�X�̃��C�e�B���O�v�Z
    vec2 lightDir = pointLightPos - fragPosition;
    float distSq = dot(lightDir, lightDir);
    float dist = sqrt(distSq);
    float attenuation = smoothstep(pointLightRadius, 0.0, dist); 
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // ���Ȕ����̌v�Z
    vec3 selfLight = selfLightColor * selfLightIntensity;

    // �����Ɠ_����������
    vec3 finalColor = ambientLight + pointLight + selfLight;

    FragColor = vec4(finalColor, texColor.a); // �A���t�@�͌��̃e�N�X�`������擾

    // �P�x�̌v�Z�Əo��
    float brightnessValue = dot(finalColor, vec3(0.2126, 0.7152, 0.0722)); 
    BrightColor = brightnessValue > 0.9 ? vec4(finalColor, texColor.a) : vec4(0.0, 0.0, 0.0, texColor.a);
}