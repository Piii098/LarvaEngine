#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;
// �J�[�l���T�C�Y�𑝂₵�āA���X���[�Y�ȃu���[���ʂ�����
uniform float weight[7] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 
                                 0.0540540541, 0.0162162162, 0.0047297297, 
                                 0.0012972973);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(image, 0);
    vec3 result = texture(image, TexCoords).rgb * weight[0];
    
    if(horizontal)
    {
        for(int i = 1; i < 7; ++i)
        {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 7; ++i)
        {
            result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}