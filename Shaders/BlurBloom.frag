#version 330 core
in vec2 fragTexCoord;
out vec4 outColor;

uniform sampler2D uTexture;
uniform vec2 uDirection; // ぼかしの方向 (1.0, 0.0) または (0.0, 1.0)

void main() {
    vec2 texOffset = 1.0 / textureSize(uTexture, 0); // テクスチャのサイズに基づいてオフセットを計算
    vec3 result = texture(uTexture, fragTexCoord).rgb * 0.227027;

    result += texture(uTexture, fragTexCoord + texOffset * uDirection).rgb * 0.1945946;
    result += texture(uTexture, fragTexCoord - texOffset * uDirection).rgb * 0.1945946;

    result += texture(uTexture, fragTexCoord + 2.0 * texOffset * uDirection).rgb * 0.1216216;
    result += texture(uTexture, fragTexCoord - 2.0 * texOffset * uDirection).rgb * 0.1216216;

    result += texture(uTexture, fragTexCoord + 3.0 * texOffset * uDirection).rgb * 0.054054;
    result += texture(uTexture, fragTexCoord - 3.0 * texOffset * uDirection).rgb * 0.054054;

    outColor = vec4(result, 1.0);
}