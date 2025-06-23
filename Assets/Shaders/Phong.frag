#version 330

in vec2 fragTexCoord;
in vec3 fragWorldPos;  // この名前に合わせる
in vec3 fragNormal;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;
uniform vec3 uColor = {1,1,1};
uniform float uAlpha = 1.0;

struct DirectionalLight{
    vec3 direction;
    vec3 diffuseColor;
    vec3 specularColor; 
};

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecularPower = 100.0;  // デフォルト値を設定
uniform DirectionalLight uDirectionalLight;

void main()
{
    // 法線を正規化
    vec3 N = normalize(fragNormal);
    // ライトの方向ベクトル（反転）
    vec3 L = normalize(-uDirectionalLight.direction);
    // 視線ベクトル
    vec3 V = normalize(uCameraPos - fragWorldPos);
    // 反射ベクトル
    vec3 R = normalize(reflect(-L, N));

    // 環境光を初期値として設定
    vec3 Phong = uAmbientLight;
    
    // 拡散反射と鏡面反射の計算
    float NdotL = dot(N, L);
    if(NdotL > 0){
        vec3 diffuse = uDirectionalLight.diffuseColor * NdotL;
        vec3 specular = uDirectionalLight.specularColor * pow(max(dot(R, V), 0.0), uSpecularPower);
        Phong += diffuse + specular;
    }

    // テクスチャ色の取得
    vec4 texColor = texture(uTexture, fragTexCoord);
    
    // 最終的な色の計算（テクスチャ * ライティング * カラーモディファイア）
    vec3 finalColor = texColor.rgb * Phong * uColor;
    
    // 出力カラーの設定
    FragColor = vec4(finalColor, texColor.a * uAlpha);
    
}