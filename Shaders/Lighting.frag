// lighting.frag
#version 330 core

in vec3 FragPos; // 頂点シェーダーから渡された位置
in vec3 Normal;  // 頂点シェーダーから渡された法線

// ライティング用のuniform変数
uniform vec3 uLightPos;   // 光源の位置
uniform vec3 uLightColor; // 光源の色
uniform vec3 uViewPos;    // カメラの位置

// オブジェクト固有の色
uniform vec3 uObjectColor;

out vec4 FragColor;

void main()
{
    // ① Ambient（環境光）
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * uLightColor;
    
    // ② Diffuse（拡散光）
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(uLightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;
    
    // ③ Specular（鏡面反射）
    float specularStrength = 0.5;
    vec3 viewDir = normalize(uViewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * uLightColor;
    
    // 各成分を合成して最終色を決定
    vec3 result = (ambient + diffuse + specular) * uObjectColor;
    FragColor = vec4(result, 1.0);
}
