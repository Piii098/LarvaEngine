#version 330 core

layout(location = 0) in vec3 inPosition; // 頂点座標
layout(location = 1) in vec2 inTexCoord; // テクスチャ座標

out vec2 fragTexCoord;

uniform mat4 uViewProj;    // ビュー投影行列

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragTexCoord = inTexCoord;
}
