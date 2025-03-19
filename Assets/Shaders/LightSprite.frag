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

uniform vec3 selfLightColor;          // 物体自体の発光色
uniform float selfLightIntensity;     // 物体自体の発光強度

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);

    // 環境光の計算
    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    // 距離ベースのライティング計算
    vec2 lightDir = pointLightPos - fragPosition;
    float distSq = dot(lightDir, lightDir);
    float dist = sqrt(distSq);
    float attenuation = smoothstep(pointLightRadius, 0.0, dist); 
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // 自己発光の計算
    vec3 selfLight = selfLightColor * selfLightIntensity;

    // 環境光と点光源を合成
    vec3 finalColor = ambientLight + pointLight + selfLight;

    FragColor = vec4(finalColor, texColor.a); // アルファは元のテクスチャから取得

    // 輝度の計算と出力
    float brightnessValue = dot(finalColor, vec3(0.2126, 0.7152, 0.0722)); 
    BrightColor = brightnessValue > 0.9 ? vec4(finalColor, texColor.a) : vec4(0.0, 0.0, 0.0, texColor.a);
}