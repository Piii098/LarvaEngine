#version 330

layout(location = 0) out vec4 FragColor;

uniform vec4 uOutlineColor;

void main() {
    FragColor = uOutlineColor;
}