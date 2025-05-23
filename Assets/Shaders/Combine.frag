#version 330

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 outColor;

uniform sampler2D spriteTexture;
uniform sampler2D lightTexture;

uniform vec3 uAmbientColor;
uniform float uAmbientIntensity;

void main(){
	vec4 spriteColor = texture(spriteTexture, fragTexCoord);

	vec3 spriteLightColor = spriteColor.rgb * uAmbientIntensity * uAmbientColor;

	vec4 lightColor = texture(lightTexture, fragTexCoord);

	outColor = vec4(spriteLightColor + lightColor.rgb * lightColor.a, spriteColor.a);
}