#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
uniform sampler2D frameTexture;

// �b��I��HSV�ϊ��ɗp����֐�
vec3 rgbToHsv(vec3 c) {
    float maxC = max(c.r, max(c.g, c.b));
    float minC = min(c.r, min(c.g, c.b));
    float delta = maxC - minC;

    float h = 0.0;
    float s = (maxC == 0.0) ? 0.0 : delta / maxC;
    float v = maxC;

    if(delta != 0.0) {
        if(maxC == c.r) {
            h = (c.g - c.b) / delta;
            if(c.g < c.b) {
                h += 6.0;
            }
        } else if(maxC == c.g) {
            h = (c.b - c.r) / delta + 2.0;
        } else { 
            h = (c.r - c.g) / delta + 4.0;
        }
        h /= 6.0;
    }

    return vec3(h, s, v);
}

// 臒l
uniform float threshold = 0.7;

void main()
{
    vec4 texColor = texture(frameTexture, TexCoords);
    FragColor = vec4(texColor.rgb, 1.0);

    // RGB �� HSV �ɕϊ�
    vec3 hsv = rgbToHsv(texColor.rgb);

    // Value �� threshold �𒴂����璊�o
    if (hsv.z > threshold) {
        BrightColor = vec4(texColor.rgb, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}