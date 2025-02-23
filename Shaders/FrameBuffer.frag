#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

void main()
{
    vec4 texColor = texture(screenTexture, TexCoords);
    
    // デバッグ用：テクスチャ座標を色として表示
    // FragColor = vec4(TexCoords.x, TexCoords.y, 0.0, 1.0);
    
    // 通常の描画
    FragColor = texColor;
}