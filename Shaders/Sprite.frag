#version 330

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 outColor;

uniform sampler2D uTexture;

uniform float brightness;

uniform vec3 ambientLightColor;    // �����̐F
uniform float ambientLightIntensity; // �����̋��x

uniform vec2 pointLightPos;    
uniform vec3 pointLightColor;  
uniform float pointLightIntensity; // ���̋��x
uniform float pointLightRadius;    // ���̓͂��ő勗��

void main()
{
	vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    //  �����x�[�X�̃��C�e�B���O�v�Z
    float dist = distance(fragPosition, pointLightPos);
    float attenuation = clamp(1.0 - dist / pointLightRadius, 0.0, 1.0); // �����ɉ���������
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // �����Ɠ_����������
    vec3 finalColor = ambientLight + pointLight;

    outColor = vec4(finalColor, texColor.a); // �A���t�@�͌��̃e�N�X�`������擾
}
