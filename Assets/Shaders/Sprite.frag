#version 330

in vec2 fragTexCoord;

out vec4 outColor; // これだけでOK

uniform sampler2D uTexture;
uniform vec3 uColor = vec3(1,1,1);
uniform float uAlpha = 1.0;

void main()
{
    vec4 texColor = texture(uTexture, fragTexCoord);
    vec3 color = uColor * texColor.rgb;
    color = clamp(color, 0.0, 1.0);
    float alpha = texColor.a * uAlpha;
    outColor = texColor;
}