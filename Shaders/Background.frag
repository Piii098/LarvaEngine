#version 330 core

in vec2 fragTexCoord;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform sampler2D uTexture;           // ”wŒiƒeƒNƒXƒ`ƒƒ
uniform vec3 ambientLightColor;       // ŠÂ‹«Œõ‚ÌF
uniform float ambientLightIntensity;   // ”wŒi‚Ì–¾‚é‚³’²®

void main() {
    vec4 texColor = texture(uTexture, fragTexCoord);
    vec3 finalColor = texColor.rgb * ambientLightColor * ambientLightIntensity;
    FragColor = vec4(finalColor, texColor.a);

    float brightnessValue = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightnessValue > 1.0)
        BrightColor = vec4(FragColor.rgb, FragColor.w);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, FragColor.w);
}
