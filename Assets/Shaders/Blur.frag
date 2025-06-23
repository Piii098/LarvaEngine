#version 330 core
out vec4 FragColor;
  
in vec2 fragTexCoord;

uniform sampler2D uTexture;
uniform bool horizontal;
// カーネルサイズを増やして、よりスムーズなブラー効果を実現
uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main()
{             
    vec2 tex_offset = 1.0 / textureSize(uTexture, 0);
    vec3 result = texture(uTexture, fragTexCoord).rgb * weight[0];
    
    if(horizontal)
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(uTexture, fragTexCoord + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(uTexture, fragTexCoord - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
        }
    }
    else
    {
        for(int i = 1; i < 5; ++i)
        {
            result += texture(uTexture, fragTexCoord + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
            result += texture(uTexture, fragTexCoord - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}