#version 330 core

in vec2 fragTexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;           // �w�i�e�N�X�`��
uniform vec3 ambientLightColor;       // �����̐F
uniform float ambientLightIntensity;   // �w�i�̖��邳����

uniform vec3 selfLightColor;          // ���̎��̂̔����F
uniform float selfLightIntensity;  

void main() {
    vec4 texColor = texture(uTexture, fragTexCoord);

    // ���Ȕ����̌v�Z
    vec3 selfLight = selfLightColor * selfLightIntensity;
    
    vec3 finalColor = texColor.rgb * ambientLightColor * ambientLightIntensity * selfLight;
    FragColor = vec4(finalColor, texColor.a);

    float brightnessValue = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightnessValue > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor.w);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, FragColor.w);
}
