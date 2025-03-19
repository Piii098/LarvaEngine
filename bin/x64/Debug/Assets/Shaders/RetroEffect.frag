#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec2 resolution;
uniform float time;

const float CRT_CURVE_AMOUNT = 0.1;
const float SCANLINE_INTENSITY = 0.1;
const float NOISE_INTENSITY = 0.05;
const vec3 BORDER_COLOR = vec3(0.5, 0.5, 0.5); // 単色の色 (黒)

vec2 applyCRTCurve(vec2 uv) {
    uv = uv * 2.0 - 1.0;
    vec2 offset = uv * length(uv) * CRT_CURVE_AMOUNT;
    uv += offset;
    uv = uv * 0.5 + 0.5;
    return uv;
}

float applyScanline(float intensity, vec2 uv) {
    float scanline = sin(uv.y * resolution.y * 3.141592 * 2.0) * 0.5 + 0.5;
    return mix(1.0, scanline, intensity);
}

float applyNoise(float intensity) {
    return mix(1.0, fract(sin(dot(gl_FragCoord.xy, vec2(12.9898, 78.233))) * 43758.5453), intensity);
}

void main() {
    vec2 uv = TexCoords;
    
    // Apply CRT curve effect
    uv = applyCRTCurve(uv);

    // Check if uv is out of bounds
    if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
        FragColor = vec4(BORDER_COLOR, 1.0);
        return;
    }
    
    // Sample the texture
    vec4 color = texture(uTexture, uv);
    
    // Apply scanline effect
    color.rgb *= applyScanline(SCANLINE_INTENSITY, uv);
    
    // Apply noise effect
    color.rgb *= applyNoise(NOISE_INTENSITY);
    
    FragColor = color;
}