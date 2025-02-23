// lighting.frag
#version 330 core

in vec3 FragPos; // ���_�V�F�[�_�[����n���ꂽ�ʒu
in vec3 Normal;  // ���_�V�F�[�_�[����n���ꂽ�@��

// ���C�e�B���O�p��uniform�ϐ�
uniform vec3 uLightPos;   // �����̈ʒu
uniform vec3 uLightColor; // �����̐F
uniform vec3 uViewPos;    // �J�����̈ʒu

// �I�u�W�F�N�g�ŗL�̐F
uniform vec3 uObjectColor;

out vec4 FragColor;

void main()
{
    // �@ Ambient�i�����j
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
    
    // �A Diffuse�i�g�U���j
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // �B Specular�i���ʔ��ˁj
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * uLightColor;
    
    // �e�������������čŏI�F������
    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    FragColor = vec4(result, 1.0);
}
