// lighting.vert
#version 330 core

layout(location = 0) in vec3 aPos;    // 頂点位置
layout(location = 1) in vec3 aNormal; // 頂点法線

// ワールド変換行列と射影変換行列（カメラ用）
uniform mat4 uWorldTransform;
uniform mat4 uViewProjTransform;

// 法線変換用の3x3行列（ワールド変換の上位3x3の逆転置行列）
uniform mat3 uNormalMatrix;

out vec3 FragPos; // フラグメント側へ渡すワールド空間での位置
out vec3 Normal;  // 変換後の法線

void main()
{
    // ワールド座標に変換
    vec4 worldPos = uWorldTransform * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;
    
    // 法線も正しく変換（正規化しておく）
    Normal = normalize(uNormalMatrix * aNormal);
    
    // ビュー・射影行列をかけて最終クリップ空間の位置へ
    gl_Position = uViewProjTransform * worldPos;
}
