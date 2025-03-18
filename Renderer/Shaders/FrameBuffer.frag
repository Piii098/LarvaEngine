#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D frameTexture;

void main()
{
    vec4 texColor = texture(frameTexture, TexCoords);
    // アンビエントライトを乗算（必要に応じてRGBのみに適用など調整可）
    FragColor = vec4(texColor.rgb, texColor.a);


}