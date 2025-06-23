﻿#version 330 core

layout (location = 0) in vec3 inPosition;

uniform mat4 uWorldTransform;
uniform mat4 uLightViewProj;

void main() {
    vec4 pos = vec4(inPosition, 1.0) * uWorldTransform;

    gl_Position = pos * uLightViewProj ;
}