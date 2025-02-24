#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D sceneTexture;
uniform float brightnessThreshold;

void main()
{
    vec4 color = texture(sceneTexture, TexCoords);
    float brightness = dot(color.rgb, vec3(0.2126, 0.7152, 0.0722)); // 輝度を計算

    if (brightness > brightnessThreshold) {
        FragColor = color; // 閾値を超える場合はそのまま出力
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0); // 閾値以下の場合は透明にする
    }
}