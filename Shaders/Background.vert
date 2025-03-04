#version 330 core

layout(location = 0) in vec3 inPosition; // 頂点座標
layout(location = 1) in vec2 inTexCoord; // テクスチャ座標

out vec2 fragTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;    // ビュー投影行列
uniform float uCameraPosX;   // カメラ位置
uniform float uTextureWidth; 

void main() {
    vec4 worldPos = vec4(inPosition, 1.0) * uWorldTransform;

    gl_Position = uViewProj * worldPos;

    fragTexCoord = inTexCoord;
}