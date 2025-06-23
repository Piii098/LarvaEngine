#version 330

in vec2 fragPosition;

// ライト中心座標（ワールド空間またはスクリーン空間で統一して扱う）
uniform vec2 uLightCenter;

// ライトの色・強度・範囲など
uniform vec3 uLightColor = vec3(1.0, 0.3, 0.3);
uniform float uLightIntensity = 2.0;
uniform float uLightRange = 50.0; // 演算安定性のため少し小さめの値に調整

out vec4 outColor;

void main() 
{
    // 中心からの距離を計算
    float dist = distance(fragPosition, uLightCenter);

    // 逆2乗減衰を計算：基準となる距離を uLightRange とし、負荷を避けるため dist^2 + 1.0 に調整
    // clamp を用いて [0, 1] 内に収める
    float attenuation = clamp(uLightRange * uLightRange / (dist * dist + 1.0), 0.0, 1.0);

    // ライト色に減衰率と強度を掛け合わせる
    vec3 finalColor = uLightColor * attenuation * uLightIntensity;

    outColor = vec4(finalColor, 1.0);
}