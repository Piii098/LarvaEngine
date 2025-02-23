#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewScreen;
uniform mat4 uViewProj;

layout(location = 0) in vec3 inPosition; // ���_�̍��W
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec2 fragPosition;

uniform vec2 uTexOffset;
uniform vec2 uTexScale; 

void main(){

	vec4 worldPos = vec4(inPosition, 1.0) * uWorldTransform;

	vec4 screenPos = worldPos * uViewScreen;

	gl_Position = uViewProj * screenPos; // ���ݓǂݍ��ݒ��̒��_�̃��[���h�s������

	fragTexCoord = inTexCoord * uTexScale + uTexOffset;
	fragPosition = worldPos.xy;
}
