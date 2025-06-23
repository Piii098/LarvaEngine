#version 330 core
in vec2 fragTexCoord;
out vec4 FragColor;

uniform sampler2D uDepthTexture;

void main()
{
    float depth = texture(uDepthTexture, fragTexCoord).r;
    FragColor = vec4(vec3(depth), 1.0); // グレースケールで表示

}