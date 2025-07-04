﻿#version 330

in vec2 fragTexCoord;
in vec2 fragPosition;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
out vec4 outColor;

uniform sampler2D uTexture;
uniform vec3 uColor = {1,1,1};
uniform float uAlpha = 1.0;

void main()
{
	vec4 texColor = texture(uTexture, fragTexCoord);

    vec3 color = uColor * texColor.rgb;

    color = clamp(color, 0.0, 1.0);

    float alpha = texColor.a * uAlpha;

    outColor = vec4(color, 1.0); // アルファは元のテクスチャから取得

}
