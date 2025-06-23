#version 330 core

layout(location = 0) in vec3 inPosition; // 頂点の座標
layout(location = 1) in vec3 inNormal; // 頂点の法線
layout(location = 2) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main() {
    fragTexCoord = inTexCoord;
    gl_Position = vec4(inPosition, 1.0);
}