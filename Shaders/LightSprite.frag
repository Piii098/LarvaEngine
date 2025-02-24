#version 330 core

in vec2 fragTexCoord;
in vec2 fragPosition;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;

uniform vec3 ambientLightColor;       // 環境光の色
uniform float ambientLightIntensity;  // 環境光の強度

uniform vec2 pointLightPos;    
uniform vec3 pointLightColor;  
uniform float pointLightIntensity;    // 光の強度
uniform float pointLightRadius;       // 光の届く最大距離

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    // 距離ベースのライティング計算
    float dist = distance(fragPosition, pointLightPos);
    float attenuation = clamp(1.0 - dist / pointLightRadius, 0.0, 1.0); // 距離に応じた減衰
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // 環境光と点光源を合成
    vec3 finalColor = ambientLight + pointLight;

    FragColor = vec4(finalColor, texColor.a); // アルファは元のテクスチャから取得

    // 輝度の計算と出力
    float brightnessValue = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightnessValue > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor.w);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, FragColor.w);
}