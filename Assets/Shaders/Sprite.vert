#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

uniform vec2 uTexOffset = {0.0, 0.0}; 
uniform vec2 uTexScale = {1.0, 1.0}; 

void main(){

	vec4 worldPos = vec4(inPosition, 1.0) * uWorldTransform;

	gl_Position = worldPos * uViewProj; 

	fragTexCoord = inTexCoord * uTexScale + uTexOffset;

}
