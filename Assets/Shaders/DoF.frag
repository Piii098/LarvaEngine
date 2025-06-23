#version 330 core
out vec4 FragColor;

in vec2 fragTexCoord;

uniform sampler2D sceneTexture;   // シャープな元画像
uniform sampler2D blurTexture;    // ぼかした画像
uniform sampler2D depthTexture;   // 深度情報
uniform float focusDistance;      // フォーカス距離
uniform float focusRange;         // フォーカス範囲
uniform float maxBlur;            // 最大ぼかし量
uniform float near;               // ニアクリッピングプレーン
uniform float far;                // ファークリッピングプレーン

void main() {
    float depth = texture(depthTexture, fragTexCoord).r;
    
    // 線形化された深度値を計算
    float linearDepth = (2.0 * near) / (far + near - depth * (far - near));
    
    // フォーカス距離からの距離に基づくぼかし係数
    float focus = abs(linearDepth - focusDistance) / focusRange;
    float blur = clamp(focus, 0.0, maxBlur);
    
    // シャープな画像とぼかした画像を混合
    vec3 color = mix(
        texture(sceneTexture, fragTexCoord).rgb,
        texture(blurTexture, fragTexCoord).rgb,
        blur
    );
    
    //オプション：デバッグ表示（フォーカス範囲の可視化）
    vec3 debugFocus = vec3(focus); // フォーカス距離からの距離を視覚化
    FragColor = vec4(color, 1.0);  // 通常の出力
    
}