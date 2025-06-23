#version 330

in vec2 fragTexCoord;

out vec4 outColor; 

uniform sampler2D uTexture;
uniform vec3 uColor = vec3(1,1,1);
uniform float uAlpha = 1.0;

void main()
{
   
    outColor = texture(uTexture, fragTexCoord);
}