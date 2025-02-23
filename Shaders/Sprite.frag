#version 330

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 outColor;

uniform sampler2D uTexture;

uniform float brightness;

uniform vec3 ambientLightColor;    // 環境光の色
uniform float ambientLightIntensity; // 環境光の強度

uniform vec2 pointLightPos;    
uniform vec3 pointLightColor;  
uniform float pointLightIntensity; // 光の強度
uniform float pointLightRadius;    // 光の届く最大距離

void main()
{
	vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 ambientLight = texColor.rgb * ambientLightColor * ambientLightIntensity;

    //  距離ベースのライティング計算
    float dist = distance(fragPosition, pointLightPos);
    float attenuation = clamp(1.0 - dist / pointLightRadius, 0.0, 1.0); // 距離に応じた減衰
    vec3 pointLight = texColor.rgb * pointLightColor * attenuation * pointLightIntensity;

    // 環境光と点光源を合成
    vec3 finalColor = ambientLight + pointLight;

    outColor = vec4(finalColor, texColor.a); // アルファは元のテクスチャから取得
}
