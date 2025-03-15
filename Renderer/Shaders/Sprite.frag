#version 330

in vec2 fragTexCoord;
in vec2 fragPosition;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

out vec4 outColor;

uniform sampler2D uTexture;
uniform vec3 uColor = {1,1,1};

void main()
{
	vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 color = uColor * texColor.rgb;

    color = clamp(color, 0.0, 1.0);

    outColor = vec4(color, texColor.a); // アルファは元のテクスチャから取得

	// 輝度の計算と出力
    float brightnessValue = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightnessValue > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor.w);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, FragColor.w);
}
