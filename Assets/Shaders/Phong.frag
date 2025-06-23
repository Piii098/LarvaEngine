#version 330

in vec2 fragTexCoord;
in vec3 fragWorldPos;  // ���̖��O�ɍ��킹��
in vec3 fragNormal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;
uniform vec3 uColor = {1,1,1};
uniform float uAlpha = 1.0;

struct DirectionalLight{
    vec3 direction;
    vec3 diffuseColor;
    vec3 specularColor; 
};

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecularPower = 100.0;  // �f�t�H���g�l��ݒ�
uniform DirectionalLight uDirectionalLight;

void main()
{
    // �@���𐳋K��
    vec3 N = normalize(fragNormal);
    // ���C�g�̕����x�N�g���i���]�j
    vec3 L = normalize(-uDirectionalLight.direction);
    // �����x�N�g��
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // ���˃x�N�g��
    vec3 R = normalize(reflect(-L, N));

    // �����������l�Ƃ��Đݒ�
    vec3 Phong = uAmbientLight;
    
    // �g�U���˂Ƌ��ʔ��˂̌v�Z
    float NdotL = dot(N, L);
    if(NdotL > 0){
        vec3 diffuse = uDirectionalLight.diffuseColor * NdotL;
        vec3 specular = uDirectionalLight.specularColor * pow(max(dot(R, V), 0.0), uSpecularPower);
        Phong += diffuse + specular;
    }

    // �e�N�X�`���F�̎擾
    vec4 texColor = texture(uTexture, fragTexCoord);
    
    // �ŏI�I�ȐF�̌v�Z�i�e�N�X�`�� * ���C�e�B���O * �J���[���f�B�t�@�C�A�j
    vec3 finalColor = texColor.rgb * Phong * uColor;
    
    // �o�̓J���[�̐ݒ�
    FragColor = vec4(finalColor, texColor.a * uAlpha);
    
}